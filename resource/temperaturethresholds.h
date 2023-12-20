#pragma once

#include <QObject>

class QSettings;

class TemperatureThresholds : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qreal min READ getMin WRITE setMin NOTIFY minChanged)
    Q_PROPERTY(qreal max READ getMax WRITE setMax NOTIFY maxChanged)

    public:
        explicit TemperatureThresholds(QObject *parent = nullptr);

        void load(const QSettings &settings,
                  const QString &prefix,
                  const qreal &defaultMin,
                  const qreal &defaultMax);

        void save(QSettings &settings, const QString &prefix) const;

        void operator=(const TemperatureThresholds &other);

        const qreal &getMin() const { return _min; }

        void setMax(const qreal &max) { setMax(max, true); }

        const qreal &getMax() const { return _max; }

        void setMin(const qreal &min) { setMin(min, true); }

    signals:
        void minChanged();

        void maxChanged();

        void changed();

    private:
        void setMax(const qreal &max, const bool &adjustMin);

        void setMin(const qreal &min, const bool &adjustMax);

        static qreal load(const QSettings &settings,
                          const QString &identifier,
                          const qreal defaultValue);

    private:
        static constexpr qreal minDelta{2.0};

    private:
        qreal _min{0.0};
        qreal _max{0.0};
};
