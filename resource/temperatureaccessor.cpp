#include "temperatureaccessor.h"

#include <sensors/sensors.h>

#include <QDebug>
#include <QLocale>


TemperatureAccessor::TemperatureAccessor(QObject *parent) :
    QObject(parent)
{
    if(sensors_init(nullptr) == 0)
    {
        sensors_chip_name const * chipName;
        int chipIndex = 0;
        while((chipName = sensors_get_detected_chips(0, &chipIndex)) && !_chipName)
        {
            sensors_feature const *feat;
            int features = 0;

            while((feat = sensors_get_features(chipName, &features)) && !_chipName)
            {
                sensors_subfeature const *subFeatures;
                int subFeatureIndex = 0;

                while((subFeatures = sensors_get_all_subfeatures(chipName, feat, &subFeatureIndex)) && !_chipName)
                {
                    if(subFeatures->flags & SENSORS_MODE_R)
                    {
                        _chipName = chipName;
                        _featureIndex = subFeatures->number;

                        qInfo() << "Found temperature sensors" << chipName->prefix << chipName->path;
                    }
                }
            }
        }
    }
    else
    {
        qWarning() << "Sensors initialization error";
    }

    if(_chipName)
    {
        updateTemperature();

        #ifdef ENV_SIMULATOR
        startTimer(500);
        #else
        startTimer(60000);
        #endif
    }
    else
    {
        qWarning() << "No temperature sensor found !";
    }
}

QString TemperatureAccessor::getTemperatureStr() const
{
    if(_history.isEmpty())
    {
        return Constants::empty;
    }
    else
    {
        return QLocale().toString(_history.last().value, 'f', 1) + " °C";
    }
}

QList<QDateTime> TemperatureAccessor::getHistoryTimestamps() const
{
    QList<QDateTime> result;

    for(const TemperatureValue &value : _history)
    {
        result.append(value.timestamp);
    }

    return result;
}

QVariantList TemperatureAccessor::getHistoryValues() const
{
    QVariantList result;

    for(const TemperatureValue &value : _history)
    {
        result.append(value.value);
    }

    return result;
}

QVariant TemperatureAccessor::getLastTemperature() const
{
    if(!_history.isEmpty())
    {
        return _history.last().value;
    }
    else
    {
        return QVariant();
    }
}

void TemperatureAccessor::timerEvent(QTimerEvent *event)
{
    updateTemperature();

    return QObject::timerEvent(event);
}

void TemperatureAccessor::updateTemperature()
{
    qreal temperature;
    int result = sensors_get_value(_chipName, _featureIndex, &temperature);
    if(result < 0)
    {
        qWarning() << "Temperature reading error" << result;
    }
    else
    {
        TemperatureValue value;
        value.timestamp = QDateTime::currentDateTimeUtc();
        value.value = temperature;

        _history.append(value);

        removeOldHistory(value.timestamp);

        updateChartLimits();

        emit temperatureChanged(temperature);
    }
}

void TemperatureAccessor::updateChartLimits()
{
    _gridValues.clear();
    _minorGridDates.clear();
    _gridDates.clear();
    _minDate = QDateTime();
    _maxDate = QDateTime();
    _minValue = 0.0;
    _maxValue = 0.0;

    if(!_history.isEmpty())
    {
        // Min and max values
        if(_history.count() == 1)
        {
            _minValue = _history[0].value - minValueDelta / 2;
            _maxValue = _history[0].value + minValueDelta / 2;
            _minDate = _history[0].timestamp.addSecs(-60 * 60 * 12);
            _maxDate = _history[0].timestamp.addSecs(60 * 60 * 12);
        }
        else
        {
            _minValue = std::numeric_limits<qreal>::max();
            _maxValue = std::numeric_limits<qreal>::lowest();

            for(const TemperatureValue &value : _history)
            {
                _minValue = std::min(_minValue, value.value);
                _maxValue = std::max(_maxValue, value.value);
            }

            // Values are stored in growing date order
            _minDate = _history.first().timestamp;
            _maxDate = _history.last().timestamp;

            if(_minDate.daysTo(_maxDate) < 1)
            {
                qint64 deltaMsecs = _minDate.msecsTo(_maxDate);
                QDateTime averageDate = _minDate.addMSecs(deltaMsecs / 2);
                #ifdef ENV_SIMULATOR
                _minDate = averageDate.addSecs(-60);
                _maxDate = averageDate.addSecs(60);
                #else
                _minDate = averageDate.addSecs(-60 * 60 * 12);
                _maxDate = averageDate.addSecs(60 * 60 * 12);
                #endif
            }
        }

        if(_maxValue - _minValue < minValueDelta)
        {
            qreal averageValue = (_maxValue + _minValue) / 2;
            _minValue = averageValue - minValueDelta / 2;
            _maxValue = averageValue + minValueDelta / 2;
        }

        // Values grids
        qreal minTick;
        qreal tickDelta;
        int precision;

        if(_maxValue - _minValue <= 5)
        {
            minTick = (((int)(_minValue * 10)) / 5 + 1) / 2.0;
            tickDelta = 0.5;
            precision = 1;
        }
        else
        {
            minTick = ((int)_minValue) + 1;
            tickDelta = 1;
            precision = 0;
        }

        for(qreal tick = minTick ; tick < _maxValue ; tick += tickDelta)
        {
            _gridValues.append(tick);
        }

        // Timestamps grids
        quint8 count = 0;
        #ifdef ENV_SIMULATOR
        int delta = 10;
        QDateTime minTickDate = _minDate.addSecs(-_minDate.time().second()).addSecs(delta);
        #else
        int delta = 60 * 60;
        QDateTime minTickDate = _minDate.addSecs(-(_minDate.time().minute() * 60 + _minDate.time().second())).addSecs(delta);
        #endif
        for(QDateTime tickDate = minTickDate ; tickDate < _maxDate ; tickDate = tickDate.addSecs(delta))
        {
            if(count % 3 == 0)
            {
                _gridDates.append(tickDate);
            }

            _minorGridDates.append(tickDate);

            count++;
        }
    }
}

void TemperatureAccessor::removeOldHistory(const QDateTime &now)
{
    QDateTime limitDate = now.addDays(-1);
    while(!_history.isEmpty() && _history.first().timestamp < limitDate)
    {
        _history.takeFirst();
    }
}
