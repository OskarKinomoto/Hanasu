import QtQuick 2.0
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.0

import "Icon.js" as MdiFont

Item {
    signal toLogin()
    signal alert(string msg)
    anchors.fill: parent

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
            onClicked: toLogin()
        }

        Label {
            x: (parent.width - width) / 2
            y: (parent.height - height) / 2
            text: "Register"
            color: "white"
            font.bold: true
            font.capitalization: Font.SmallCaps
            font.pixelSize: 20
        }

    }

    ColumnLayout {
        id: columnLayout
        width: 300
        height: 300
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        TextField {
            id: nickI
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            x: 220
            y: 105
            width: 300
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
            Layout.fillWidth: true

            Placeholder {
                text: "Password"
            }
        }

        TextField {
            id: passI2
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            x: 220
            y: 105
            width: 300
            echoMode: "Password"
            Layout.fillWidth: true

            Placeholder {
                text: "Repeat password"
            }

            Keys.onEnterPressed: register()
        }

        Button {
            id: loginBtn
            x: 20
            text: qsTr("Zarejestruj się")
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            onClicked: register()
        }


        Label {
            id: info
            text: qsTr("Register failed")
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

    function register() {
        if (passI.text != passI2.text) {
            alert("Passwords mismatch!")
            return;
        }

        c.registration(nickI.text, passI.text)
    }
}
