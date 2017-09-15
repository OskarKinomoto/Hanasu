import QtQuick 2.5
import QtQuick.Controls 2.2
//import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import QtQuick.Controls.Material 2.1

import "Icon.js" as MdiFont

Item {
    signal fetchConversations()
    signal alert(string str)
    signal toFriends()

    property string convName: ""

    id: root

    Component {
        id: convdelegate
        ConvDel {
            id: delConv
        }
    }


    Component {
        id: msdel
        MsgsDel {
            id: msdel2
        }
    }

    Menubar {
       id: menu
       onAddFriend: friendDial.open()
       onToFriendList: toFriends()
    }

    Item { /* ScrollView */
        id: scrollLeft
        width: 200
        anchors.top: menu.bottom
        anchors.bottom: parent.bottom

        ListView {
            anchors.fill: parent
            model: cs
            delegate: convdelegate
        }
    }

    Item {
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: scrollLeft.right

        Rectangle {
            id: title
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.left: parent.left

            height: 50

            color: Material.color(Material.Teal)

            Label {
                x: 10
                y: (parent.height - height) / 2
                text: convName
                color: "white"
                font.bold: true
                font.capitalization: Font.SmallCaps
                font.pixelSize: 20
            }

            MaterialButton {
                icon: MdiFont.Icon.accountMinus
                anchors.right: parent.right
                anchors.rightMargin: 10
                y: 5
            }
        }

        ScrollView {
            id: msgsScroll
            anchors.top: title.bottom
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.bottom: inp.top
            anchors.topMargin: 10
            ListView {
                id: msgsV
                anchors.right: parent.right
                anchors.left: parent.left

                anchors.fill: parent
                model: msgs
                delegate: msdel
            }

        }

        Rectangle {
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.bottom: inp.top
            height: 2
            id: line
            color: "#cccccc"
        }

        NewMsg {
            id: inp
        }

    }

    function refresh() {
        fetchConversations()
        c.newMsgs();
    }

    Timer {
        id: timer
        interval: 2000
        running: false
        repeat: true
        triggeredOnStart: true
        onTriggered: refresh()
    }

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

    Connections {
        target: cs
        onConversationNameChange: convName = name
    }

    Connections {
        target: msgs
        onNewMsgs: {
            msgsScroll.flickableItem.contentY = 1000000000
        }
    }

    AddFriendPopup {
        id: friendDial
        onAlert: alert(msg)
    }

}
