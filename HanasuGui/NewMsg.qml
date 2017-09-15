import QtQuick 2.0
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.0

import "Icon.js" as MdiFont

RowLayout {

    id: root
    anchors.right: parent.right
    anchors.left: parent.left
    anchors.bottom: parent.bottom
    height: 48

    TextField {
        id: msgF
        Layout.fillWidth: true

        anchors.verticalCenter: parent.verticalCenter

        Keys.onEnterPressed: send()
        Keys.onReturnPressed: send()
    }


    MaterialButton {
        icon: MdiFont.Icon.send
        iconColor: Material.color(Material.Blue)
        onClicked: send()
    }

    function send() {
        if (msgF.text == "")
            return;
        c.sendMsg(msgF.text)
        msgF.text = ""
    }

}

