#include "MusicPlayer.hpp"

MusicPlayer::MusicPlayer(QObject *parent)
    : QObject{parent}
{

    _AudioOutput = new QAudioOutput;
    _Player.setAudioOutput(_AudioOutput);
    for (const auto& Iterator : std::filesystem::directory_iterator("./Music"))
    {
        qDebug() << Iterator.path().string() << '\n';
        std::string Buffer{Iterator.path().string()};

        this->Tracks.append(Buffer.substr(Buffer.find('\\') + 1, Buffer.size()).c_str());
        //this->Tracks.push_back(Buffer.substr(Buffer.find("\\", Buffer.size() - 1)));
    }
    State = 0;
    SliderPosition = 0.25;
    Volume  = 50;
    emit TracksChanged();
    emit VolumeChanged();
}

MusicPlayer::~MusicPlayer()
{
    delete _AudioOutput;

}

void MusicPlayer::_Play_n_Stop()
{
    if (State == 0)
    {
        if(_Player.hasAudio() == false)
        {
            qDebug() << "No audio";
            return;
        }
        _AudioOutput->setVolume(Volume);
        _Player.play();
        qDebug() << "Song started playing";
        //qDebug() << _Player.isPlaying();
        State = 2;
        //SliderChecker = std::thread(&MusicPlayer::SetNewSlider, this);

    }
    else if (State == 2)
    {
        _Player.pause();
        qDebug() << "Song paused";
        qDebug() << _Player.position();
        State = 1;
    }
    else if (State == 1)
    {
        qDebug() << "Song paused" << _Player.position();
        _Player.play();
        State = 2;
    }

    emit TracksChanged();

}

void MusicPlayer::_SetVolume(float NewVolume)
{
    qDebug() << NewVolume << "New volume";
    _AudioOutput->setVolume(NewVolume);
    Volume = NewVolume;
    emit VolumeChanged();
}

void MusicPlayer::_SetProgress(float NewProgress)
{
    qDebug() << NewProgress << "New volume";
    double MediaDuration {static_cast<double>(_Player.duration())};
    _Player.setPosition(NewProgress * MediaDuration);
    emit SliderPositionChanged();
}

void MusicPlayer::_ChooseTrack(int Id)
{
    _Player.stop();
    _Player.setSource(QUrl("./Music/" + Tracks[Id]));
    State = 0;
}


QStringList MusicPlayer::getTracks() const
{
    qDebug() << "getter was called";
    qDebug() << Tracks.size();
    return Tracks;
}
void MusicPlayer::SetNewSlider()
{
    using namespace std::literals::chrono_literals;
    //qDebug() << _Player.duration() << " - media duration";
    double MediaDuration {static_cast<double>(_Player.duration())};
    qDebug() << MediaDuration << " - media duration";
    for (;State != 0;)
    {        
        long long int Current {_Player.position()};
        qDebug() << Current;
        SliderPosition = (Current / MediaDuration);
        //qDebug() << SliderPosition << " - new slider position";
        std::this_thread::sleep_for(0.25s);
        emit SliderPositionChanged();
    }
    SliderPosition = 0;
    emit SliderPositionChanged();
    qDebug() << "Slider thread ended";
}
void MusicPlayer::setTracks(const QStringList &newTracks)
{
    if (Tracks == newTracks)
        return;
    Tracks = newTracks;
    emit TracksChanged();
}

double MusicPlayer::getSliderPosition() const
{
    return SliderPosition;
}

void MusicPlayer::setSliderPosition(double newSliderPosition)
{
    if (qFuzzyCompare(SliderPosition, newSliderPosition))
        return;
    SliderPosition = newSliderPosition;
    emit SliderPositionChanged();
}

double MusicPlayer::getVolume() const
{
    return Volume;
}

void MusicPlayer::setVolume(double newVolume)
{
    Volume = newVolume;
    emit VolumeChanged();
}


