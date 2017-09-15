import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.0

Popup {
    height: 200
    id: root
    modal: false
    topMargin: (parent.height - height) / 2
    leftMargin: (parent.width - width) / 2
    dim: true

    property alias text: alertLabel.text

    function openStr(str) {
        text = str;
        open()
    }

    ColumnLayout {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        Label {
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            id: alertLabel
            horizontalAlignment: Text.AlignHCenter
            Layout.preferredWidth: 200
            Layout.fillWidth: true
        }

        Item {
            height: 20
        }

        Button {
            id: alertBtn
            text: "Ok"
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            onClicked: {
                root.close()
            }
        }
    }
    width: 300 + (alertLabel.width > 200 ? (alertLabel.width - 200) : 0)
}
