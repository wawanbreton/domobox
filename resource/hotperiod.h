#pragma once

#include <QObject>

#include <QTime>

class QSettings;

class HotPeriod : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool active READ isActive WRITE setActive NOTIFY activeChanged)
    Q_PROPERTY(QTime begin READ getBegin WRITE setBegin NOTIFY beginChanged)
    Q_PROPERTY(QTime end READ getEnd WRITE setEnd NOTIFY endChanged)

    public:
        explicit HotPeriod(QObject *parent = nullptr);

        void load(const QSettings &settings,
                  const QString &prefix,
                  const bool &defaultActive,
                  const QTime &defaultBegin,
                  const QTime &defaultEnd);

        void save(QSettings &settings, const QString &prefix) const;

        void operator=(const HotPeriod &other);

    signals:
        void activeChanged();

        void beginChanged();

        void endChanged();

        void changed();

    public:
        const bool &isActive() const { return _active; }

        void setActive(const bool &active);

        const QTime &getBegin() const { return _begin; }

        void setBegin(const QTime &begin);

        const QTime &getEnd() const { return _end; }

        void setEnd(const QTime &end);

        qreal lengthHours() const
        { return _begin.secsTo(_end) / 3600.0; }

        bool contains(const QTime &time) const
        { return time >= _begin && time <= _end; }

    private:
        static QTime load(const QSettings &settings,
                          const QString &identifier,
                          const QTime &defaultTime);

    private:
        bool _active{false};
        QTime _begin{6, 0};
        QTime _end{22, 0};
};
