#include "temperaturethresholds.h"

#include <QSettings>
#include <QVariant>


TemperatureThresholds::TemperatureThresholds(QObject *parent) :
    QObject(parent)
{
    connect(this, &TemperatureThresholds::minChanged, this, &TemperatureThresholds::changed);
    connect(this, &TemperatureThresholds::maxChanged, this, &TemperatureThresholds::changed);
}

void TemperatureThresholds::load(const QSettings &settings,
                                 const QString &prefix,
                                 const qreal &defaultMin,
                                 const qreal &defaultMax)
{
    _min = load(settings, QString("%1/min").arg(prefix), defaultMin);
    _max = load(settings, QString("%1/max").arg(prefix), defaultMax);
}

void TemperatureThresholds::save(QSettings &settings, const QString &prefix) const
{
    settings.setValue(QString("%1/min").arg(prefix), _min);
    settings.setValue(QString("%1/max").arg(prefix), _max);
}

void TemperatureThresholds::operator=(const TemperatureThresholds &other)
{
    setMin(other._min, false);
    setMax(other._max, false);
}

void TemperatureThresholds::setMax(const qreal &max, const bool &adjustMin)
{
    if(max != _max)
    {
        _max = max;
        emit maxChanged();

        if(adjustMin)
        {
            setMin(std::min(_min, _max - minDelta));
        }
    }
}

void TemperatureThresholds::setMin(const qreal &min, const bool &adjustMax)
{
    if(min != _min)
    {
        _min = min;
        emit minChanged();

        if(adjustMax)
        {
            setMax(std::max(_max, _min + minDelta));
        }
    }
}

qreal TemperatureThresholds::load(const QSettings &settings,
                                  const QString &identifier,
                                  const qreal defaultValue)
{
    QVariant value = settings.value(identifier);
    if(!value.isNull() && value.canConvert(QVariant::Double))
    {
        return value.toReal();
    }
    else
    {
        return defaultValue;
    }
}
