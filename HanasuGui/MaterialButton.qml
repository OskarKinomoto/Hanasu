import QtQuick 2.7

Rectangle {
    signal clicked()
    property alias icon: txt.text
    width: 40
    height: 40
    id: root

    property color iconColor: "white"

    color: "transparent"

    Text {
        y: 7
        x: 7
        id: txt
        font.family: "Material Design Icons"
        font.pixelSize: 26
        color: ma.containsMouse ? Qt.darker(iconColor, 1.2) : iconColor



        MouseArea {
            id: ma
            anchors.fill: parent
            onClicked: root.clicked()
            hoverEnabled: true
        }

    }
}
