#pragma once

#include <QObject>

class FireMode : public QObject
{
    Q_OBJECT
    Q_ENUMS(Enum)

    public:
        enum Enum
        {
            Automatic,
            LongHoliday,  /* Stove is off all the time */
            ShortHoliday, /* Temperature is kept with the cold thresholds */
            Manual
        };

        inline static Enum parse(const QString &string)
        {
            if(string == "Automatic")
            {
                return Automatic;
            }
            else if(string == "LongHoliday")
            {
                return LongHoliday;
            }
            else if(string == "ShortHoliday")
            {
                return ShortHoliday;
            }
            else
            {
                return Manual;
            }
        }

        inline static QString toString(Enum value)
        {
            switch(value)
            {
                case Automatic:
                    return "Automatic";
                case LongHoliday:
                    return "LongHoliday";
                case ShortHoliday:
                    return "ShortHoliday";
                default:
                    return "Manual";
            }
        }
};

Q_DECLARE_METATYPE(FireMode::Enum)
