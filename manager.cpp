#include "manager.h"
#include "trainer.h"

#include <QDir>
#include <QFileInfo>
#include <QDirIterator>

#include <opencv2/core/core.hpp>

#include <stdio.h>
#include <iostream>


Manager::Manager(QObject *parent) : QObject(parent)
{    
    mRoiRect.left = mRoiRect.top = mRoiRect.right = mRoiRect.bottom = 0;
}

bool Manager::start()
{
    //PROCESSING IS DONE HERE

    qDebug() << "Starting the training session!";
    mState = pr::WORKING;

    //Prepareing filters from enum flag
    bool a,b,c;

    if(mFilters & pr::GAUSS)
        a = true;
    if(mFilters & pr::SOBEL)
        b = true;
    if(mFilters & pr::FEATURE)
        c = true;

    //genrate list of files
    mPositiveFilesList = generateFileList(pr::POSITIVE);
    mNegativeFilesList = generateFileList(pr::NEGATIVE);


    //get the size of one of positive files
    //because for negative files we need to have same size
    //either by resizing or windowing!
    //Assumption is all positive files are the same size!!!!
    //TODO move the getImageSize and Type to helpers.cpp
    cv::Size posSize = pr::getImageSize(mPositiveFilesList.at(0));
    int posType = pr::getImageType(mPositiveFilesList.at(0));

    qDebug() << "Size of positive files are width = " << posSize.width
             << "height = " << posSize.height;


    //Preparing positive trainer
    Trainer trainerPositive(pr::POSITIVE);
    trainerPositive.setTrainingMethod(mMethod);
    trainerPositive.setFileList(&mPositiveFilesList);
    trainerPositive.setFilters(a, b, c);
    trainerPositive.setNumToTrain(mNumberOfImagesToTrain);
    trainerPositive.setRequiredSize(posSize);
    trainerPositive.setRoiRect(mRoiRect);


    //Preparing negative trainer
    Trainer trainerNegative(pr::NEGATIVE);
    trainerNegative.setTrainingMethod(mMethod);
    trainerNegative.setFileList(&mNegativeFilesList);
    trainerNegative.setFilters(a, b, c);
    trainerNegative.setNumToTrain(mNumberOfImagesToTrain);
    trainerNegative.setSizeMode(mSizeMode);
    trainerNegative.setRequiredSize(posSize);
    trainerNegative.setRoiRect(mRoiRect);

    //Start work
    //TODO do this on 2 thread
    pr::training_vector posResult = trainerPositive.performTraining();
    //we do negative for now only if ARFF generation is set to true
    pr::training_vector negResult;
    if(mArffGeneration) {
        negResult = trainerNegative.performTraining();
    } else {
        negResult = pr::training_vector(0);
    }

    //calculate mean and variance for baysian

    //TODO this if statement is redundand....mMethod is enough! lol :D :P
    if(mMethod == pr::GRAYSCALE) {   /**     GRAYSCALE       **/

        BayesianClassifier bcp("positive", mMethod);
        bcp.setFilterGauss(a);
        if(trainerPositive.getShouldCrop()) {
            bcp.setSize(cv::Size(trainerPositive.getCvRect().width
                                 , trainerPositive.getCvRect().height));
            bcp.setShouldCrop(trainerPositive.getShouldCrop());
            bcp.setRoi(trainerPositive.getCvRect());

        } else {
            bcp.setSize(posSize);
        }
        bcp.setType(posType);
        bcp.performCalculations(posResult);
        mTestFilesList = generateTestFileList();
        pr::result_vector results = bcp.performTest(mTestFilesList);

        //save result to CVS file
        pr::saveResultVectorAsCVS(&results, mOutputPath.toLocalFile()
                                  , mProjectName);

    } else if (mMethod == pr::HOG) { /**    HOG     **/

        BayesianClassifier bcp("positive", mMethod);
        bcp.setFilterGauss(a);
        if(trainerPositive.getShouldCrop()) {
            bcp.setSize(cv::Size(trainerPositive.getCvRect().width
                                 , trainerPositive.getCvRect().height));
            bcp.setShouldCrop(trainerPositive.getShouldCrop());
            bcp.setRoi(trainerPositive.getCvRect());
        } else {
            bcp.setSize(posSize);
        }
        bcp.setType(posType);
        bcp.performCalculations(posResult);
        mTestFilesList = generateTestFileList();
        pr::result_vector results = bcp.performTest(mTestFilesList);

        //save result to CVS file
        pr::saveResultVectorAsCVS(&results, mOutputPath.toLocalFile()
                                  , mProjectName);

    }

    //Generate ARFF file
    if(mArffGeneration) {
        qDebug() << "Performing ARFF generation...";
        ArffGenerator ag;

        if(trainerPositive.getShouldCrop()) {
            ag.setImageSize(cv::Size(trainerPositive.getCvRect().width
                                     , trainerPositive.getCvRect().height));
        } else {
            ag.setImageSize(posSize);
        }
        ag.setPosVector(&posResult);
        ag.setNegVector(&negResult);
        ag.setPath(mOutputPath.toLocalFile());
        ag.setProjectName(mProjectName);
        bool result = ag.generateArff(pr::FileType::COMPLETE);
        result = result & ag.generateArff(pr::FileType::POSITIVE_ONLY);
        result = result & ag.generateArff(pr::FileType::NEGATIVE_ONLY);
        if(!result) {
            qDebug() << "Could not generate ARFF file";
        }
    }

    mState = pr::IDLE;

    qDebug() << "Finished with all tasks!"
             << QTime::currentTime().toString();

    return true;
}

