#pragma once

#include <QObject>

#include <QTimer>

class PowerManager : public QObject
{
    Q_OBJECT

    public:
        explicit PowerManager(QObject *parent=NULL);

        virtual bool eventFilter(QObject *receiver, QEvent *event);

    private:
        typedef enum
        {
            Asleep,
            WakingUp,
            Awake
        } State;

    private:
        void onTimeout();

    private:
        State _state{Awake};
        QTimer *_timer{nullptr};
};
