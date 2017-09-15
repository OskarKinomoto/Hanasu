import QtQuick 2.0
import QtQuick.Controls.Material 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Popup {
    width: 300
    height: 200
    id: friendDial
    modal: false
    topMargin: (window.height - height) / 2
    leftMargin: (window.width - width) / 2
    dim: true

    signal alert(string msg)

    ColumnLayout {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        TextField {
            id: friendname
            Layout.preferredWidth: 200
            Layout.fillWidth: true
            Keys.onEnterPressed: addFriend()
            Keys.onReturnPressed: addFriend()
        }

        Button {
            id: addFriendBtn
            text: "Add Friend"
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            onClicked: addFriend()
        }
    }

    onVisibleChanged: { friendname.text = "" }

    function addFriend() {
        c.addFriend(friendname.text)
    }
}
