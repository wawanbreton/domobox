#include "powermanager.h"

#include <QDebug>
#include <QEvent>

#include "resource/gpiocontroller.h"


PowerManager::PowerManager(QObject *parent) :
    QObject(parent),
    _state(Awake),
    _timer(new QTimer(this))
{
    GpioController::get()->setBacklightOn(true);

    _timer->setSingleShot(true);
    _timer->setInterval(90000);
    connect(_timer, &QTimer::timeout, this, &PowerManager::onTimeout);
    _timer->start();
}

bool PowerManager::eventFilter(QObject *receiver, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress ||
       event->type() == QEvent::MouseMove ||
       event->type() == QEvent::MouseButtonRelease)
    {
        switch(_state)
        {
            case Awake:
                _timer->start();
                break;
            case Asleep:
            {
                if(event->type() == QEvent::MouseButtonPress)
                {
                    _state = WakingUp;
                    GpioController::get()->setBacklightOn(true);
                }
                return true;

            }
            case WakingUp:
            {
                if(event->type() == QEvent::MouseButtonRelease)
                {
                    _state = Awake;
                    _timer->start();
                }
                return true;
            }
        }
    }

    return QObject::eventFilter(receiver, event);
}

void PowerManager::onTimeout()
{
    if(_state == Awake)
    {
        _state = Asleep;
        GpioController::get()->setBacklightOn(false);
    }
}
