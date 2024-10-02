#ifndef MUSICPLAYER_HPP
#define MUSICPLAYER_HPP

# include <QObject>
# include <QtMultimedia/QMediaPlayer>
# include <QtMultimedia/QAudioOutput>
# include <QDebug>
# include <filesystem>
# include <iostream>
# include <vector>
class MusicPlayer : public QObject
{
    Q_OBJECT
public:
    explicit MusicPlayer(QObject *parent = nullptr);
    ~MusicPlayer();
    QMediaPlayer _Player;
    QAudioOutput* _AudioOutput;
    Q_INVOKABLE void _PlaySong();

    QStringList getTracks() const;
    void setTracks(const QStringList &newTracks);

private:
    QStringList Tracks{};



    Q_PROPERTY(QStringList _Tracks READ getTracks WRITE setTracks NOTIFY TracksChanged FINAL)

signals:
    void TracksChanged();
};

#endif // MUSICPLAYER_HPP
