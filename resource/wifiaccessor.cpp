#include "wifiaccessor.h"

#include <QCoreApplication>
#include <QDateTime>
#include <QDebug>
#include <QProcess>
#include <QTimer>

#include "constants.h"


WifiAccessor::WifiAccessor(QObject *parent) :
    QObject(parent),
    _timerDisable(new QTimer(this)),
    _timerUpdateRemainingTime(new QTimer(this))
{
    setWifiOn(false);

    _timerDisable->setSingleShot(true);
    connect(_timerDisable, &QTimer::timeout, this, &WifiAccessor::deactivate);

    _timerUpdateRemainingTime->setSingleShot(true);
    connect(_timerUpdateRemainingTime, &QTimer::timeout,
            this,                      &WifiAccessor::onRemainingTimeChanged);
}

QString WifiAccessor::getRemainingTimeStr() const
{
    if(_timerDisable->isActive())
    {
        int remaining = _timerDisable->remainingTime();
        remaining /= 1000;
        remaining += 60;
        int hours = remaining / 3600;
        int minutes = (remaining % 3600) / 60;

        QString minutesStr = QString::number(minutes).rightJustified(2, '0');

        return QString("%1h%2").arg(hours).arg(minutesStr);
    }
    else
    {
        return Constants::empty;
    }
}

QString WifiAccessor::getEndTimeStr() const
{
    if(_timerDisable->isActive())
    {
        QDateTime now = QDateTime::currentDateTimeUtc();
        now = now.addMSecs(_timerDisable->remainingTime());
        return now.toLocalTime().toString("hh:mm");
    }
    else
    {
        return Constants::empty;
    }
}

void WifiAccessor::addTime(const int &minutes)
{
    if(_state == WifiState::Up || _state == WifiState::TurningUp)
    {
        int delay = _timerDisable->remainingTime();
        delay += minutesToDelay(minutes);
        _timerDisable->start(delay);
        onTimeChanged();
    }
    else if(_state == WifiState::Down)
    {
        setWifiOn(true, minutes);
    }
}

void WifiAccessor::deactivate()
{
    if(_state == WifiState::Up)
    {
        setWifiOn(false);
    }
}

void WifiAccessor::setWifiOn(const bool &on, const int &durationMinutes)
{
    if(on && _state != WifiState::Up)
    {
        setWifiOnImpl(true);
        _timerDisable->start(minutesToDelay(durationMinutes));
        onTimeChanged();
    }
    else if(!on && _state != WifiState::Down)
    {
        setWifiOnImpl(false);
        _timerDisable->stop();
        onTimeChanged();
    }
}

void WifiAccessor::setWifiOnImpl(const bool &on)
{
    setState(on ? WifiState::TurningUp : WifiState::TurningDown);

    #ifdef ENV_SIMULATOR
    qInfo() << "Pseudo-setting wifi" << on;
    QTimer::singleShot(2000, this, &WifiAccessor::onSetWifiOver);
    #else
    QProcess *process = new QProcess();
    connect(process, qOverload<int, QProcess::ExitStatus>(&QProcess::finished),
            this,    &WifiAccessor::onSetWifiOver);
    connect(process, qOverload<int, QProcess::ExitStatus>(&QProcess::finished),
            process, &QProcess::deleteLater);
    process->setWorkingDirectory(QCoreApplication::applicationDirPath());
    process->start("/bin/sh", QStringList() << (on ? "setwifion.sh" : "setwifioff.sh"));
    #endif
}

void WifiAccessor::onSetWifiOver()
{
    if(_state == WifiState::TurningDown)
    {
        setState(WifiState::Down);
    }
    else if(_state == WifiState::TurningUp)
    {
        setState(WifiState::Up);
    }
}

void WifiAccessor::setState(const WifiState::Enum &state)
{
    if(state != _state)
    {
        _state = state;
        emit stateChanged();
    }
}

void WifiAccessor::onTimeChanged()
{
    emit endTimeChanged();

    onRemainingTimeChanged();
}

void WifiAccessor::onRemainingTimeChanged()
{
    emit remainingTimeChanged();

    _timerUpdateRemainingTime->stop();

    if(_timerDisable->isActive())
    {
        _timerUpdateRemainingTime->start(_timerDisable->remainingTime() % 60000 + 1);
    }
}

int WifiAccessor::minutesToDelay(const int &minutes)
{
    #ifdef ENV_SIMULATOR
    return minutes * 1000;
    #else
    return minutes * 60 * 1000;
    #endif
}
