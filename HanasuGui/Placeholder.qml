import QtQuick 2.0
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.0


Label {
    color: "#aaaaaa"
    visible: !parent.text
    y: 8
    font.pixelSize: parent.font.pixelSize
}
