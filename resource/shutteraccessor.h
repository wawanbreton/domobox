#pragma once

#include <QObject>

#include "resource/shutter.h"
#include "resource/shuttercommand.h"

class QSerialPort;

class ShutterAccessor : public QObject
{
    Q_OBJECT

    public:
        explicit ShutterAccessor(QObject *parent = nullptr);

        Q_INVOKABLE void sendShuttersCommand(const ShutterCommand::Enum &command,
                                             const QVariantList &shutters);

        void sendShutterCommand(const ShutterCommand::Enum &command,
                                const Shutter::Enum &shutter);

    private:
        QSerialPort *_port{nullptr};
};
