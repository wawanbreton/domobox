#include "temperatureconfiguration.h"

#include "resource/configurationmanager.h"
#include "resource/dailyhotperiods.h"
#include "resource/temperaturethresholds.h"


TemperatureConfiguration::TemperatureConfiguration(QObject *parent) :
    QObject(parent),
    _thresholdsHot(new TemperatureThresholds(this)),
    _thresholdsCold(new TemperatureThresholds(this))
{
    connect(_thresholdsHot,  &TemperatureThresholds::changed,
            this,            &TemperatureConfiguration::changed);
    connect(_thresholdsCold, &TemperatureThresholds::changed,
            this,            &TemperatureConfiguration::changed);

    for(int day = 0 ; day < 7 ; ++day)
    {
        DailyHotPeriods *hotPeriod = new DailyHotPeriods(this);
        connect(hotPeriod, &DailyHotPeriods::changed, this, &TemperatureConfiguration::changed);
        _hotPeriods.append(hotPeriod);
    }
}

void TemperatureConfiguration::operator=(const TemperatureConfiguration &other)
{
    (*_thresholdsHot) = (*other.getThresholdsHot());
    (*_thresholdsCold) = (*other.getThresholdsCold());

    for(int day = 0 ; day < 7 ; ++day)
    {
        (*_hotPeriods.at(day)) = (*other.getHotPeriodsRaw().at(day));
    }
}

void TemperatureConfiguration::loadFromConfig()
{
    (*this) = (*ConfigurationManager::get()->getTemperatureConfiguration());
}

void TemperatureConfiguration::load(const QSettings &settings, const QString &prefix)
{
    _thresholdsCold->load(settings, QString("%1/thresholds_cold").arg(prefix), 18.0, 21.0);
    _thresholdsHot->load(settings, QString("%1/thresholds_hot").arg(prefix), 20, 22);

    for(int day = 0 ; day < 7 ; ++day)
    {
        _hotPeriods.at(day)->load(settings, QString("%1/periods_%2").arg(prefix).arg(day));
    }
}

void TemperatureConfiguration::save(QSettings &settings, const QString &prefix) const
{
    _thresholdsCold->save(settings, QString("%1/thresholds_cold").arg(prefix));
    _thresholdsHot->save(settings, QString("%1/thresholds_hot").arg(prefix));

    for(int day = 0 ; day < 7 ; ++day)
    {
        _hotPeriods.at(day)->save(settings, QString("%1/periods_%2").arg(prefix).arg(day));
    }
}

void TemperatureConfiguration::setThresholdsHot(const TemperatureThresholds *thresholds)
{
    (*_thresholdsHot) = (*thresholds);
}

void TemperatureConfiguration::setThresholdsCold(const TemperatureThresholds *thresholds)
{
    (*_thresholdsCold) = (*thresholds);
}

QQmlListProperty<DailyHotPeriods> TemperatureConfiguration::getHotPeriods()
{
    return QQmlListProperty<DailyHotPeriods>(this, &_hotPeriods);
}
