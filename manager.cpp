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
    mState = pr::WORKING;

    //Prepareing filters from enum flag
    bool a,b,c;

    if(mFilters & pr::GAUSS)
        a = true;
    if(mFilters & pr::SOBEL)
        b = true;
    if(mFilters & pr::FEATURE)
        c = true;

    //Preparing positive trainer
    Trainer trainerPositive(pr::POSITIVE);
    mPositiveFilesList = generateFileList(pr::POSITIVE);
    trainerPositive.setFileList(&mPositiveFilesList);
    trainerPositive.setFilters(a, b, c);
    trainerPositive.setNumToTrain(mNumberOfImagesToTrain);

    //get the size of one of positive files
    //because for negative files we need to have same size
    //either by resizing or windowing!
    //Assumption is all positive files are the same size!!!!
    //TODO move the getImageSize and Type to helpers.cpp
    cv::Size posSize = pr::getImageSize(mPositiveFilesList.at(0));
    int posType = pr::getImageType(mPositiveFilesList.at(0));

    qDebug() << "Size of positive files are width = " << posSize.width
             << "height = " << posSize.height;


    //Preparing negative trainer
    Trainer trainerNegative(pr::NEGATIVE);
    mNegativeFilesList = generateFileList(pr::NEGATIVE);
    trainerNegative.setFileList(&mNegativeFilesList);
    trainerNegative.setFilters(a, b, c);
    trainerNegative.setNumToTrain(mNumberOfImagesToTrain);
    trainerNegative.setSizeMode(mSizeMode);
    trainerNegative.setRequiredSize(posSize);

    //Start work
    //TODO do this on 2 thread
    pr::training_vector posResult = trainerPositive.performTraining();
    pr::training_vector negResult = trainerNegative.performTraining();

    //Generate the ARFF file
    ArffGenerator ag;
    ag.setImageSize(posSize);
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

    //calculate mean and variance for baysian
    if(mMethod == pr::BAYESIAN) {

        BayesianClassifier bcp("positive");
        bcp.setSize(posSize);
        bcp.setType(posType);
        bcp.performCalculations(posResult);


//        StatisticsCalculator bc;
//        bc.setPosVector(&posResult);
//        bc.setNegVector(&negResult);

//        qDebug() << "Calculating POSITIVE Bayesian Mean vector"
//                 << QTime::currentTime().toString();
//        pr::double_vector *posMeanVector = bc.positiveMeanVector();
//        qDebug() << "showing POSITIVE Bayesian Mean as an image"
//                 << QTime::currentTime().toString();
//        //show pos mean vector as image!!
//        pr::convertVectorToImageAndShow(posSize, posType, posMeanVector
//                                                 , "POSITIVE MEAN VECTOR");


//        qDebug() << "Calculating POSITIVE Bayesian StdDev vector"
//                 << QTime::currentTime().toString();
//        pr::double_vector *posStdDevVector = bc.positiveStdDevVector();
//        qDebug() << "showing POSITIVE Bayesian StdDev as an image"
//                 << QTime::currentTime().toString();
//        //show pos std dev vector as image!!
//        pr::convertVectorToImageAndShow(posSize, posType, posStdDevVector
//                                                 , "POSITIVE STD DEV VECTOR");

//        qDebug() << "Calculating NEGATIVE Bayesian Mean vector"
//                 << QTime::currentTime().toString();
//        pr::double_vector *negMeanVector = bc.negativeMeanVector();
//        qDebug() << "showing NEGATIVE Bayesian Mean as an image"
//                 << QTime::currentTime().toString();
//        //show neg mean vector as image!!
//        pr::convertVectorToImageAndShow(posSize, posType, negMeanVector
//                                                 , "NEGATIVE MEAN VECTOR");


//        qDebug() << "Calculating NEGATIVE Bayesian StdDev vector"
//                 << QTime::currentTime().toString();
//        pr::double_vector *negStdDevVector = bc.negativeStdDevVector();
//        qDebug() << "showing NEGATIVE Bayesian StdDev as an image"
//                 << QTime::currentTime().toString();
//        //show neg std dev vector as image!!
//        pr::convertVectorToImageAndShow(posSize, posType, negStdDevVector
//                                                 , "NEGATIVE STD DEV VECTOR");

//        //save vectors
//        ag.saveSingleVector("positive_mean", posMeanVector);
//        ag.saveSingleVector("positive_std_dev", posStdDevVector);
//        ag.saveSingleVector("negative_mean", negMeanVector);
//        ag.saveSingleVector("negative_std_dev", negStdDevVector);
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
