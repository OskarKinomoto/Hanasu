import QtQuick 2.0
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.0

import "Icon.js" as MdiFont

Item {
    signal login(string nick, string pass)
    signal register()
    id: page

    anchors.fill: parent


    Timer {
        id: loginTimer
        repeat: false
        interval: 200
        running: false
        onTriggered: login(nickI.text, passI.text)
    }

    Rectangle {
        anchors.right: parent.right
        anchors.left: parent.left
        color: Material.color(Material.Teal)
        height: 50

        Label {
            x: (parent.width - width) / 2
            y: (parent.height - height) / 2
            text: "Login"
            color: "white"
            font.bold: true
            font.capitalization: Font.SmallCaps
            font.pixelSize: 20
        }

        MaterialButton {
            icon: MdiFont.Icon.creation
            anchors.right: parent.right
            anchors.rightMargin: 10
            y: 5
            onClicked: register()
        }

    }

    ColumnLayout {
        id: columnLayout
        width: 300
        height: 200
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        TextField {
            id: nickI
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            x: 220
            y: 105
            width: 300
            text: "user4"
            Layout.fillWidth: true

            Placeholder {
                text: "Nick"
            }
        }

        TextField {
            id: passI
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            x: 220
            y: 105
            width: 300
            echoMode: "Password"
            text: "pass444"
            Layout.fillWidth: true


            Placeholder {
                text: "Password"
            }

            Keys.onEnterPressed: login(nickI.text, passI.text)
            Keys.onReturnPressed: login(nickI.text, passI.text)
        }

        Button {
            id: loginBtn
            x: 20
            text: qsTr("Zaloguj się")
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            onClicked: login(nickI.text, passI.text)
        }

        Label {
            id: info
            text: qsTr("Login failed")
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Material.foreground: Material.Red
        }
    }

    state: "clean"
    states: [
        State {
            name: "clean";
            PropertyChanges {
                target: info;
                opacity: 0
            }
        },
        State {
            name: "failed";
            PropertyChanges {
                target: info;
                opacity: 1
            }
            StateChangeScript {
                name: "fs"
                script: wait.state = "off"
            }
        }

    ]
}
