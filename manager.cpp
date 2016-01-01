#include "manager.h"
#include "trainer.h"

#include <QDir>
#include <QFileInfo>
#include <QDirIterator>

Manager::Manager(QObject *parent) : QObject(parent)
{

}

bool Manager::start()
{
    //PROCESSING IS DONE HERE

    qDebug() << "Starting the training session!";
    mState = PedRecog::WORKING;

    //Prepareing filters from enum flag
    bool a,b,c;

    if(mFilters & PedRecog::GAUSS)
        a = true;
    if(mFilters & PedRecog::SOBEL)
        b = true;
    if(mFilters & PedRecog::FEATURE)
        c = true;


    //Preparing positive trainer
    Trainer trainerPositive(PedRecog::POSITIVE);
    mPositiveFilesList = generateFileList(PedRecog::POSITIVE);
    trainerPositive.setFileList(&mPositiveFilesList);
    trainerPositive.setFilters(a, b, c);
    trainerPositive.setNumToTrain(mNumberOfImagesToTrain);


    //Preparing negative trainer
    Trainer trainerNegative(PedRecog::NEGATIVE);
    mNegativeFilesList = generateFileList(PedRecog::NEGATIVE);
    trainerNegative.setFileList(&mNegativeFilesList);
    trainerNegative.setFilters(a, b, c);
    trainerPositive.setNumToTrain(mNumberOfImagesToTrain);


    //Start work
    //TODO do this on 2 thread
    PedRecog::training_vector posResult = trainerPositive.performTraining();
    PedRecog::training_vector negResult = trainerNegative.performTraining();

    //TODO now save to ARFF file ;)


    mState = PedRecog::IDLE;

    return true;
}

//not sure what to do with the method for now!
void Manager::stop()
{
    switch (mState) {
    case PedRecog::IDLE:
        //do nothing
        break;

    case PedRecog::WORKING:
        //TODO stop the work if requested by user
        break;

    default:
        //do nothing?
        break;
    }
}

bool Manager::checkDataFolder(QUrl folderPath, int whichFolder)
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

            switch(whichFolder) {
            case 0:
                mPositiveDataPath = folderPath;
                mPositiveFilesList = pgmFiles;
                break;

            case 1:
                mNegativeDataPath = folderPath;
                mNegativeFilesList = pgmFiles;
                break;

            default:
                qDebug() << "Invalid folder ID received from QML. send 0 for"
                         << " POSITIVE check and 1 for NEGATIVE check";
                return false;
            }

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
            mOutputPath = outPath;
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

void Manager::setNumberOfImagesToTrain(QString num)
{
    qDebug() << "Setting number of images to train to "
             << num.toInt();

    mNumberOfImagesToTrain = num.toInt();
}

void Manager::setMethod(int method)
{
    mMethod = static_cast<PedRecog::TrainingMethod>(method);
}

void Manager::setFilters(int filters)
{

    QString what("Selected filters:");

    if(filters == 0) {
        what.append(" NONE");
    } else { //this is a trick to see the flag
        if(filters & PedRecog::GAUSS) //e.g. 0 AND 1 or OTHERS & 1
            what.append(" GAUSS");
        if(filters & PedRecog::SOBEL)
            what.append(" SOBEL");
        if(filters & PedRecog::FEATURE)
            what.append(" FEATURE");
    }

    //in case QML calculated the flag wrong
    if(filters < 0 || filters > 8) {
        qDebug() << "Invalid FILTERS enum received!"
                 << "NO filter will be used!";
        mFilters = PedRecog::NONE;
        return;
    }

    qDebug() << what;

    //set the class member, the cast is necessary from int to enum
    mFilters = static_cast<PedRecog::TrainingFilters>(filters);
}

QStringList Manager::generateFileList(PedRecog::TrainingType t)
{
    qDebug() << "Generating list of files for"
             << (t == PedRecog::POSITIVE ? " POSITIVE "
                                        : " NEGATIVE ")
             << " data";

    //set source based on type
    QStringList src;
    t == PedRecog::POSITIVE
            ? src = mPositiveFilesList : src = mNegativeFilesList;

    //set dir baed on type
    QString dir;
    t == PedRecog::POSITIVE
            ? dir = mPositiveDataPath.toLocalFile()
            : dir = mNegativeDataPath.toLocalFile();


    QStringList dest;
    int i;
    for(i = 0; i < src.count(); i++) {
        QString file = dir
                + "/" + src.at(i);
        dest << file; //adds to list
    }

    return dest;
}

PedRecog::TrainingMethod Manager::method() const
{
    return mMethod;
}

PedRecog::TrainingFilters Manager::filters() const
{
    return mFilters;
}

int Manager::numberOfImagesToTrain() const
{
    return mNumberOfImagesToTrain;
}
