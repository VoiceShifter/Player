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

private:
    QStringList Tracks{};
    unsigned int State{};
    qint64 _SongTime{};
    double SliderPosition{};
    double Volume{};
    std::thread SliderChecker;
    void SetNewSlider();
    std::mutex fSliderMutex;

    Q_PROPERTY(QStringList _Tracks READ getTracks WRITE setTracks NOTIFY TracksChanged FINAL)
    Q_PROPERTY(double _SliderPosition READ getSliderPosition WRITE setSliderPosition NOTIFY SliderPositionChanged FINAL)
    Q_PROPERTY(double _Volume READ getVolume WRITE setVolume NOTIFY VolumeChanged FINAL)

signals:
    void TracksChanged();
    void SliderPositionChanged();
    void VolumeChanged();
};

#endif // MUSICPLAYER_HPP
