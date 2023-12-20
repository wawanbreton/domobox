#include "hotperiod.h"

#include <QSettings>


HotPeriod::HotPeriod(QObject *parent) :
    QObject(parent)
{
    connect(this, &HotPeriod::activeChanged, this, &HotPeriod::changed);
    connect(this, &HotPeriod::beginChanged, this,  &HotPeriod::changed);
    connect(this, &HotPeriod::endChanged, this,    &HotPeriod::changed);
}

void HotPeriod::load(const QSettings &settings,
                     const QString &prefix,
                     const bool &defaultActive,
                     const QTime &defaultBegin,
                     const QTime &defaultEnd)
{
    _active = settings.value(QString("%1/active").arg(prefix), defaultActive).toBool();
    _begin = load(settings, QString("%1/begin").arg(prefix), defaultBegin);
    _end = load(settings, QString("%1/end").arg(prefix), defaultEnd);
}

void HotPeriod::save(QSettings &settings, const QString &prefix) const
{
    settings.setValue(QString("%1/active").arg(prefix), _active);
    settings.setValue(QString("%1/begin").arg(prefix), _begin.toString("HH:mm"));
    settings.setValue(QString("%1/end").arg(prefix), _end.toString("HH:mm"));
}

void HotPeriod::operator=(const HotPeriod &other)
{
    setActive(other._active);
    setBegin(other._begin);
    setEnd(other._end);
}

void HotPeriod::setActive(const bool &active)
{
    if(active != _active)
    {
        _active = active;
        emit activeChanged();
    }
}

void HotPeriod::setBegin(const QTime &begin)
{
    if(begin != _begin)
    {
        _begin = begin;
        emit beginChanged();
    }
}

void HotPeriod::setEnd(const QTime &end)
{
    if(end != _end)
    {
        _end = end;
        emit endChanged();
    }
}

QTime HotPeriod::load(const QSettings &settings,
                      const QString &identifier,
                      const QTime &defaultTime)
{
    QString valueStr = settings.value(identifier).toString();
    QTime value = QTime::fromString(valueStr, "HH:mm");
    return value.isValid() ? value : defaultTime;
}
