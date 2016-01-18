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

pr::training_vector Trainer::performTraining()
{
    pr::training_vector result;
    int i;
    
    qDebug() << "Starting "
             << (mTrainerType == pr::POSITIVE ? "POSITIVE" : "NEGATIVE")
             <<  " training session" << QTime::currentTime().toString();

    if(mNumToTrain > mFileList->count()) {
        qWarning() << "Number to train is greater than the list of files"
                   << "So the maximum training will be equal to number of "
                   <<  "available files. mNumToTrain = " << mNumToTrain
                   << " mFilesList count = " << mFileList->count();

        mNumToTrain = mFileList->count();
    }

    for(i = 0; i < mNumToTrain; i++) {
        result.push_back(getPixelValues(mFileList->at(i)));
    }

    qDebug() << "Finished "
             << (mTrainerType == pr::POSITIVE ? "POSITIVE" : "NEGATIVE")
             << " training session" << QTime::currentTime().toString();

    return result;
}

pr::pixel_vector Trainer::getPixelValues(QString file)
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

    /*
     * TODO here we should do pre filtering...
     * also each operation should have a function for itself
     * to avoid long code in this function!
     */
    if(mFilterGauss) {
        //do gauess
    }

    if(mFilterSobel) {
        //do sobel
    }

    if(mFilterFeature) {
        //feature detection stuff
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
            pv.push_back((pr::KIRE_KHAR_TYPE)*pInput);
            ++pInput;
        }
    }
    return pv;
}

