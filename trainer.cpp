#include "trainer.h"

Trainer::Trainer(QObject *parent) : QObject(parent)
{
    //for compatibility with Qt Framework
}

Trainer::Trainer(pr::TrainingType t) : mTrainerType(t)
{
    qDebug() << "Instanciating Trainer class for "
             << (t == pr::POSITIVE ? "POSITIVE" : "NEGATIVE");

}

void Trainer::setTrainerType(pr::TrainingType t)
{
    this->mTrainerType = t;
}

void Trainer::setFilters(bool gauss, bool sobel, bool feature)
{
    mFilterGauss = gauss;
    mFilterSobel = sobel;
    mFilterFeature = feature;
}

pr::TrainingType Trainer::trainerType()
{
    return mTrainerType;
}

QStringList *Trainer::fileList() const
{
    return mFileList;
}

void Trainer::setFileList(QStringList *fileList)
{
    mFileList = fileList;
}

int Trainer::getNumToTrain() const
{
    return mNumToTrain;
}

void Trainer::setNumToTrain(int numToTrain)
{
    mNumToTrain = numToTrain;
}

void Trainer::setSizeMode(const pr::SizeMode &sizeMode)
{
    mSizeMode = sizeMode;
}

cv::Size Trainer::getRequiredSize() const
{
    return mRequiredSize;
}

void Trainer::setRequiredSize(const cv::Size &requiredSize)
{
    mRequiredSize = requiredSize;
}

pr::TrainingMethod Trainer::getTrainingMethod() const
{
    return mTrainingMethod;
}

void Trainer::setTrainingMethod(const pr::TrainingMethod &trainingMethod)
{
    mTrainingMethod = trainingMethod;
}

pr::training_vector Trainer::performTraining()
{
    pr::training_vector result;
    int i;
    
    qDebug() << "Starting "
             << (mTrainerType == pr::POSITIVE ? "POSITIVE" : "NEGATIVE")
             << " Using" << pr::getTrainingMethodAsString(mTrainingMethod)
             <<  " training session" << QTime::currentTime().toString();

    if(mNumToTrain > mFileList->count()) {
        qWarning() << "Number to train is greater than the list of files"
                   << "So the maximum training will be equal to number of "
                   <<  "available files. mNumToTrain = " << mNumToTrain
                    << " mFilesList count = " << mFileList->count();

        mNumToTrain = mFileList->count();
    }

    switch(mTrainingMethod) {
    case pr::GRAYSCALE:
        for(i = 0; i < mNumToTrain; i++) {
            result.push_back(getGrayscaleFeature(mFileList->at(i)));
        }
        break;
    case pr::HOG:
        for(i = 0; i < mNumToTrain; i++) {
            result.push_back(getHogFeature(mFileList->at(i)));
        }
        break;
    case pr::EXTRA:
        //NOT IMPLEMENTED
        break;
    }

    qDebug() << "Finished "
             << (mTrainerType == pr::POSITIVE ? "POSITIVE" : "NEGATIVE")
             << " training session" << QTime::currentTime().toString();

    return result;
}

pr::pixel_vector Trainer::getGrayscaleFeature(QString file)
{
    pr::pixel_vector pv;
    cv::Mat mat = cv::imread(file.toStdString(), CV_LOAD_IMAGE_ANYDEPTH
                             | CV_LOAD_IMAGE_ANYCOLOR);

    //check if resizing is required, do if necessary
    if(mTrainerType == pr::NEGATIVE) { //only in negative mode
        if(mat.size() != mRequiredSize) { //if size is different
            if(mSizeMode == pr::RESIZE) {
                cv::resize(mat,mat, mRequiredSize);
            }
            if(mSizeMode == pr::WINDOW) {
                //TODO implement window resizing mode
            }
        }
    }

    if(mFilterGauss) {
        //do gauss
    }

    if(mFilterSobel) {
        //do sobel
    }

    if(mFilterFeature) {
        //do feature (this is just a place holder....
        //as we dont know yet what feature!
    }

    //Getting pixel values
    int rows = mat.rows;
    int cols = mat.cols;

    if (mat.isContinuous()) {
        cols = rows*cols;
        rows = 1;
    }

    //THIS IS THE ONLY THING WE LEARNT FROM RDSP-2 UBUNGS!!!!
    for (int r = 0; r < rows; ++r) {
        //pointer for pixels
        const uchar *pInput = mat.ptr<uchar>(r);

        for (int c = 0; c < cols; ++c) {
            pv.push_back((pr::MY_FLOAT)*pInput);
            ++pInput;
        }
    }

    return pv;
}

pr::pixel_vector Trainer::getHogFeature(QString file)
{
    pr::pixel_vector pv;
    cv::Mat mat = cv::imread(file.toStdString(), CV_LOAD_IMAGE_ANYDEPTH
                             | CV_LOAD_IMAGE_ANYCOLOR);

    //check if resizing is required, do if necessary
    if(mTrainerType == pr::NEGATIVE) { //only in negative mode
        if(mat.size() != mRequiredSize) { //if size is different
            if(mSizeMode == pr::RESIZE) {
                cv::resize(mat,mat, mRequiredSize);
            }
            if(mSizeMode == pr::WINDOW) {
                //TODO implement window resizing mode
            }
        }
    }

    int rows = mat.rows;
    int cols = mat.cols;

    //perform X and Y sobel
    cv::Mat dstX = cv::Mat(rows, cols, CV_32FC1);
    cv::Mat dstY = cv::Mat(rows, cols, CV_32FC1);


    cv::Mat matFlaot; //TODO do directly to reduce memory footprint
    mat.convertTo(matFlaot, CV_32FC1);

    cv::Sobel(matFlaot, dstX, -1, 1, 0, 3); //X
    cv::Sobel(matFlaot, dstY, -1, 0, 1, 3); //Y

    //check if everything is continues in memory
    if (mat.isContinuous() && dstX.isContinuous()
            && dstY.isContinuous() && matFlaot.isContinuous()) {
        cols = rows*cols;
        rows = 1;
    }


    //get feature degrees
    for (int r = 0; r < rows; ++r) {

        //pointer for pixels
        const pr::MY_FLOAT *xInput = dstX.ptr<pr::MY_FLOAT>(r);
        const pr::MY_FLOAT *yInput = dstY.ptr<pr::MY_FLOAT>(r);

        for (int c = 0; c < cols; ++c) {

            pv.push_back((pr::MY_FLOAT)atan2(*yInput, *xInput)
                         * ((pr::MY_FLOAT)180/PI));
            ++xInput;
            ++yInput;
        }
    }

    //initializing hog for 0 to 360
    unsigned int initHog[360] = {0}; //important to be inited with 0!!!

    //increment occuring degrees
    for(unsigned int i = 0; i < pv.size(); i++) {
        initHog[180 + (int)pv.at(i)]++;
    }

    pr::pixel_vector finalHog(HOG_CHANNEL, 0);


    //TODO convert to constants to save cycles
    for(int i = 0; i < HOG_CHANNEL; i++) {
        for(int j = 0; j < HOG_BEAN; j++) {
            finalHog.at(i) += initHog[i*HOG_BEAN+j];
        }
    }

    return finalHog;
}

