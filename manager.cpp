#include "manager.h"
#include "trainer.h"

#include <QDir>
#include <QFileInfo>
#include <QDirIterator>

#include <opencv2/core/core.hpp>

Manager::Manager(QObject *parent) : QObject(parent)
{

}

bool Manager::start()
{
    //PROCESSING IS DONE HERE

    qDebug() << "Starting the training session!";
    mState = PedRec::WORKING;

    //Prepareing filters from enum flag
    bool a,b,c;

    if(mFilters & PedRec::GAUSS)
        a = true;
    if(mFilters & PedRec::SOBEL)
        b = true;
    if(mFilters & PedRec::FEATURE)
        c = true;


    //Preparing positive trainer
    Trainer trainerPositive(PedRec::POSITIVE);
    mPositiveFilesList = generateFileList(PedRec::POSITIVE);
    trainerPositive.setFileList(&mPositiveFilesList);
    trainerPositive.setFilters(a, b, c);
    trainerPositive.setNumToTrain(mNumberOfImagesToTrain);

    //get the size of one of positive files
    //because for negative files we need to have same size
    //either by resizing or windowing!
    //Assumption is all positive files are the same size!!!!
    cv::Size posSize = trainerPositive.getImageSize(mPositiveFilesList.at(0));

    qDebug() << "Size of positive files are width = " << posSize.width
             << "height = " << posSize.height;


    //Preparing negative trainer
    Trainer trainerNegative(PedRec::NEGATIVE);
    mNegativeFilesList = generateFileList(PedRec::NEGATIVE);
    trainerNegative.setFileList(&mNegativeFilesList);
    trainerNegative.setFilters(a, b, c);
    trainerNegative.setNumToTrain(mNumberOfImagesToTrain);
    trainerNegative.setSizeMode(mSizeMode);
    trainerNegative.setRequiredSize(posSize);

    //common settings for both positive and negative



    //Start work
    //TODO do this on 2 thread
    PedRec::training_vector posResult = trainerPositive.performTraining();
    PedRec::training_vector negResult = trainerNegative.performTraining();

    //Generate the ARFF file
    ArffGenerator ag;
    ag.setImageSize(posSize);
    ag.setPosVector(&posResult);
    ag.setNegVector(&negResult);
    ag.setPath(mOutputPath.toLocalFile() + "/" + mOutputFileName);
    bool result = ag.generate();
    if(result) {
        qDebug() << "Output ARFF file created.";
    } else {
        qDebug() << "Could not generate ARFF file";
    }

    mState = PedRec::IDLE;

    return true;
}

//not sure what to do with the method for now!
void Manager::stop()
{
    switch (mState) {
    case PedRec::IDLE:
        //do nothing
        break;

    case PedRec::WORKING:
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
    mMethod = static_cast<PedRec::TrainingMethod>(method);
}

void Manager::setFilters(int filters)
{

    QString what("Selected filters:");

    if(filters == 0) {
        what.append(" NONE");
    } else { //this is a trick to see the flag
        if(filters & PedRec::GAUSS) //e.g. 0 AND 1 or OTHERS & 1
            what.append(" GAUSS");
        if(filters & PedRec::SOBEL)
            what.append(" SOBEL");
        if(filters & PedRec::FEATURE)
            what.append(" FEATURE");
    }

    //in case QML calculated the flag wrong
    if(filters < 0 || filters > 8) {
        qDebug() << "Invalid FILTERS enum received!"
                 << "NO filter will be used!";
        mFilters = PedRec::NONE;
        return;
    }

    qDebug() << what;

    //set the class member, the cast is necessary from int to enum
    mFilters = static_cast<PedRec::TrainingFilters>(filters);
}

void Manager::setSizeMode(int mode)
{
    switch (mode) {
    case 0:
        mSizeMode = PedRec::RESIZE;
        qDebug() << "Setting SizeMode to RESIZE";
        break;

    case 1:
        mSizeMode = PedRec::WINDOW;
        qDebug() << "Setting SizeMode to WINDOW";
        break;

    default:
        qDebug() << "Invalid enum value for SizeMode! setting it to RESIZE";
        break;
    }
}

void Manager::setOutputFileName(QString name)
{
    mOutputFileName = name;
}


QString Manager::outputFileName() const
{
    return mOutputFileName;
}

PedRec::SizeMode Manager::sizeMode() const
{
    return mSizeMode;
}

QStringList Manager::generateFileList(PedRec::TrainingType t)
{
    qDebug() << "Generating list of files for"
             << (t == PedRec::POSITIVE ? " POSITIVE "
                                        : " NEGATIVE ")
             << " data";

    //set source based on type
    QStringList src;
    t == PedRec::POSITIVE
            ? src = mPositiveFilesList : src = mNegativeFilesList;

    //set dir baed on type
    QString dir;
    t == PedRec::POSITIVE
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

PedRec::TrainingMethod Manager::method() const
{
    return mMethod;
}

PedRec::TrainingFilters Manager::filters() const
{
    return mFilters;
}

int Manager::numberOfImagesToTrain() const
{
    return mNumberOfImagesToTrain;
}
