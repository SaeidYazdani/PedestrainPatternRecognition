import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.2

Window {
    title: qsTr("Pedestrain Pattern Recognition")

    visible: true
    width: 800;
    height: 600;


    property int pathRectsHeight: 50
    property int margin: 10;

    property url pathPos : "";
    property url pathNeg : "";
    property url pathOut : "";
    property int numTrain : 0;


    MainForm {
        id: mainForm;

        //check starting conditions
        function validateInputs() {
            if(rectPos.isValid && rectNeg.isValid && rectOut.isValid) {
                rectStart.validated(true);
            } else {
                rectStart.validated(false);
            }
        }

        //for positive data path and info
        Rectangle {
            id: rectPos;

            x: parent.x + (margin / 2);
            y: parent.y + (margin / 2);
            width: parent.width - margin;
            height: pathRectsHeight - margin;
            color: "red";

            property bool isValid: false;
            signal folderSelected;

            onFolderSelected:  {
                if(isValid) {

                    //check if pos and neg paths are not the same!
                    if(pathPos === pathNeg) {
                        textArea.append("POSITIVE folder can not be"
                                        +" the same as NEGATIVE folder!");

                            //invalid again
                            isValid = false;
                            return;
                    }

                    rectPos.color = "green";
                } else {
                    rectPos.color = "red";
                }

                mainForm.validateInputs();
            }

            Button {
                id:buttonPosPath;
                anchors.centerIn: parent;
                width: parent.width / 2;
                text: qsTr("Select POSITIVE folder");

                onClicked: {
                    dialogPosPath.open();
                }
            }
        }

        //for negative stuff
        Rectangle {
            id: rectNeg;

            x: parent.x + (margin / 2);
            y: parent.y + rectPos.height + margin;
            width: parent.width - margin;
            height: pathRectsHeight - margin;
            color: "red";

            property bool isValid: false;
            signal folderSelected

            onFolderSelected:  {
                if(isValid) {
                    //check if pos and neg paths are not the same!
                    if(pathPos === pathNeg) {
                        textArea.append("NEGATIVE folder can not be"
                                        +" the same as POSITIVE folder!");

                            //invalid again
                            isValid = false;
                            return;
                    }
                    rectNeg.color = "green";
                } else {
                    rectNeg.color = "red";
                }

                mainForm.validateInputs();
            }

            Button {
                id:buttonNegPath;
                anchors.centerIn: parent;
                width: parent.width / 2;
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
            y: rectPos.height  + rectNeg.height + margin + margin / 2;
            width: parent.width - margin;
            height: pathRectsHeight - margin;
            color: "red";

            property bool isValid: false;
            signal folderSelected

            onFolderSelected:  {
                if(isValid) {
                    rectOut.color = "green";
                } else {
                    rectOut.color = "red";
                }

                mainForm.validateInputs();
            }

            Button {
                id:buttonOutPath;
                anchors.centerIn: parent;
                width: parent.width / 2;
                text: qsTr("Select OUTPUT folder");

                onClicked: {
                    dialogOutPath.open();
                }
            }
        }

        Rectangle {
            id: rectOptions;
            x: rectOut.x;
            y: rectOut.y + rectOut.height + margin;
            width: mainForm.width - margin;
            height: (mainForm.height / 4) - margin;
            color: "#600000FF";

            /** BEGIN TEST  **/

            RowLayout {
                id: mainLayout
                width: parent.width;
                height: parent.height;

                GroupBox {
                    id: gbTrainingOptions
                    title: "Training Options"
                    width: parent.width / 2;
                    height: parent.height


                    GridLayout {
                        id: gridLayout
                        rows: 4
                        columns: 2;
                        flow: GridLayout.TopToBottom
                        width: parent.width;
                        height: parent.height;

                        Label { text: "#Images to train" }
                        Label { text: "Training Method" }
                        Label { text: "Pre Filtering" }
                        Label { text: "Size Mode" }

                        TextField {
                            id: tfNumToTrain
                            anchors.right:  parent.right;
                            inputMask: "09999"
                            maximumLength: 5;
                            text: qsTr("50");
                        }

                        ComboBox {
                            id: coboType
                            anchors {
                                left: tfNumToTrain.left;
                                right: tfNumToTrain.right
                            }

                            model: ["BAYESIAN", "HAG", "EXTRA"];
                        }

                        Row {
                            spacing: 5;

                            CheckBox {
                                id: cbGauss
                                text: "GAUSS"
                                checked: false;
                            }

                            CheckBox {
                                id: cbSobel
                                text: "SOBEL"
                                checked: false;
                            }

                            CheckBox {
                                id: cbFeat
                                text: "FEATURE"
                                checked: false;
                            }
                        }

                        ComboBox {
                            id: coboSizeMode;
                            anchors {
                                left: tfNumToTrain.left;
                                right: tfNumToTrain.right
                            }

                            model: ["RESIZE","WINDOW"];
                        }
                    }
                }

                GroupBox {
                    id: gbOutputOptions;
                    title: "Output Options";
                    width: parent.width / 2;
                    height: parent.height;

                    Column {
                        id: rowLayout
                        height: parent.height;
                        width: parent.width;

                        Row {
                            id: rowOutputFile
                            width:parent.width;
                            height: parent.height / 2;
                            spacing: 5;


                            Label {
                                text: "File name";
                            }

                            TextField {

                                id: tfFileName
                                validator: RegExpValidator {
                                    regExp: /^[-\w^&'@{}[\],$=!#().%+~ ]+$/
                                }
                                maximumLength: 128;
                                text: qsTr("default.arff");
                            }
                        }

                        Rectangle {
                            id: rectStart
                            color:"red"
                            width: parent.width - 35;
                            height: parent.height / 2;

                            property bool lastResult: false;

                            signal validated(bool result);

                            onValidated: {
                                if(result) {
                                    buttonStart.enabled = true;
                                    rectStart.color = "green";
                                    textArea.append("Program can be started now!");
                                    lastResult = true;
                                } else {
                                    buttonStart.enabled = false;
                                    rectStart.color = "red";
                                    if(lastResult) {
                                        textArea.append("Please solve the problems!");
                                    }
                                }
                            }

                            Button {
                                anchors.centerIn: parent;
                                id: buttonStart
                                text: qsTr("START")
                                enabled: false;

                                onClicked: {
                                    //send filters
                                    var a, b,c;
                                    cbGauss.checked ? a = 1 : a = 0;
                                    cbSobel.checked ? b = 2 : b = 0;
                                    cbFeat.checked ? c = 4 : c = 0;
                                    cpManager.setFilters(a | b | c);

                                    //send other options
                                    cpManager.setSizeMode(coboSizeMode.currentIndex);
                                    cpManager.setOutputFileName(tfFileName.text);
                                    cpManager.setNumberOfImagesToTrain(tfNumToTrain.text.trim());

                                    var startTime = new Date().getTime();
                                    var result = cpManager.start();

                                    if(result) {
                                    textArea.append("Training is done. Time "
                                                    + (new Date().getTime()
                                                       - startTime)
                                                    + " ms");
                                    }
                                }
                            }
                        }
                    }

                }
            }


            /** END TEST    **/
        }

        Rectangle {
            id: rectMsg;
            x: rectPos.x
            y: rectOptions.y + rectOptions.height + margin;

            height: mainForm.height - (rectPos.height + rectNeg.height + rectOut.height + rectOptions.height) - margin * 4;
            width: parent.width - 10;
            color: "blue"

            TextArea {
                id: textArea;
                anchors.fill: parent;
                readOnly: true;

                function append(msg) {
                    text += msg + "\n";
                }
            }

        }

        //TODO all these dialogs can be only one,,,but I am lazy
        //TODO move dialogs, functions and ... to a separate qml file

        //dialog for positive data folder
        FileDialog {
            id:dialogPosPath;
            selectFolder: true; //this will change to work as FolderDialog!
            title: "Please select folder which contains POSITIVE training data";
            folder: shortcuts.home;

            onAccepted: {
                textArea.append("Selecting POSITIVE data folder");
                pathPos = dialogPosPath.folder;
                var msg;
                var result = cpManager.checkDataFolder(pathPos, 0);

                switch(result) {
                case true:
                    msg = "\tFolder exist and contains 'pgm' files";
                    textArea.append(msg);
                    rectPos.isValid = true;
                    rectPos.folderSelected();
                    break;

                case false:
                    msg = "\Folder does not exist or does not contain"
                            + " 'pgm' files";
                    textArea.append(msg);
                    rectPos.isValid = false;
                    rectPos.folderSelected();
                    break;
                }
            }
        }

        //dialog for negative data folder
        FileDialog {
            id:dialogNegPath;
            selectFolder: true; //this will change to work as FolderDialog!
            title: "Please select folder which contains NEGATIVE training data";
            folder: shortcuts.home;

            onAccepted: {
                textArea.append("Selecting NEGATIVE data folder");
                pathNeg = dialogNegPath.folder;
                var msg;
                var result = cpManager.checkDataFolder(pathNeg, 1);

                switch(result) {
                case true:
                    msg = "\tFolder exist and contains 'pgm' files";
                    textArea.append(msg);
                    rectNeg.isValid = true;
                    rectNeg.folderSelected();
                    break;

                case false:
                    msg = "\tFodler does not exist or does not contain"
                            + " 'pgm' files";
                    textArea.append(msg);
                    rectNeg.isValid = false;
                    rectNeg.folderSelected();
                    break;
                }
            }
        }

        //dialog for output folder
        FileDialog {
            id:dialogOutPath;
            selectFolder: true; //this will change to work as FolderDialog!
            title: "Please select OUTPUT folder";
            folder: shortcuts.home;

            onAccepted: {
                textArea.append("Selecting OUTPUT folder");
                pathOut = dialogOutPath.folder;
                var result = cpManager.checkOutputFolder(pathOut);
                var msg;

                switch(result) {
                case true:
                    msg = "\tFolder exist and is writable";
                    textArea.append(msg);
                    rectOut.isValid = true;
                    rectOut.folderSelected();
                    break;

                case false:
                    msg = "\tFodler does not exist or is not writeable";
                    textArea.append(msg);
                    rectOut.isValid = false;
                    rectOut.folderSelected()
                    break;
                }
            }
        }
    }
}
