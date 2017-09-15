import QtQuick 2.0
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.0


Rectangle {
    color: "transparent"
    anchors.right: parent.right
    anchors.left: parent.left

    id: root

    property bool own: owner
    property var col: Material.color(own ? Material.Grey : Material.Indigo)
    property real tw: width * 0.7

    Rectangle {
        id: rec
        radius: 5

        color: col
        anchors.left: own ? undefined : parent.left
        anchors.right: own ? parent.right : undefined
        anchors.leftMargin: 10
        anchors.rightMargin: 10

        width: lab.width + 10
        height: lab.height + 10
    }

    Label {
        id: lab
        text: content
        wrapMode: Label.WordWrap
        anchors.left: rec.left
        anchors.leftMargin: 5
        y: 5
        color: "white"
        textFormat: Text.RichText
        horizontalAlignment: own ? "AlignRight" : "AlignLeft"

        Component.onCompleted: {
            resize()
        }

        function resize() {
            width = undefined
            if (width > 0.7 * root.width)
                width = 0.7 * root.width

        }
    }

    height: rec.height + 10
}
