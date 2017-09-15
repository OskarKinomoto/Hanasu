import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.0

import "Icon.js" as MdiFont

Item {
    anchors.fill: parent

    signal toConversations()


    Component {
        id: fDel

        RowLayout {
            anchors.right: parent.right
            anchors.left: parent.left

            opacity: role == 0 ? 1 : 0
            Label {
                text: name
            }

            RowLayout {

                MaterialButton {
                    icon: MdiFont.Icon.accountRemove
                    onClicked: c.rejectFriend(id)
                    iconColor: Material.color(Material.Red)
                }

                MaterialButton {
                    icon: MdiFont.Icon.accountCheck
                    onClicked: c.acceptFriend(id)
                    iconColor: Material.color(Material.Green)
                }
            }

        }
    }

    Rectangle {
        anchors.right: parent.right
        anchors.left: parent.left
        color: Material.color(Material.Teal)
        height: 50

        MaterialButton {
            y: 5
            anchors.leftMargin: 10
            anchors.left: parent.left
            icon: MdiFont.Icon.arrowLeft
            onClicked: toConversations()
        }

        Label {
            x: (parent.width - width) / 2
            y: (parent.height - height) / 2
            text: "Friends"
            color: "white"
            font.bold: true
            font.capitalization: Font.SmallCaps
            font.pixelSize: 20
        }

    }


    ListView {
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.topMargin: 50
        anchors.leftMargin: 10
        width: parent.width / 2
        model: fmodel
        delegate: fDel
    }


    Timer {
        id: timer
        interval: 3000
        running: false
        triggeredOnStart: true
        onTriggered: c.fetchFriends()
    }

    state: "off"

    states: [
        State {
            name: "off"
        },

        State {
            name: "on"
            PropertyChanges {
                target: timer
                running: true
            }
        }
    ]

}
