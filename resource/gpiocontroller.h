#pragma once

#include <QObject>

#include <alsa/asoundlib.h>

#include <QPointer>

#include "resource/audiostate.h"

class QVariantAnimation;

class GpioController : public QObject
{
    Q_OBJECT

    public:
        void setFireOn(const bool &fireOn);

        const bool &getFireOn() const { return _fireOn; }

        void setBacklightOn(const bool &lightOn);

        const bool &getBacklightOn() const { return _backlightOn; }

        const AudioState::Enum &getAudioState() const { return _audioState; }

        void setAudioState(const AudioState::Enum &mode);

        const qreal &getVolumeBathroom() const { return _volumeBathroom; }

        void setVolumeBathroom(const qreal &volume);

        const qreal &getVolumeLivingRoom() const { return _volumeLivingRoom; }

        void setVolumeLivingRoom(const qreal &volume);

        static void init(QObject *parent);

        static GpioController *get() { return _instance; }

    signals:
        void backlightOff();

    private:
        explicit GpioController(QObject *parent=NULL);

        void setBacklightValueImpl(const QVariant &value);

        void updateAudioVolumeImpl();

        void setFireOnImpl(const bool &fireOn);

        void setAudioStateImpl(AudioState::Enum state);

        void setAudioVolumeImpl(int volume);

        static void setRelay(int gpio, bool on);

        static bool getRelay(int gpio);

    private:
        static GpioController *_instance;

    private:
        bool _fireOn{false};
        bool _backlightOn{false};
        AudioState::Enum _audioState{AudioState::None};
        qreal _volumeBathroom{0.33};
        qreal _volumeLivingRoom{0.33};
        QPointer<QVariantAnimation> _backlightAnimation;
        snd_mixer_elem_t *_alsaHandle{nullptr};
        long _volumeRangeMin{-1};
        long _volumeRangeMax{-1};
};

