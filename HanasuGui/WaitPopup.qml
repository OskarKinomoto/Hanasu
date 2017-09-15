import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.0


Item {
    id: root
    visible: false
    anchors.fill: parent

        Rectangle {
        id: rec
        anchors.fill: parent

        color: "#eeeeee"
        opacity: 0.9

        BusyIndicator {
            running: image.status === Image.Loading
            id: ind
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
        }
    }
}
