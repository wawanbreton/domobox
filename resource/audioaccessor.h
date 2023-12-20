#pragma once

#include <QObject>

#include <QMediaPlayer>

#include "resource/audiostate.h"
#include "resource/song.h"

class Playlist;

class AudioAccessor : public QObject
{
    Q_OBJECT
    Q_PROPERTY(AudioState::Enum state READ getState NOTIFY stateChanged)
    Q_PROPERTY(QString artist READ getArtist NOTIFY songChanged)
    Q_PROPERTY(QString title READ getTitle NOTIFY songChanged)
    Q_PROPERTY(bool hasPrevious READ getHasPrevious NOTIFY hasPreviousChanged)
    Q_PROPERTY(qreal volumeBathroom READ getVolumeBathroom
                                    WRITE setVolumeBathroom
                                    NOTIFY volumeBathroomChanged)
    Q_PROPERTY(qreal volumeLivingRoom READ getVolumeLivingRoom
                                      WRITE setVolumeLivingRoom
                                      NOTIFY volumeLivingRoomChanged)

    public:
        explicit AudioAccessor(QObject *parent = nullptr);

        AudioState::Enum getState() const;

        QString getArtist() const;

        QString getTitle() const;

        bool getHasPrevious() const;

        qreal getVolumeBathroom() const;

        void setVolumeBathroom(const qreal &volume);

        qreal getVolumeLivingRoom() const;

        void setVolumeLivingRoom(const qreal &volume);

        Q_INVOKABLE void mainButtonClicked();

        Q_INVOKABLE void nextButtonClicked();

        Q_INVOKABLE void previousButtonClicked();

        Q_INVOKABLE void bluetoothButtonToggled(const bool &checked);

    signals:
        void stateChanged();

        void songChanged();

        void hasPreviousChanged();

        void volumeBathroomChanged();

        void volumeLivingRoomChanged();

    private:
        bool playNext();

        void playSong(const Song &song);

        void setState(const AudioState::Enum &state);

        void stopMusic();

        void onMediaStatusChanged(const QMediaPlayer::MediaStatus &status);

    private:
        QMediaPlayer *_music{nullptr};
        Song _song;
        Playlist * _playlist{nullptr};
};
