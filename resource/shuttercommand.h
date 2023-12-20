#pragma once

#include <QObject>

class ShutterCommand : public QObject
{
    Q_OBJECT
    Q_ENUMS(Enum)

    public:
        enum Enum
        {
            Stop = 0,
            Up   = 1,
            Down = 3,
        };
};

Q_DECLARE_METATYPE(ShutterCommand::Enum)
