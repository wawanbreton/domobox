#pragma once

#include <QObject>

#include "resource/firemode.h"

class TemperatureConfiguration;
class TemperatureThresholds;

class FireAccessor : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool fireOn READ isFireOn WRITE setFireOn NOTIFY fireOnChanged)
    Q_PROPERTY(bool modeAuto READ isModeAuto WRITE setModeAuto NOTIFY modeChanged)
    Q_PROPERTY(FireMode::Enum mode READ getMode NOTIFY modeChanged)

    public:
        explicit FireAccessor(QObject *parent = nullptr);

        const bool &isFireOn() const;

        void setFireOn(const bool &on);

        bool isModeAuto() const;

        void setModeAuto(const bool &modeAuto);

        void onNewTemperatureReceived(const qreal &temperature);

        const FireMode::Enum &getMode() const;

        Q_INVOKABLE void startHoliday(const QDateTime &destinationDate, const FireMode::Enum &mode);

        Q_INVOKABLE void setConfiguration(TemperatureConfiguration *config);

    private:
        const TemperatureThresholds *currentThresholds() const;

        void reEvaluteLastTemperature();

        QVariant getLastTemperature() const;

    signals:
        void fireOnChanged();

        void modeChanged();
};
