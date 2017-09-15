import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.0
import QtQuick.Controls 2.0

import Qt.labs.platform 1.0

ApplicationWindow {
    //Material.theme: Material.Dark

    id: window
    width: 800
    height: 600
    visible: true

    title: qsTr("Gawariet")

    ListModel {
        id: convmodel
    }

    Component {
        id: convdelegate
        Label {
            text: username
        }
    }

    StackLayout {
        id: stack
        currentIndex: 0
        anchors.fill: parent

        Login {
            id: loginPage
            onLogin: {c.login(nick, pass)}
            onRegister: stack.state = "register"
        }

        Conv {
            id: convPage
            onToFriends: stack.state = "friends"
        }

        FriendList {
            id: flist
            onToConversations: stack.state = "main"
        }

        Register {
            id: register
            onAlert: alert.openStr(msg)
            onToLogin: stack.state = "login"
        }

        states: [
            State {
                name: "login"
                PropertyChanges {
                    target: stack;
                    currentIndex: 0;
                }
            },

            State {
                name: "main"
                PropertyChanges {
                    target: stack;
                    currentIndex: 1;
                }
                PropertyChanges {
                    target: convPage;
                    state: "on"
                }
            },

            State {
                name: "friends"
                PropertyChanges {
                    target: stack;
                    currentIndex: 2
                }
                PropertyChanges {
                    target: flist
                    state: "on"
                }
            },

            State {
                name: "register"
                PropertyChanges {
                    target: stack;
                    currentIndex: 3;
                }
            }

        ]


    }

    Connections {
        target: c
        onLoginFailed: {
            loginPage.state = "failed"
        }
    }

    Connections {
        target: c
        onLoginSuccess: {
            loginPage.state = "clean"
            stack.state = "main"
        }
    }

    Connections {
        target: convPage
        onAlert: alert.openStr(str)
    }



    Alert {
        id: alert
    }

    WaitPopup {
        id: waitPopup
    }

    Connections {
        target: c
        onAlert: alert.openStr(str)
    }

    Connections {
        target: c
        onRegisterSuccess: stack.state = "login"
    }

    Connections {
        target: convPage
        onFetchConversations: {
            c.fetchConv()
        }
    }
}
