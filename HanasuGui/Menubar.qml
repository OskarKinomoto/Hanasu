import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.0
import QtQuick.Controls 2.1

import "Icon.js" as MdiFont

Rectangle {
    id: root
    width: 200
    height: 50

    signal addFriend()
    signal toFriendList()

    color: Material.color(Material.Teal)

    RowLayout {
        x: 10
        y: 5
        MaterialButton {
            icon: MdiFont.Icon.accountPlus
            onClicked: addFriend()
        }

        MaterialButton {
            icon: MdiFont.Icon.accountMultiple
            onClicked: toFriendList()
        }

    }
}
