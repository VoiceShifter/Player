#include "MusicPlayer.hpp"

MusicPlayer::MusicPlayer(QObject *parent)
    : QObject{parent}, CurrentIconPath{"./Icons/play-button.svg"}, CurrentDirectory{"./Music/"}
{
    _AudioOutput = new QAudioOutput;
    _Player.setAudioOutput(_AudioOutput);
    for (const auto& Iterator : std::filesystem::directory_iterator(CurrentDirectory))
    {
        qDebug() << Iterator.path().string() << '\n';
        std::string Buffer{Iterator.path().string()};

        this->Tracks.append(Buffer.substr(Buffer.find('/', 3) + 1, Buffer.size()).c_str());
        //this->Tracks.push_back(Buffer.substr(Buffer.find("\\", Buffer.size() - 1)));
    }
    fState = States::SongNotStarted;
    SliderPosition = 0;
    Volume  = 1;
    _AudioOutput->setVolume(Volume);
    for (const auto& Iterator : Tracks)
    {
        qDebug() << Iterator;
    }
    if (Tracks.size() > 0)
    {
        _Player.setSource(CurrentDirectory.c_str() + Tracks[0]);
    }
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
        CurrentIconPath = "./Icons/play-button.svg";

        emit IconPathChanged();

    }
    else if (fState == States::SongPaused)
    {
        qDebug() << "Song continued" << _Player.position();
        _Player.play();
        fState = States::SongPlaying;
        CurrentIconPath = "./Icons/pause-button.svg";
        emit IconPathChanged();
    }
}



void MusicPlayer::_SetProgress(float NewProgress)
{
    _AudioOutput->setVolume(0);
    qDebug() << NewProgress << "New volume";
    double MediaDuration {static_cast<double>(_Player.duration())};
    _Player.setPosition(NewProgress * MediaDuration);    
    emit SliderPositionChanged();
    _AudioOutput->setVolume(Volume);
}

void MusicPlayer::_ChooseTrack(int ID)
{
    _Player.stop();
    CurrentIconPath = "./Icons/play-button.svg";
    emit IconPathChanged();

    fState = States::SongNotStarted;
    if (SliderChecker.joinable())
    {
        SliderChecker.join();
    }
    _Player.setSource(QUrl(CurrentDirectory.c_str() + Tracks[ID]));
    SliderPosition = 0;
    emit SliderPositionChanged();
}

void MusicPlayer::_ChangeDirectory(QString NewPath)
{
    CurrentDirectory = NewPath.toStdString().substr(7, NewPath.size()) + "/";
    _Player.setSource(QUrl((CurrentDirectory).c_str()));
    Tracks.clear();
    for (const auto& Iterator : std::filesystem::directory_iterator(CurrentDirectory))
    {
        qDebug() << Iterator.path().string() << '\n';
        std::string Buffer{Iterator.path().string()};
        this->Tracks.append(Buffer.substr(CurrentDirectory.size(), Buffer.size()).c_str());
    }
    emit TracksChanged();
    if (Tracks.size() > 0)
    {
        _Player.setSource(CurrentDirectory.c_str() + Tracks[0]);
    }
}


void MusicPlayer::SetNewSlider()
{
    using namespace std::literals::chrono_literals;
    //qDebug() << _Player.duration() << " - media duration";
    double MediaDuration {static_cast<double>(_Player.duration())};
    qDebug() << MediaDuration << " - media duration";
    fSliderMutex.lock();
    for (;fState == States::SongPlaying || fState == States::SongPaused;)
    {
        long long int Current {_Player.position()};
        //qDebug() << "Player.position = " << Current;
        SliderPosition = (Current / MediaDuration);
        //qDebug() << SliderPosition << " - new slider position";
        std::this_thread::sleep_for(0.25s);
        emit SliderPositionChanged();
    }    
    qDebug() << "Slider thread ended";
    fSliderMutex.unlock();
}
