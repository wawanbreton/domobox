#include "timeaccessor.h"

#include <QDateTime>
#include <QDebug>
#include <QTimer>
#include <QVariant>


TimeAccessor::TimeAccessor(QObject *parent) : QObject(parent)
{
    updateDateTime();
}

void TimeAccessor::updateDateTime()
{
    QDateTime now = QDateTime::currentDateTime();

    setProperty("time", now.toString("HH:mm"));

    QString dateStr = now.toString("dddd dd MMMM");
    dateStr = dateStr[0].toUpper() + dateStr.mid(1);
    setProperty("date", dateStr);

    QDateTime nextTrigger = now;
    QTime nextTime = QTime(now.time().hour(), now.time().minute(), 0);
    nextTrigger.setTime(nextTime.addMSecs(60001));

    QTimer::singleShot(qMax((qint64)0, now.msecsTo(nextTrigger)), this, &TimeAccessor::updateDateTime);
}
