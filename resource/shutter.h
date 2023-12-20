#pragma once

#include <QObject>

class Shutter : public QObject
{
    Q_OBJECT
    Q_ENUMS(Enum)

    public:
        enum Enum
        {
            Stairs          = 1,
            LivingRoomLeft  = 2,
            LivingRoomRight = 3,
            Kitchen         = 4,
            RoomChild       = 5,
            RoomParent      = 6
        };
};

Q_DECLARE_METATYPE(Shutter::Enum)
