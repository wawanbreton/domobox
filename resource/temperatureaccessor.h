#pragma once

#include <QObject>

#include <QDateTime>
#include <QDebug>

#include "constants.h"

struct sensors_chip_name;

class TemperatureAccessor : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString temperatureStr READ getTemperatureStr NOTIFY temperatureChanged)
    Q_PROPERTY(QDateTime minDate READ getMinDate NOTIFY temperatureChanged)
    Q_PROPERTY(QDateTime maxDate READ getMaxDate NOTIFY temperatureChanged)
    Q_PROPERTY(qreal minValue READ getMinValue NOTIFY temperatureChanged)
    Q_PROPERTY(qreal maxValue READ getMaxValue NOTIFY temperatureChanged)
    Q_PROPERTY(QList<QDateTime> gridMinorDates READ getMinorGridDates NOTIFY temperatureChanged)
    Q_PROPERTY(QList<QDateTime> gridDates READ getGridDates NOTIFY temperatureChanged)
    Q_PROPERTY(QVariantList gridValues READ getGridValues NOTIFY temperatureChanged)
    Q_PROPERTY(QList<QDateTime> historyTimestamps READ getHistoryTimestamps NOTIFY temperatureChanged)
    Q_PROPERTY(QVariantList historyValues READ getHistoryValues NOTIFY temperatureChanged)

    public:
        explicit TemperatureAccessor(QObject *parent = nullptr);

        QString getTemperatureStr() const;

        const QDateTime &getMinDate() const { return _minDate; }

        const QDateTime &getMaxDate() const { return _maxDate; }

        const qreal &getMinValue() const { return _minValue; }

        const qreal &getMaxValue() const { return _maxValue; }

        const QList<QDateTime> &getMinorGridDates() const { return _minorGridDates; }

        const QList<QDateTime> &getGridDates() const { return _gridDates; }

        const QVariantList &getGridValues() const { return _gridValues; }

        QList<QDateTime> getHistoryTimestamps() const;

        QVariantList getHistoryValues() const;

        QVariant getLastTemperature() const;

    signals:
        void temperatureChanged(const qreal temperature);

    protected:
        virtual void timerEvent(QTimerEvent *event) override;

    private:
        void updateTemperature();

        void updateChartLimits();

        void removeOldHistory(const QDateTime &now);

    private:
        struct TemperatureValue
        {
            QDateTime timestamp;
            qreal value;
        };

    private:
        static constexpr qreal minValueDelta{3};

    private:
        const sensors_chip_name *_chipName{nullptr};
        int _featureIndex{-1};
        QList<TemperatureValue> _history;
        QDateTime _minDate;
        QDateTime _maxDate;
        qreal _minValue{0.0};
        qreal _maxValue{0.0};
        QList<QDateTime> _minorGridDates;
        QList<QDateTime> _gridDates;
        QVariantList _gridValues;
};
