#include "manager.h"
#include "trainer.h"

#include <QDir>
#include <QFileInfo>
#include <QDirIterator>

Manager::Manager(QObject *parent) : QObject(parent)
{

}

void Manager::start(QString posPath, QString negPath, QString outputPath, int numToTrain)
{
    Trainer trainerPositive(POSITIVE);
    trainerPositive.setImagePath(posPath);
    trainerPositive.setNumberOfImageToTrain(numToTrain);
    trainerPositive.setOuputPath(outputPath);
    trainerPositive.performTraining();

    Trainer trainerNegative(NEGATIVE);
    trainerNegative.setImagePath(negPath);
    trainerNegative.setNumberOfImageToTrain(numToTrain);
    trainerNegative.setOuputPath(outputPath);
    trainerNegative.performTraining();
}

void Manager::stop()
{
    switch (mState) {
    case IDLE:
        //do nothing
        break;

    case WORKING:
        //TODO stop the work if requested by user
        break;

    default:
        //do nothing?
        break;
    }
}

bool Manager::checkDataFolder(QUrl folderPath)
{
    qDebug() << "Checking for training data folder -> " << folderPath;

    QDir dir(folderPath.path());

    //check if directory exist
    if(dir.exists()) {
        qDebug() << "Folder exists. checking for presence of PMG files";

        //check if pgm files exist and count them
        QStringList filter("*.pgm");
        QStringList pgmFiles = dir.entryList(filter);
        int count = pgmFiles.count();

        if(count > 0) {
            qDebug() << "Number of 'pgm' files -> " << count;
            return true;
        } else {
            qDebug() << "There are no 'pgm' files in " << folderPath;
            return false;
        }
    } else {
        qDebug() << "Directory does not exist!";
        return false;
    }
}

bool Manager::checkOutputFolder(QUrl outPath)
{
    qDebug() << "Checking for output folder -> " << outPath;

    QDir dir(outPath.path());

    //check if directory exists
    if(dir.exists()) {
        qDebug() << "Folder exist!";

        //check if directory is writeable
        QFileInfo fi(outPath.path());
        if(fi.isWritable()) {
            qDebug() << "Folder is writeable!";
            return true;
        } else {
            qDebug() << "Folder is not writeable!";
            return false;
        }
    } else {
        qDebug() << "Directory does not exist!";
        return false;
    }
}