//not sure what to do with the method for now!
void Manager::stop()
{
    switch (mState) {
    case pr::IDLE:
        //do nothing
        break;

    case pr::WORKING:
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

            case 2: //test data
                mTestDataPath = folderPath;
                mTestFilesList = pgmFiles;
                break;

            default:
                qWarning() << "Invalid folder ID received from QML. send 0 for"
                           << " POSITIVE check and 1 for NEGATIVE check";
                return false;
            }

            return true;
        } else {
            qWarning() << "There are no 'pgm' files in " << folderPath;
            return false;
        }
    } else {
        qWarning() << "Directory does not exist!";
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
            qWarning() << "Folder is not writeable!";
            return false;
        }
    } else {
        qWarning() << "Directory does not exist!";
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
    mMethod = static_cast<pr::TrainingMethod>(method);
}

void Manager::setFilters(int filters)
{
    QString what("Selected filters:");

    if(filters == 0) {
        what.append(" NONE");
    } else { //this is a trick to see the flag
        if(filters & pr::GAUSS) //e.g. 0 AND 1 or OTHERS & 1
            what.append(" GAUSS");
        if(filters & pr::SOBEL)
            what.append(" SOBEL");
        if(filters & pr::FEATURE)
            what.append(" FEATURE");
    }

    //in case QML calculated the flag wrong
    if(filters < 0 || filters > 8) {
        qWarning() << "Invalid FILTERS enum received!"
                   << "NO filter will be used!";
        mFilters = pr::NONE;
        return;
    }

    qDebug() << what;

    //set the class member, the cast is necessary from int to enum
    mFilters = static_cast<pr::TrainingFilters>(filters);
}

void Manager::setSizeMode(int mode)
{
    switch (mode) {
    case 0:
        mSizeMode = pr::RESIZE;
        qDebug() << "Setting SizeMode to RESIZE";
        break;

    case 1:
        mSizeMode = pr::WINDOW;
        qDebug() << "Setting SizeMode to WINDOW";
        break;

    default:
        qWarning() << "Invalid enum value for SizeMode! setting it to RESIZE";
        break;
    }
}

void Manager::setOutputFileName(QString name)
{
    mProjectName = name;
}

pr::RoiRect Manager::roiRect() const
{
    return mRoiRect;
}

void Manager::setRoiRect(QString l, QString t, QString r, QString b)
{
    mRoiRect.left = l.toInt();
    mRoiRect.top = t.toInt();
    mRoiRect.right = r.toInt();
    mRoiRect.bottom = b.toInt();
}

void Manager::setArffGeneration(bool what)
{
    mArffGeneration = what;
}


QString Manager::outputFileName() const
{
    return mProjectName;
}

pr::SizeMode Manager::sizeMode() const
{
    return mSizeMode;
}

QStringList Manager::generateFileList(pr::TrainingType t)
{
    qDebug() << "Generating list of files for"
             << (t == pr::POSITIVE ? " POSITIVE " : " NEGATIVE ")
             << " data";

    //set source based on type
    QStringList src;
    t == pr::POSITIVE
            ? src = mPositiveFilesList : src = mNegativeFilesList;

    //set dir baed on type
    QString dir;
    t == pr::POSITIVE
            ? dir = mPositiveDataPath.toLocalFile()
            : dir = mNegativeDataPath.toLocalFile();


    QStringList dest;
    int i;
    for(i = 0; i < src.count(); i++) {
        QString file = dir + "/" + src.at(i);
        dest << file; //adds to list
    }

    return dest;
}

QStringList Manager::generateTestFileList() {
    qDebug() << "Generating list of files for TEST data";

    //set source based on type
    QStringList src(mTestFilesList);

    //set dir baed on type
    QString dir(mTestDataPath.toLocalFile());


    QStringList dest;
    int i;
    for(i = 0; i < src.count(); i++) {
        QString file = dir + "/" + src.at(i);
        dest << file; //adds to list
    }

    return dest;
}

pr::TrainingMethod Manager::method() const
{
    return mMethod;
}

pr::TrainingFilters Manager::filters() const
{
    return mFilters;
}

int Manager::numberOfImagesToTrain() const
{
    return mNumberOfImagesToTrain;
}
