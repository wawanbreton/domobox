#include "dailyhotperiods.h"

#include "resource/hotperiod.h"


DailyHotPeriods::DailyHotPeriods(QObject *parent) :
    QObject(parent),
    _period1(new HotPeriod(this)),
    _period2(new HotPeriod(this))
{
    _period1->setActive(true);

    connect(_period1, &HotPeriod::changed, this, &DailyHotPeriods::changed);
    connect(_period2, &HotPeriod::changed, this, &DailyHotPeriods::changed);
}

void DailyHotPeriods::load(const QSettings &settings,  const QString &prefix)
{
    _period1->load(settings, QString("%1/period1").arg(prefix), true, QTime(6, 0), QTime(9, 0));
    _period2->load(settings, QString("%1/period2").arg(prefix), true, QTime(17, 0), QTime(22, 0));
}

void DailyHotPeriods::save(QSettings &settings, const QString &prefix) const
{
    _period1->save(settings, QString("%1/period1").arg(prefix));
    _period2->save(settings, QString("%1/period2").arg(prefix));
}

QList<HotPeriod *> DailyHotPeriods::getActivePeriods() const
{
    QList<HotPeriod *> result;

    for(HotPeriod *period : {_period1, _period2})
    {
        if(period->isActive())
        {
            result.append(period);
        }
    }

    return result;
}

void DailyHotPeriods::operator=(const DailyHotPeriods &other)
{
    (*_period1) = (*other._period1);
    (*_period2) = (*other._period2);
}
