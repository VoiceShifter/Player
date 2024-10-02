#include "MusicPlayer.hpp"

MusicPlayer::MusicPlayer(QObject *parent)
    : QObject{parent}
{
    _AudioOutput = new QAudioOutput;
    _Player.setAudioOutput(_AudioOutput);
}

MusicPlayer::~MusicPlayer()
{
    delete[] _AudioOutput;

}

void MusicPlayer::_PlaySong()
{
    _Player.setSource(QUrl("./Music/Thorn to Liver - Hold on You.mp3"));
    _AudioOutput->setVolume(50);
    _Player.play();
    qDebug() << "Song started playing";
    qDebug() << _Player.isPlaying();



    for (const auto& Iterator : std::filesystem::directory_iterator("./Music"))
    {
        qDebug() << Iterator.path().string() << '\n';
        std::string Buffer{Iterator.path().string()};

        this->Tracks.append(Buffer.substr(Buffer.find('\\') + 1, Buffer.size()).c_str());
        //this->Tracks.push_back(Buffer.substr(Buffer.find("\\", Buffer.size() - 1)));

    }
    qDebug() << Tracks.size();
    // for (const auto& Iterator : this->Tracks)
    // {
    //     qDebug() << Iterator;
    // }
    qDebug() << "Files printed\n";
    emit TracksChanged();

}

QStringList MusicPlayer::getTracks() const
{
    qDebug() << "getter was called";
    qDebug() << Tracks.size();
    return Tracks;
}

void MusicPlayer::setTracks(const QStringList &newTracks)
{
    if (Tracks == newTracks)
        return;
    Tracks = newTracks;
    emit TracksChanged();
}
