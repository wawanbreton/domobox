#include "configurationmanager.h"

#include <QSettings>

#include "resource/dailyhotperiods.h"
#include "resource/hotperiod.h"
#include "resource/temperatureconfiguration.h"
#include "resource/temperaturethresholds.h"


ConfigurationManager *ConfigurationManager::_instance = nullptr;

ConfigurationManager::ConfigurationManager(QObject *parent) :
    QObject(parent),
    _temperatureConfig(new TemperatureConfiguration(this))
{
    _instance = this;

    const QSettings settings;

    _fireMode = FireMode::parse(QSettings().value("fire/mode").toString());
    _holidayEnd = settings.value("fire/holiday_end").toDateTime();

    _temperatureConfig->load(settings, "fire");
    connect(_temperatureConfig, &TemperatureConfiguration::changed,
            this,                                    &ConfigurationManager::onPeriodsConfigurationChanged);
}

void ConfigurationManager::setFireMode(FireMode::Enum fireMode, const QDateTime &holidayEnd)
{
    if(fireMode != _fireMode || holidayEnd != _holidayEnd)
    {
        QSettings settings;

        _fireMode = fireMode;
        _holidayEnd = holidayEnd;
        settings.setValue("fire/mode", FireMode::toString(_fireMode));
        settings.setValue("fire/holiday_end", _holidayEnd);
        emit fireModeChanged(_fireMode);
    }
}

void ConfigurationManager::setTemperatureConfiguration(const TemperatureConfiguration &config)
{
    _configChanged = false;

    (*_temperatureConfig) = config;

    if(_configChanged)
    {
        _configChanged = false;

        QSettings settings;
        _temperatureConfig->save(settings, "fire");
    }
}

void ConfigurationManager::init(QObject *parent)
{
    _instance = new ConfigurationManager(parent);
}
