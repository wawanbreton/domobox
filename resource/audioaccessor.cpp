#include "audioaccessor.h"

#include <QDebug>
#include <QMediaPlayer>

#include <QAudioDeviceInfo>

#include "resource/gpiocontroller.h"
#include "resource/playlist.h"


AudioAccessor::AudioAccessor(QObject *parent) :
    QObject(parent),
    _music(new QMediaPlayer(this)),
    _playlist(new Playlist(this))
{
    connect(_playlist, &Playlist::hasPreviousChanged, this, &AudioAccessor::hasPreviousChanged);
    connect(_music, &QMediaPlayer::mediaStatusChanged, this, &AudioAccessor::onMediaStatusChanged);
}

AudioState::Enum AudioAccessor::getState() const
{
    return GpioController::get()->getAudioState();
}

QString AudioAccessor::getArtist() const
{
    return _song.artist;
}

QString AudioAccessor::getTitle() const
{
    return _song.title;
}

bool AudioAccessor::getHasPrevious() const
{
    return _playlist->hasPrevious();
}

qreal AudioAccessor::getVolumeBathroom() const
{
    return GpioController::get()->getVolumeBathroom();
}

void AudioAccessor::setVolumeBathroom(const qreal &volume)
{
    if(volume != getVolumeBathroom())
    {
        GpioController::get()->setVolumeBathroom(volume);
        emit volumeBathroomChanged();
    }
}

qreal AudioAccessor::getVolumeLivingRoom() const
{
    return GpioController::get()->getVolumeLivingRoom();
}

void AudioAccessor::setVolumeLivingRoom(const qreal &volume)
{
    if(volume != getVolumeLivingRoom())
    {
        GpioController::get()->setVolumeLivingRoom(volume);
        emit volumeLivingRoomChanged();
    }
}

void AudioAccessor::mainButtonClicked()
{
    switch(getState())
    {
        case AudioState::None:
            if(playNext())
            {
                setState(AudioState::Music);
            }
            break;

        case AudioState::Music:
            stopMusic();
            setState(AudioState::None);
            break;

        case AudioState::Bluetooth:
            qCritical() << "This is not supposed to happen";
            break;
    }
}

void AudioAccessor::nextButtonClicked()
{
    if(getState() == AudioState::Music)
    {
        playNext();
    }
}

void AudioAccessor::previousButtonClicked()
{
    if(getState() == AudioState::Music)
    {
        Song song = _playlist->playPrevious();
        if(!song.path.isEmpty())
        {
            playSong(song);
        }
    }
}

void AudioAccessor::bluetoothButtonToggled(const bool &checked)
{
    stopMusic();
    setState(checked ? AudioState::Bluetooth : AudioState::None);
}

bool AudioAccessor::playNext()
{
    Song song = _playlist->playNext();
    if(!song.path.isEmpty())
    {
        playSong(song);
        return true;
    }
    else
    {
        return false;
    }
}

void AudioAccessor::playSong(const Song &song)
{
    _song = song;
    _music->stop();

    if(!song.path.isEmpty())
    {
        _music->setMedia(QUrl::fromLocalFile(song.path));
        _music->play();
    }

    emit songChanged();
}

void AudioAccessor::setState(const AudioState::Enum &state)
{
    if(state != getState())
    {
        GpioController::get()->setAudioState(state);
        emit stateChanged();
    }
}

void AudioAccessor::stopMusic()
{
    _music->stop();
    _playlist->clearHistory();
    _song = Song();
    emit songChanged();
}

void AudioAccessor::onMediaStatusChanged(const QMediaPlayer::MediaStatus &status)
{
    if(status == QMediaPlayer::EndOfMedia)
    {
        playNext();
    }
}
