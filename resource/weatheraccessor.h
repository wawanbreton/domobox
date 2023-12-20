#pragma once

#include <QObject>

#include <QDateTime>
#include <QQmlListProperty>

#include "constants.h"
#include "resource/daymode.h"

class QNetworkAccessManager;
class QNetworkReply;
class QTimer;

class ForecastData;

class WeatherAccessor : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString temperature MEMBER _temperature NOTIFY dataChanged)
    Q_PROPERTY(QString icon MEMBER _icon NOTIFY dataChanged)
    Q_PROPERTY(QQmlListProperty<ForecastData> forecast READ getForecast NOTIFY dataChanged)

    public:
        explicit WeatherAccessor(QObject *parent = nullptr);

        QQmlListProperty<ForecastData> getForecast();

    signals:
        void dataChanged();

        void dayModeChanged(DayMode::Enum mode);

    protected:
        virtual void timerEvent(QTimerEvent *event) override;

    private:
        void updateWeather();

        void updateDayMode();

        void onRequestFinished(QNetworkReply *reply);

        static QString getIcon(const QJsonValue &value);

    private:
        static const QString noIcon;

    private:
        QString _temperature{Constants::empty};
        QString _icon{noIcon};
        QDateTime _lastGetSuccess;
        QList<ForecastData *> _forecast;
        QNetworkAccessManager *_network{nullptr};
        int _delayRetry{5000};
        DayMode::Enum _dayMode{DayMode::Unknown};
        QDateTime _sunrise;
        QDateTime _sunset;
        QTimer *_timerUpdateDayMode{nullptr};
};


Q_DECLARE_METATYPE(QQmlListProperty<ForecastData>);
