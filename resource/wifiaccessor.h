#pragma once

#include <QObject>

#include "resource/wifistate.h"

class QTimer;

class WifiAccessor : public QObject
{
    Q_OBJECT
    Q_PROPERTY(WifiState::Enum state READ getState NOTIFY stateChanged);
    Q_PROPERTY(QString remainingTimeStr READ getRemainingTimeStr NOTIFY remainingTimeChanged);
    Q_PROPERTY(QString endTimeStr READ getEndTimeStr NOTIFY endTimeChanged);

    public:
        explicit WifiAccessor(QObject *parent = nullptr);

        const WifiState::Enum &getState() const { return _state; }

        QString getRemainingTimeStr() const;

        QString getEndTimeStr() const;

        Q_INVOKABLE void addTime(const int &minutes);

        Q_INVOKABLE void deactivate();

    signals:
        void stateChanged();

        void remainingTimeChanged();

        void endTimeChanged();

    private:
        void setWifiOn(const bool &on, const int &durationMinutes = 0);

        void setWifiOnImpl(const bool &on);

        void onSetWifiOver();

        void setState(const WifiState::Enum &state);

        void onTimeChanged();

        void onRemainingTimeChanged();

        static int minutesToDelay(const int &minutes);

    private:
        WifiState::Enum _state{WifiState::Unknown};
        QTimer *_timerDisable{nullptr};
        QTimer *_timerUpdateRemainingTime{nullptr};
};
