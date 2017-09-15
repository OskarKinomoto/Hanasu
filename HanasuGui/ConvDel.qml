import QtQuick 2.0
import QtQuick.Controls 2.1


Rectangle {
    color: "transparent"
    width: 200
    height: 50

    property int uuid: id
    property bool active: act
    property bool online: onl

    signal activate(int uid)

    Rectangle {
        id: rec
        width: 200
        height: 50

        color: active ? "#03A9F4" : (ma.containsMouse ? "#81D4FA" : "transparent")

        Circle {
            id: circ
            r: 15
            color: "#EF9A9A"
            anchors.left: parent.left
            anchors.leftMargin: 5
            anchors.verticalCenter: parent.verticalCenter
        }

        Label {
            text: name
            anchors.left: parent.left
            anchors.leftMargin: 40
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: Text.AlignLeft
        }

        MouseArea {
            id: ma
            anchors.fill: parent
            hoverEnabled: true
            onClicked: { cs.activate(uuid); c.showMsgs(uuid);}
        }

        state: "Avaiable"
        states: [
            State {
                name: "Avaiable"
                PropertyChanges {
                    target: circ
                    color: '#A5D6A7'
                }
                when: (online == true)
            },
            State {
                name: "Offline"
                when: (online == false)
            }
        ]
    }

}
