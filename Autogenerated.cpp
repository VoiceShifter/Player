# include "MusicPlayer.hpp"

void MusicPlayer::_SetVolume(float NewVolume)
{
    qDebug() << NewVolume << "New volume";
    _AudioOutput->setVolume(NewVolume);
    Volume = NewVolume;
    emit VolumeChanged();
}

QStringList MusicPlayer::getTracks() const
{
    qDebug() << "getter was called";
    qDebug() << Tracks.size();
    return Tracks;
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
