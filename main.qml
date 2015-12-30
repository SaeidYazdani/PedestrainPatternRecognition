import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2

Window {
    visible: true
    width: 640;
    height: 480;

    MainForm {
        id: mainForm;

        Rectangle {
            width: parent.width;
            height: parent.height / 4;
            color: "green";

            Text {
                id: textSelPosPath;
                anchors.verticalCenter: parent.Center;
                text: qsTr("Please select POSITIVE data path");
            }

            Button {
                id:buttonPosPath;
                anchors.top: textSelPosPath.bottom;
                text: qsTr("Browse");

                onClicked: {
                    dialogFile.open();
                }

            }

        }

        FileDialog {
            id:dialogFile;
            title: "Please select folder which contains positive training data";
            folder: shortcuts.home;
            onAccepted: {
                console.log("User has selectd " + dialogFile.folder);
                Component.deleteLater();
            }

        }
    }
}
