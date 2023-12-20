#pragma once

#include <QObject>

#include <QQmlListProperty>

class QSettings;

class DailyHotPeriods;
class TemperatureThresholds;

class TemperatureConfiguration : public QObject
{
    Q_OBJECT
    Q_PROPERTY(TemperatureThresholds *thresholdsHot READ getThresholdsHot
                                                    WRITE setThresholdsHot
                                                    NOTIFY dummyChanged)
    Q_PROPERTY(TemperatureThresholds *thresholdsCold READ getThresholdsCold
                                                     WRITE setThresholdsCold
                                                     NOTIFY dummyChanged)
    Q_PROPERTY(QQmlListProperty<DailyHotPeriods> hotPeriods READ getHotPeriods NOTIFY dummyChanged)
    
    public:
        explicit TemperatureConfiguration(QObject *parent = nullptr);

        explicit TemperatureConfiguration(const TemperatureConfiguration &other);

        void operator=(const TemperatureConfiguration &other);

        void load(const QSettings &settings, const QString &prefix);

        void save(QSettings &settings, const QString &prefix) const;

        TemperatureThresholds *getThresholdsHot() const { return _thresholdsHot; }

        void setThresholdsHot(const TemperatureThresholds *thresholds);

        TemperatureThresholds *getThresholdsCold() const { return _thresholdsCold; }

        void setThresholdsCold(const TemperatureThresholds *thresholds);

        QQmlListProperty<DailyHotPeriods> getHotPeriods();

        const QList<DailyHotPeriods *> &getHotPeriodsRaw() const { return _hotPeriods; }

        Q_INVOKABLE void loadFromConfig();
    
    signals:
        void dummyChanged();

        void changed();
        
    private:
        TemperatureThresholds *_thresholdsHot{nullptr};
        TemperatureThresholds *_thresholdsCold{nullptr};
        QList<DailyHotPeriods *> _hotPeriods;
};
