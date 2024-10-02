import QtQuick
import QtLocation
import com.MusicPlayer

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")
    MusicPlayer
    {
        id: _Player

    }
    Rectangle
    {
        id: _Button
        color: "Cyan"
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: 10
        anchors.topMargin: 10
        width: 100
        height: 50
        radius: 60
        Image {

            id: _PlayButton
            sourceSize.height: 42
            sourceSize.width: 42
            source: "./Icons/play-button.svg"
            anchors.centerIn: parent
        }
        MouseArea
        {
            anchors.fill: parent
            onClicked: {
                _Player._PlaySong()
            }
        }
    }
    ListView
    {
        id: _TrackList
        anchors.top: _Button.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width
        height: parent.height
        model: _Player._Tracks
        delegate:
            Rectangle
            {
                id: _Deligated
                border.color: "black"
                border.width: 1
                width: _ListLines.width
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
            }
    }
}
