#include "domostyle.h"

#include <QDebug>
#include <QPropertyAnimation>
#include <QVariantAnimation>


DomoStyle::DomoStyle(QObject *parent) :
    QObject(parent)
{
}

void DomoStyle::onDayModeChanged(DayMode::Enum mode)
{
    if(mode != _dayMode)
    {
        _dayMode = mode;
        emit changed();
    }
}

QColor DomoStyle::getForeground() const
{
    return _dayMode == DayMode::Day ? foregroundDay : foregroundNight;
}

QColor DomoStyle::getButtonBackground() const
{
    return _dayMode == DayMode::Day ? buttonBackgroundDay : buttonBackgroundNight;
}

QColor DomoStyle::getComponentBackground() const
{
    return _dayMode == DayMode::Day ? componentBackgroundDay : componentBackgroundNight;
}

QColor DomoStyle::getComponentBackgroundAlternative() const
{
    return _dayMode == DayMode::Day ? componentBackgroundAlternativeDay : componentBackgroundAlternativeNight;
}

QColor DomoStyle::getBackgroundCenter() const
{
    return _dayMode == DayMode::Day ? backgroundCenterDay : backgroundCenterNight;
}

QColor DomoStyle::getBackgroundOuter() const
{
    return _dayMode == DayMode::Day ? backgroundOuterDay : backgroundOuterNight;
}
