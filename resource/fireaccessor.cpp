#include "fireaccessor.h"

#include "resource/configurationmanager.h"
#include "resource/dailyhotperiods.h"
#include "resource/gpiocontroller.h"
#include "resource/fireaccessor.h"
#include "resource/hotperiod.h"
#include "resource/resourceaccessor.h"
#include "resource/temperatureaccessor.h"
#include "resource/temperatureconfiguration.h"
#include "resource/temperaturethresholds.h"


FireAccessor::FireAccessor(QObject *parent) : QObject(parent)
{
    connect(ConfigurationManager::get(), &ConfigurationManager::fireModeChanged,
            this,                        &FireAccessor::reEvaluteLastTemperature);
    connect(ConfigurationManager::get(), &ConfigurationManager::fireModeChanged,
            this,                        &FireAccessor::modeChanged);
    connect(ConfigurationManager::get()->getTemperatureConfiguration(),
            &TemperatureConfiguration::changed,
            this,
            &FireAccessor::reEvaluteLastTemperature);
}

const bool &FireAccessor::isFireOn() const
{
    return GpioController::get()->getFireOn();
}

void FireAccessor::setFireOn(const bool &on)
{
    if(on != isFireOn())
    {
        GpioController::get()->setFireOn(on);
        emit fireOnChanged();

        // Manage some special cases
        if(ConfigurationManager::get()->getFireMode() == FireMode::LongHoliday)
        {
            ConfigurationManager::get()->setFireMode(FireMode::Automatic);
        }

        if(ConfigurationManager::get()->getFireMode() == FireMode::Automatic ||
           ConfigurationManager::get()->getFireMode() == FireMode::ShortHoliday)
        {
            const TemperatureThresholds *thresholds = currentThresholds();
            QVariant temperature = getLastTemperature();

            if(!temperature.isNull() &&
               ((on && temperature.toReal() > thresholds->getMax()) ||
                (!on && temperature.toReal() < thresholds->getMin())))
            {
                ConfigurationManager::get()->setFireMode(FireMode::Manual);
            }
        }
    }
}

bool FireAccessor::isModeAuto() const
{
    return ConfigurationManager::get()->getFireMode() == FireMode::Automatic;
}

void FireAccessor::setModeAuto(const bool &modeAuto)
{
    if(modeAuto != isModeAuto())
    {
        if(isModeAuto())
        {
            ConfigurationManager::get()->setFireMode(FireMode::Manual);
        }
        else
        {
            ConfigurationManager::get()->setFireMode(FireMode::Automatic);
        }
    }
}

void FireAccessor::onNewTemperatureReceived(const qreal &temperature)
{
    FireMode::Enum mode = ConfigurationManager::get()->getFireMode();
    if((mode == FireMode::LongHoliday || mode == FireMode::ShortHoliday) &&
       QDateTime::currentDateTime() > ConfigurationManager::get()->getHolidayEnd())
    {
        ConfigurationManager::get()->setFireMode(FireMode::Automatic);
        return;
    }

    if(mode == FireMode::Automatic || mode == FireMode::ShortHoliday)
    {
        const TemperatureThresholds *thresholds = currentThresholds();

        if(isFireOn())
        {
            if(temperature > thresholds->getMax())
            {
                setFireOn(false);
            }
        }
        else
        {
            if(temperature < thresholds->getMin())
            {
                setFireOn(true);
            }
        }
    }
    else if(mode == FireMode::LongHoliday)
    {
        setFireOn(false);
    }
}

const FireMode::Enum &FireAccessor::getMode() const
{
    return ConfigurationManager::get()->getFireMode();
}

void FireAccessor::startHoliday(const QDateTime &destinationDate, const FireMode::Enum &mode)
{
    ConfigurationManager::get()->setFireMode(mode, destinationDate);
}

void FireAccessor::setConfiguration(TemperatureConfiguration *config)
{
    ConfigurationManager::get()->setTemperatureConfiguration(*config);
}

const TemperatureThresholds *FireAccessor::currentThresholds() const
{
    const TemperatureConfiguration *config =
            ConfigurationManager::get()->getTemperatureConfiguration();

    if(ConfigurationManager::get()->getFireMode() == FireMode::ShortHoliday)
    {
        return config->getThresholdsCold();
    }

    QDateTime dateTime = QDateTime::currentDateTime();
    const QList<DailyHotPeriods *> &hotPeriods = config->getHotPeriodsRaw();
    DailyHotPeriods *dayPeriods = hotPeriods[dateTime.date().dayOfWeek() - 1];

    for(const HotPeriod *period : dayPeriods->getActivePeriods())
    {
        if(period->contains(dateTime.time()))
        {
            return config->getThresholdsHot();
        }
    }

    return config->getThresholdsCold();
}

void FireAccessor::reEvaluteLastTemperature()
{
    QVariant value = getLastTemperature();
    if(!value.isNull())
    {
        onNewTemperatureReceived(value.toReal());
    }
}

QVariant FireAccessor::getLastTemperature() const
{
    TemperatureAccessor *temp =
            ResourceAccessor::get()->property("temperature").value<TemperatureAccessor *>();
    return temp->getLastTemperature();
}
