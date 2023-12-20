#pragma once

#include <QObject>

class AudioAccessor;
class FireAccessor;
class ShutterAccessor;
class TimeAccessor;
class TemperatureAccessor;
class WeatherAccessor;
class WifiAccessor;

class ResourceAccessor : public QObject
{
    Q_OBJECT
    Q_PROPERTY(AudioAccessor* audio MEMBER _audio CONSTANT)
    Q_PROPERTY(TimeAccessor* time MEMBER _time CONSTANT)
    Q_PROPERTY(TemperatureAccessor* temperature MEMBER _temperature CONSTANT)
    Q_PROPERTY(WeatherAccessor* weather READ getWeather CONSTANT)
    Q_PROPERTY(WifiAccessor* wifi MEMBER _wifi CONSTANT)
    Q_PROPERTY(FireAccessor* fire MEMBER _fire CONSTANT)
    Q_PROPERTY(ShutterAccessor* shutter MEMBER _shutter CONSTANT)

    public:
        static void init(QObject *parent);

        static ResourceAccessor *get() { return _instance; }

        WeatherAccessor *getWeather() const { return _weather; }

    signals:
        void backlightOff();

    private:
        explicit ResourceAccessor(QObject *parent = nullptr);

    private:
        static ResourceAccessor *_instance;

    private:
        AudioAccessor *const _audio;
        TimeAccessor *const _time;
        TemperatureAccessor *const _temperature;
        WeatherAccessor *const _weather;
        WifiAccessor *const _wifi;
        FireAccessor *const _fire;
        ShutterAccessor *const _shutter;
};
