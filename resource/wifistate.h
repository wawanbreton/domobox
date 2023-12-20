#pragma once

#include <QObject>

class WifiState : public QObject
{
    Q_OBJECT
    Q_ENUMS(Enum)

    public:
        enum Enum
        {
            Unknown,
            TurningUp,
            Up,
            TurningDown,
            Down
        };
};
