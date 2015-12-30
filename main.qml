import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2

Window {
    title: qsTr("Pedestrain Pattern Recognition")

    visible: true
    width: 640;
    height: 480;


    property url pathPos : "";
    property url pathNeg : "";
    property url pathOut : "";
    property int    numTrain : 0;


    MainForm {
        id: mainForm;

        //for positive data path and info
        Rectangle {
            id:     rectPos;

            x:  parent.x + 5;
            y: parent.y + 5;
            width: parent.width - 10;
            height: (parent.height / 5) - 10;
            color: "red";

            property bool isValid: false;
            signal folderSelected

            onFolderSelected:  {
                if(isValid) {
                    rectPos.color = "green";
                } else {
                    rectPos.color = "red";
                }
            }

            Button {
                id:buttonPosPath;
                anchors.centerIn: parent;
                text: qsTr("Select POSITIVE folder");

                onClicked: {
                    dialogPosPath.open();
                }
            }
        }

        //for negative stuff
        Rectangle {
            id: rectNeg;

            x: rectPos.x;
            y: rectPos.height + 10;
            width: parent.width - 10;
            height: (parent.height / 5) - 10;
            color: "red";

            property bool isValid: false;
            signal folderSelected

            onFolderSelected:  {
                if(isValid) {
                    rectNeg.color = "green";
                } else {
                    rectNeg.color = "red";
                }
            }

            Button {
                id:buttonNegPath;
                anchors.centerIn: parent;
                text: qsTr("Select NEGATIVE folder");

                onClicked: {
                    dialogNegPath.open();
                }
            }
        }

        //for output folder and stuff
        Rectangle {
            id: rectOut;

            x: rectNeg.x;
            y: rectPos.height  + rectNeg.height + 15;
            width: parent.width - 10;
            height: (parent.height / 5) - 10;
            color: "red";

            property bool isValid: false;
            signal folderSelected

            onFolderSelected:  {
                if(isValid) {
                    rectOut.color = "green";
                } else {
                    rectOut.color = "red";
                }
            }

            Button {
                id:buttonOutPath;
                anchors.centerIn: parent;
                text: qsTr("Select OUTPUT folder");

                onClicked: {
                    dialogOutPath.open();
                }
            }
        }

        Rectangle {
            id: rectMsg;
            x: rectPos.x
            y: rectOut.y + rectOut.height + 10;
            height: mainForm.height - y - 10;
            width: parent.width - 10;
            color: "black"

            ScrollView {
                width: parent.width - 10;
                height: parent.height - 10;

                TextEdit {
                    id: txtMsg;
                    readOnly: true;
                    anchors.fill: parent;
                    color: "green";
                }
            }
        }




        //TODO all these dialogs can be only one,,,but I am lazy

        //dialog for positive data folder
        FileDialog {
            id:dialogPosPath;
            selectFolder: true; //this will change to work as FolderDialog!
            title: "Please select folder which contains POSITIVE training data";
            folder: shortcuts.home;

            onAccepted: {
                txtMsg.append("Selecting POSITIVE data folder");
                pathPos = dialogPosPath.folder;
                var msg;
                var result = cpManager.checkDataFolder(pathPos);

                switch(result) {
                case true:
                    msg = "\tFolder exist and contains 'pgm' files";
                    rectPos.isValid = true;
                    rectPos.folderSelected();
                    break;

                case false:
                    msg = "\tFodler does not exist or does not contain"
                            + " 'pgm' files";
                    rectPos.isValid = false;
                    rectPos.folderSelected();
                    break;
                }

                txtMsg.append(msg);
            }
        }

        //dialog for negative data folder
        FileDialog {
            id:dialogNegPath;
            selectFolder: true; //this will change to work as FolderDialog!
            title: "Please select folder which contains NEGATIVE training data";
            folder: shortcuts.home;

            onAccepted: {
                txtMsg.append("Selecting NEGATIVE data folder");
                pathNeg = dialogNegPath.folder;
                var msg;
                var result = cpManager.checkDataFolder(pathNeg);

                switch(result) {
                case true:
                    msg = "\tFolder exist and contains 'pgm' files";
                    rectNeg.isValid = true;
                    rectNeg.folderSelected();
                    break;

                case false:
                    msg = "\tFodler does not exist or does not contain"
                            + " 'pgm' files";
                    rectNeg.isValid = false;
                    rectNeg.folderSelected();
                    break;
                }

                txtMsg.append(msg);
            }
        }

        //dialog for output folder
        FileDialog {
            id:dialogOutPath;
            selectFolder: true; //this will change to work as FolderDialog!
            title: "Please select OUTPUT folder";
            folder: shortcuts.home;

            onAccepted: {
                txtMsg.append("Selecting OUTPUT folder");
                pathOut = dialogOutPath.folder;
                var result = cpManager.checkOutputFolder(pathOut);
                var msg;

                switch(result) {
                case true:
                    msg = "\tFolder exist and is writable";
                    rectOut.isValid = true;
                    rectOut.folderSelected();
                    break;

                case false:
                    msg = "\tFodler does not exist or is not writeable";
                    rectOut.isValid = false;
                    rectOut.folderSelected()
                    break;
                }

                txtMsg.append(msg);
            }
        }
    }
}
