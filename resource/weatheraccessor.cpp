#include "weatheraccessor.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>

#include "constants.h"
#include "resource/forecastdata.h"


const QString WeatherAccessor::noIcon{"/assets/error.svg"};

WeatherAccessor::WeatherAccessor(QObject *parent) :
    QObject(parent),
    _network(new QNetworkAccessManager(this)),
    _timerUpdateDayMode(new QTimer(this))
{
    connect(_network, &QNetworkAccessManager::finished, this, &WeatherAccessor::onRequestFinished);

    for(int i = 0 ; i < 24 ; ++i)
    {
        _forecast.append(new ForecastData(this));
    }

    onRequestFinished(NULL);

    updateWeather();

    startTimer(5 * 60 *1000);

    _timerUpdateDayMode->setSingleShot(true);
    connect(_timerUpdateDayMode, &QTimer::timeout, this, &WeatherAccessor::updateDayMode);
}

void WeatherAccessor::timerEvent(QTimerEvent *event)
{
    updateWeather();

    QObject::timerEvent(event);
}

QQmlListProperty<ForecastData> WeatherAccessor::getForecast()
{
    return QQmlListProperty<ForecastData>(this, &_forecast);
}

void WeatherAccessor::updateWeather()
{
    QUrl url("https://api.openweathermap.org/data/2.5/onecall?"
             #warning set your GPS location
             "lat=47.89304&"
             "lon=-1.61049&"
             "units=metric&"
             #warning set your Openweathermap APPID here
             "APPID="
             "exclude=minutely,daily,alerts");
    _network->get(QNetworkRequest(url));
}

void WeatherAccessor::updateDayMode()
{
    _timerUpdateDayMode->stop();

    DayMode::Enum newDayMode = DayMode::Unknown;
    if(_sunrise.isValid() && _sunset.isValid())
    {
        QDateTime now = QDateTime::currentDateTimeUtc();
        newDayMode = (now <= _sunrise || now >= _sunset) ? DayMode::Night : DayMode::Day;

        if(now < _sunrise)
        {
            _timerUpdateDayMode->start(now.msecsTo(_sunrise) + 1);
        }
        else if(now < _sunset)
        {
            _timerUpdateDayMode->start(now.msecsTo(_sunset) + 1);
        }
        else
        {
            // Way for next day data !
        }
    }

    if(newDayMode != _dayMode)
    {
        _dayMode = newDayMode;
        emit dayModeChanged(_dayMode);
    }
}

void WeatherAccessor::onRequestFinished(QNetworkReply *reply)
{
    _temperature = Constants::empty;
    _icon = noIcon;

    if(reply)
    {
        reply->deleteLater();

        if(reply->error() == QNetworkReply::NoError)
        {
            _delayRetry = 5000;

            QJsonParseError error;
            QJsonDocument doc = QJsonDocument::fromJson(reply->readAll(), &error);
            if(error.error == QJsonParseError::NoError)
            {
                QJsonValue valueCurrent = doc["current"];

                _temperature = QString("%1 °C").arg(qRound(valueCurrent["temp"].toDouble()));
                _sunrise = QDateTime::fromSecsSinceEpoch(valueCurrent["sunrise"].toInt(), Qt::UTC);
                _sunset = QDateTime::fromSecsSinceEpoch(valueCurrent["sunset"].toInt(), Qt::UTC);

                _icon = getIcon(valueCurrent);

                qint64 now = QDateTime::currentDateTimeUtc().toSecsSinceEpoch();
                QJsonArray hourlyForecast = doc["hourly"].toArray();

                int index = 0;
                for(const QJsonValue &forecast : hourlyForecast)
                {
                    double timestamp = forecast["dt"].toDouble();
                    if(timestamp > now && index < _forecast.count())
                    {
                        ForecastData *data = _forecast.at(index++);
                        data->set(timestamp, forecast["temp"].toDouble(), getIcon(forecast));
                    }
                }

                updateDayMode();
            }
            else
            {
                qWarning() << "JSON parsing error :" << error.errorString();
            }
        }
        else
        {
            qWarning() << "MeteoWidget::onRequestFinished"
                       << "Network error on receiving meteo data :" << reply->errorString();
            QTimer::singleShot(_delayRetry, this, SLOT(updateMeteo()));
            _delayRetry *= 2;
        }
    }
    else
    {
        for(int i = 0 ; i < 24 ; ++i)
        {
            _forecast.at(i)->set(0.0, 0.0, QString());
        }
    }

    emit dataChanged();
}

QString WeatherAccessor::getIcon(const QJsonValue &value)
{
    QString icon = value["weather"].toArray().first()["icon"].toString();
    return QString("http://openweathermap.org/img/wn/%1@2x.png").arg(icon);
}
