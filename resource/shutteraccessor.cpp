#include "shutteraccessor.h"

#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>

#include "resource/shutter.h"


ShutterAccessor::ShutterAccessor(QObject *parent) :
    QObject{parent}
{
    for(auto port : QSerialPortInfo::availablePorts())
    {
        if(!_port && port.vendorIdentifier() == 0x0403 && port.productIdentifier() == 0x6015)
        {
            _port = new QSerialPort(port);
        }
    }

    if(_port)
    {
        if(_port->open(QIODevice::ReadWrite))
        {
            _port->setBaudRate(QSerialPort::Baud38400);
            _port->setDataBits(QSerialPort::Data8);
            _port->setParity(QSerialPort::NoParity);
            _port->setStopBits(QSerialPort::OneStop);
            _port->setFlowControl(QSerialPort::NoFlowControl);

            qInfo() << "RFXCom is ready";
        }
        else
        {
            qWarning() << "Error when opening RFXCom serial port :" << _port->errorString();
            delete  _port;
            _port = nullptr;
        }
    }
    else
    {
        qWarning() << "RFXCom was not found !";
    }
}

void ShutterAccessor::sendShuttersCommand(const ShutterCommand::Enum &command,
                                          const QVariantList &shutters)
{
    for(auto shutter : shutters)
    {
        sendShutterCommand(command, shutter.value<Shutter::Enum>());
    }
}

void ShutterAccessor::sendShutterCommand(const ShutterCommand::Enum &command,
                                         const Shutter::Enum &shutter)
{
    if(_port)
    {
        QByteArray data;

        // Common header
        data.append(QByteArray::fromHex("0C 1A 00 01 00 00"));

        // Specific ID
        data.append(static_cast<quint8>(shutter));

        // Unit ID
        data.append('\1');

        // Command
        data.append(static_cast<quint8>(command));

        // Common footer
        data.append(QByteArray::fromHex("00 00 00 00"));

        qDebug() << data.toHex();

        _port->write(data);
    }
}
