#include "resourceaccessor.h"

#include "resource/audioaccessor.h"
#include "resource/fireaccessor.h"
#include "resource/gpiocontroller.h"
#include "resource/shutteraccessor.h"
#include "resource/temperatureaccessor.h"
#include "resource/timeaccessor.h"
#include "resource/weatheraccessor.h"
#include "resource/wifiaccessor.h"


ResourceAccessor *ResourceAccessor::_instance{nullptr};

ResourceAccessor::ResourceAccessor(QObject *parent) :
    QObject(parent),
    _audio(new AudioAccessor(this)),
    _time(new TimeAccessor(this)),
    _temperature(new TemperatureAccessor(this)),
    _weather(new WeatherAccessor(this)),
    _wifi(new WifiAccessor(this)),
    _fire(new FireAccessor(this)),
    _shutter(new ShutterAccessor(this))
{
    connect(_temperature, &TemperatureAccessor::temperatureChanged,
            _fire,        &FireAccessor::onNewTemperatureReceived);
    connect(GpioController::get(), &GpioController::backlightOff,
            this,                  &ResourceAccessor::backlightOff);
}

void ResourceAccessor::init(QObject *parent)
{
    _instance = new ResourceAccessor(parent);
}
