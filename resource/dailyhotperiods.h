#pragma once

#include <QObject>

class QSettings;

class HotPeriod;

class DailyHotPeriods : public QObject
{
    Q_OBJECT
    Q_PROPERTY(HotPeriod *period1 READ getPeriod1 NOTIFY dummyChanged)
    Q_PROPERTY(HotPeriod *period2 READ getPeriod2 NOTIFY dummyChanged)

    public:
        explicit DailyHotPeriods(QObject *parent = nullptr);

        void load(const QSettings &settings, const QString &prefix);

        void save(QSettings &settings, const QString &prefix) const;

        HotPeriod *getPeriod1() { return _period1; }

        HotPeriod *getPeriod2() { return _period2; }

        QList<HotPeriod *> getActivePeriods() const;

        void operator=(const DailyHotPeriods &other);

    signals:
        void dummyChanged();

        void changed();

    private:
        HotPeriod *_period1{nullptr};
        HotPeriod *_period2{nullptr};
};

