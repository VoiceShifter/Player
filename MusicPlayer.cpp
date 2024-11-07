#include "MusicPlayer.hpp"

MusicPlayer::MusicPlayer(QObject *parent)
    : QObject{parent}, CurrentIconPath{"./Icons/play-button.svg"}
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
    fState = States::SongNotStarted;
    SliderPosition = 0;
    Volume  = 1;
    _AudioOutput->setVolume(Volume);
}

MusicPlayer::~MusicPlayer()
{
    delete _AudioOutput;
}

void MusicPlayer::_Play_n_Stop()
{
    if (fState == States::SongNotStarted)
    {
        if(_Player.hasAudio() == false)
        {
            qDebug() << "No audio";
            return;
        }
        _Player.play();
        fState = States::SongPlaying;
        CurrentIconPath = "./Icons/pause-button.svg";
        qDebug() << "Song started playing";


        SliderChecker = std::thread(&MusicPlayer::SetNewSlider, this);


        emit IconPathChanged();

    }
    else if (fState == States::SongPlaying)
    {
        _Player.pause();
        fState = States::SongPaused;
        qDebug() << "Song paused";
        qDebug() << _Player.position();
        State = 1;
        CurrentIconPath = "./Icons/pause-button.svg";
        emit IconPathChanged();
    }
    else if (fState == States::SongPaused)
    {
        qDebug() << "Song paused" << _Player.position();
        _Player.play();
        fState = States::SongPlaying;
        State = 2;
        CurrentIconPath = "./Icons/play-button.svg";
        emit IconPathChanged();
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
    fState = States::SongNotStarted;
    State = 0;
    if (SliderChecker.joinable())
    {
        SliderChecker.join();
    }
    fSliderMutex.lock();
    _Player.setSource(QUrl("./Music/" + Tracks[Id]));
    fSliderMutex.unlock();
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
    fSliderMutex.lock();
    for (;fState == States::SongPlaying;)
    {
        long long int Current {_Player.position()};
        qDebug() << "Player.position = " << Current;
        SliderPosition = (Current / MediaDuration);
        //qDebug() << SliderPosition << " - new slider position";
        std::this_thread::sleep_for(0.25s);
        emit SliderPositionChanged();
    }
    SliderPosition = 0;
    emit SliderPositionChanged();
    qDebug() << "Slider thread ended";
    fSliderMutex.unlock();
}

QString MusicPlayer::getCurrentIconPath() const
{
    return CurrentIconPath;
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


