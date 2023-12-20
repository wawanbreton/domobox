#pragma once

#include <QObject>

class TimeAccessor : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString time MEMBER _time NOTIFY timeChanged)
    Q_PROPERTY(QString date MEMBER _date NOTIFY dateChanged)

    public:
        explicit TimeAccessor(QObject *parent = nullptr);

    signals:
        void timeChanged(const QString &time);

        void dateChanged(const QString &date);

    private:
        void updateDateTime();

    private:
        QString _time;
        QString _date;
};
