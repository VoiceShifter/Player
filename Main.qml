import QtQuick
import QtLocation
import QtQuick.Controls
import com.MusicPlayer

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Music Player")
    MusicPlayer
    {
        id: _Player
    }
    Rectangle
    {
        id: _DialogButton
        color: "#063970"
        width: 50
        height: 40
        radius: 60
        anchors.left: parent.left
        anchors.verticalCenter: _Button.verticalCenter
    }

    Rectangle
    {
        id: _Button

        color: "#063970"
        anchors.leftMargin: 10
        anchors.topMargin: 10
        width: 100
        height: 50
        radius: 60
        Image {

            id: _PlayButton
            sourceSize.height: 42
            sourceSize.width: 42
            source: _Player._CurrentIconPath
            anchors.centerIn: parent
        }
        MouseArea
        {
            anchors.fill: parent
            onClicked: {
                _Player._Play_n_Stop()
            }
        }
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
    }
    ListView
    {
        id: _TrackList
        z: -1
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width
        height: parent.height
        model: _Player._Tracks
        spacing: -1

        delegate:
            Rectangle
            {
                id: _Deligated
                border.color: "black"
                border.width: 1                
                width: _TrackList.width
                color: "transparent"
                height: 60
                Text {
                    id: _Name
                    font.bold: true
                    text: modelData

                    anchors.verticalCenter: parent.verticalCenter
                    // anchors.horizontalCenter: parent.horizontalCenter
                    font.pointSize: 12
                    color: "black"
                   }
                MouseArea
                {
                    anchors.fill: parent
                    onClicked:
                    {
                        _Player._ChooseTrack(index);
                        console.log(index)
                        console.log("_TrackList.highlightItem ", _TrackList.highlightItem)
                        _TrackList.currentIndex = index
                    }
                }

            }

        highlight: Rectangle
        {

            anchors.right: parent.right
            width: 1
            color: "lightsteelblue";
            radius: 5
        }
    }

    Slider
    {
        id: _ProgressSlider
        width: parent.width - 20
        value: _Player._SliderPosition
        anchors.bottom: _Button.top
        anchors.bottomMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        background: Rectangle {

                implicitWidth: 200
                implicitHeight: 4
                width: parent.width
                height: parent.height
                radius: 2
                color: "#bdbebf"
        }
        onMoved:
        {
            _Player._SetProgress(_ProgressSlider.position)
            console.log(_ProgressSlider.position)
        }
    }
    Slider
    {
        id: _VolumeSlider
        from: 0
        to: 1
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.left: _Button.right
        anchors.leftMargin: 10
        anchors.verticalCenter: _Button.verticalCenter
        value: _Player._Volume
        onMoved:
        {
            _Player._SetVolume(_VolumeSlider.position)
            console.log(_VolumeSlider.position)
        }
    }
}
