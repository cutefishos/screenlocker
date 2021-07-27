import QtQuick 2.0
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.0

import FishUI 1.0 as FishUI

Item {
    id: root

    property alias notification: message.text

    Image {
        id: wallpaperImage
        anchors.fill: parent
        source: "file://" + wallpaper.path
        sourceSize: Qt.size(width * Screen.devicePixelRatio,
                            height * Screen.devicePixelRatio)
        fillMode: Image.PreserveAspectCrop
        asynchronous: false
        clip: true
        cache: false
        smooth: true
    }

    FastBlur {
        id: wallpaperBlur
        anchors.fill: parent
        radius: 64
        source: wallpaperImage
        cached: true
        visible: true
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: FishUI.Units.largeSpacing * 1.5

        Item {
            Layout.fillHeight: true
        }

        Label {
            Layout.alignment: Qt.AlignHCenter
            text: "cutefish"
            color: "white"
        }

        TextField {
            id: password
            Layout.alignment: Qt.AlignHCenter
            Layout.preferredHeight: 40
            Layout.preferredWidth: 240
            placeholderText: qsTr("Password")
            focus: true

            echoMode: TextInput.Password

            background: Rectangle {
                color: "white"
                opacity: 0.4
                radius: FishUI.Theme.bigRadius
            }

            Keys.onEnterPressed: authenticator.tryUnlock(password.text)
            Keys.onReturnPressed: authenticator.tryUnlock(password.text)
            Keys.onEscapePressed: password.text = ""
        }

        Button {
            Layout.alignment: Qt.AlignHCenter
            Layout.preferredHeight: 40
            Layout.preferredWidth: 240
            text: qsTr("Unlock")
            onClicked: authenticator.tryUnlock(password.text)
        }

        Label {
            id: message
            text: ""
            Layout.alignment: Qt.AlignHCenter
            font.bold: true
            color: "white"
            Behavior on opacity {
                NumberAnimation {
                    duration: 250
                }
            }
            opacity: text == "" ? 0 : 1
        }

        Item {
            Layout.fillHeight: true
        }
    }

    Connections {
        target: authenticator
        function onFailed() {
            root.notification = qsTr("Unlocking failed")
        }
        function onGraceLockedChanged() {
            if (!authenticator.graceLocked) {
                root.notification = ""
                password.selectAll()
                password.focus = true
            }
        }
        function onMessage(text) {
            root.notification = text
        }
        function onError(text) {
            root.notification = text
        }
    }
}
