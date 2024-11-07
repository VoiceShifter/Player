#ifndef MUSICPLAYER_HPP
#define MUSICPLAYER_HPP

# include <QObject>
# include <QtMultimedia/QMediaPlayer>
# include <QtMultimedia/QAudioOutput>
# include <QDebug>
# include <filesystem>
# include <iostream>
# include <vector>
# include <thread>
# include <mutex>
# include <chrono>
enum class States
{
    SongNotStarted,
    SongPaused,
    SongPlaying
};

class MusicPlayer : public QObject
{
    Q_OBJECT
public:
    explicit MusicPlayer(QObject *parent = nullptr);
    ~MusicPlayer();
    QMediaPlayer _Player;
    QAudioOutput* _AudioOutput;
    Q_INVOKABLE void _Play_n_Stop();
    Q_INVOKABLE void _SetVolume(float);
    Q_INVOKABLE void _SetProgress(float);
    Q_INVOKABLE void _ChooseTrack(int ID);
    QStringList getTracks() const;    
    void setTracks(const QStringList &newTracks);
    double getSliderPosition() const;
    void setSliderPosition(double newSliderPosition);

    double getVolume() const;
    void setVolume(double newVolume);

    QString getCurrentIconPath() const;

private:
    QStringList Tracks{};
    unsigned int State{};
    States fState{States::SongNotStarted};
    qint64 _SongTime{};
    double SliderPosition{};
    double Volume{};
    std::thread SliderChecker;
    void SetNewSlider();
    std::mutex fSliderMutex;
    QString CurrentIconPath;



    Q_PROPERTY(QStringList _Tracks READ getTracks WRITE setTracks NOTIFY TracksChanged FINAL)
    Q_PROPERTY(double _SliderPosition READ getSliderPosition WRITE setSliderPosition NOTIFY SliderPositionChanged FINAL)
    Q_PROPERTY(double _Volume READ getVolume WRITE setVolume NOTIFY VolumeChanged FINAL)
    Q_PROPERTY(QString _CurrentIconPath READ getCurrentIconPath NOTIFY IconPathChanged FINAL)

signals:
    void TracksChanged();
    void SliderPositionChanged();
    void VolumeChanged();
    void IconPathChanged();
};

#endif // MUSICPLAYER_HPP
