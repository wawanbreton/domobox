#include "gpiocontroller.h"

#include <QDebug>
#include <QFile>
#include <QTimer>
#include <QVariant>
#include <QVariantAnimation>

#define GPIO_RELAY1 17
#define GPIO_RELAY2 27
#define GPIO_RELAY3 22
#define GPIO_RELAY4 18
#define GPIO_RELAY5 24
#define GPIO_RELAY6 23
#define GPIO_RELAY7 25
#define GPIO_RELAY8 8

#define RELAY_STOVE      GPIO_RELAY1
#define RELAY_AMPLI      GPIO_RELAY3
#define RELAY_AMPLI_MODE GPIO_RELAY8


GpioController *GpioController::_instance{nullptr};

GpioController::GpioController(QObject *parent) :
    QObject(parent)
{
    #ifdef ENV_SIMULATOR
    setFireOnImpl(_fireOn);
    #else
    _fireOn = getRelay(RELAY_STOVE);
    #endif

    setBacklightValueImpl(1.0);

    setAudioStateImpl(_audioState);

    QTimer::singleShot(5000, this, &GpioController::updateAudioVolumeImpl);
}

void GpioController::init(QObject *parent)
{
    _instance = new GpioController(parent);
}

void GpioController::setFireOn(const bool &fireOn)
{
    if(fireOn != _fireOn)
    {
        _fireOn = fireOn;
        setFireOnImpl(fireOn);
    }
}

void GpioController::setBacklightOn(const bool &lightOn)
{
    if(lightOn != _backlightOn)
    {
        _backlightOn = lightOn;

        QVariant startValue = lightOn ? 0.0 : 1.0;
        if(!_backlightAnimation.isNull())
        {
            startValue = _backlightAnimation->currentValue();
            _backlightAnimation->deleteLater();
        }

        _backlightAnimation = new QVariantAnimation(this);
        _backlightAnimation->setStartValue(startValue);
        _backlightAnimation->setEndValue(lightOn ? 1.0 : 0.0);
        _backlightAnimation->setDuration(lightOn ? 200 : 3000);
        _backlightAnimation->setEasingCurve(lightOn ? QEasingCurve::OutCubic : QEasingCurve::InQuad);
        connect(_backlightAnimation.data(), &QVariantAnimation::valueChanged,
                this,                       &GpioController::setBacklightValueImpl);
        _backlightAnimation->start(QAbstractAnimation::DeleteWhenStopped);

        if(!lightOn)
        {
            connect(_backlightAnimation, &QVariantAnimation::finished,
                    this,                &GpioController::backlightOff);
        }
    }
}

void GpioController::setAudioState(const AudioState::Enum &state)
{
    if(state != _audioState)
    {
        _audioState = state;
        setAudioStateImpl(_audioState);
    }
}

void GpioController::setVolumeBathroom(const qreal &volume)
{
    if(volume != _volumeBathroom)
    {
        _volumeBathroom = volume;
        updateAudioVolumeImpl();
    }
}

void GpioController::setVolumeLivingRoom(const qreal &volume)
{
    if(volume != _volumeLivingRoom)
    {
        _volumeLivingRoom = volume;
        updateAudioVolumeImpl();
    }
}

void GpioController::setFireOnImpl(const bool &fireOn)
{
    setRelay(RELAY_STOVE, fireOn);
}

void GpioController::setBacklightValueImpl(const QVariant &value)
{
    int percentValue = value.toReal() * 50;

    #ifdef ENV_SIMULATOR
    qDebug() << "Pseuso-setting backlight" << percentValue;
    #else
    QFile backlightFile("/sys/class/backlight/rpi_backlight/brightness");
    if(backlightFile.open(QIODevice::WriteOnly))
    {
        backlightFile.write(QString::number(percentValue).toUtf8());
    }
    else
    {
        qDebug() << "Unable to open backlight file (write)";
    }
    #endif
}

void GpioController::updateAudioVolumeImpl()
{
    #ifdef ENV_SIMULATOR
    qDebug() << "Pseuso-setting volume" << _volumeBathroom << _volumeLivingRoom;
    #else
    if(!_alsaHandle)
    {
        snd_mixer_t *handle;
        snd_mixer_selem_id_t *sid;
        const char *card = "default";
        const char *selem_name = "Master";

        snd_mixer_open(&handle, 0);
        snd_mixer_attach(handle, card);
        snd_mixer_selem_register(handle, NULL, NULL);
        snd_mixer_load(handle);

        snd_mixer_selem_id_alloca(&sid);
        snd_mixer_selem_id_set_index(sid, 0);
        snd_mixer_selem_id_set_name(sid, selem_name);
        _alsaHandle = snd_mixer_find_selem(handle, sid);

        snd_mixer_selem_get_playback_volume_range(_alsaHandle, &_volumeRangeMin, &_volumeRangeMax);
    }

    if(_alsaHandle && _volumeRangeMin >= 0 && _volumeRangeMax > _volumeRangeMin)
    {
        long volumeLivingRoom = _volumeLivingRoom * (_volumeRangeMax - _volumeRangeMin) + _volumeRangeMin;
        snd_mixer_selem_set_playback_volume(_alsaHandle, SND_MIXER_SCHN_FRONT_LEFT, volumeLivingRoom);

        long volumeBathroom = _volumeBathroom * (_volumeRangeMax - _volumeRangeMin) + _volumeRangeMin;
        snd_mixer_selem_set_playback_volume(_alsaHandle, SND_MIXER_SCHN_FRONT_RIGHT, volumeBathroom);
    }
    #endif
}

void GpioController::setAudioStateImpl(AudioState::Enum mode)
{
    setRelay(RELAY_AMPLI,      mode != AudioState::None);
    setRelay(RELAY_AMPLI_MODE, mode == AudioState::Bluetooth);
}

void GpioController::setRelay(int gpio, bool on)
{
    #ifdef ENV_SIMULATOR
    qDebug() << "Pseuso-setting relay" << gpio << on;
    #else
    QFile gpioFile(QString("/sys/class/gpio/gpio%1/value").arg(gpio));
    if(gpioFile.open(QIODevice::WriteOnly))
    {
        gpioFile.write(on ? "0" : "1");
    }
    else
    {
        qDebug() << "Unable to open GPIO file (write) for" << gpio;
    }
    #endif
}

bool GpioController::getRelay(int gpio)
{
    #ifndef ENV_SIMULATOR
    QFile gpioFile(QString("/sys/class/gpio/gpio%1/value").arg(gpio));
    if(gpioFile.open(QIODevice::ReadOnly))
    {
        return gpioFile.readAll().mid(0, 1) != "1";
    }
    else
    {
        qDebug() << "Unable to open GPIO file (read) for" << gpio;
    }
    #endif

    return false;
}
