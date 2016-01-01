#include <QPoint>


#include "trainer.h"
#include "pedrecog_types.h"


Trainer::Trainer(QObject *parent) : QObject(parent)
{
    //for compatibility with Qt Framework
}

Trainer::Trainer(PedRec::TrainingType t) : mTrainerType(t)
{
    qDebug() << "Instanciating Trainer class for "
             << (t == PedRec::POSITIVE ? "POSITIVE" : "NEGATIVE");

}

void Trainer::setTrainerType(PedRec::TrainingType t)
{
    this->mTrainerType = t;
}

void Trainer::setFilters(bool gauss, bool sobel, bool feature)
{
    mFilterGauss = gauss;
    mFilterSobel = sobel;
    mFilterFeature = feature;
}

PedRec::TrainingType Trainer::trainerType()
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

cv::Size Trainer::getImageSize(QString file)
{
    return cv::imread(file.toStdString()).size();
}


void Trainer::setSizeMode(const PedRec::SizeMode &sizeMode)
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

PedRec::training_vector Trainer::performTraining()
{
    PedRec::training_vector result;
    int i;
    
    qDebug() << "Starting "
             << (mTrainerType == PedRec::POSITIVE ? "POSITIVE" : "NEGATIVE")
             <<  " training session" << QTime::currentTime().toString();

    //debug
    if(mNumToTrain > mFileList->count()) {
        qDebug() << "Number to train is greater than the list of files"
                 << "So the maximum training will be equal to number of "
                 <<  "available files. mNumToTrain = " << mNumToTrain
                  << " mFilesList count = " << mFileList->count();

        mNumToTrain = mFileList->count();
    }

    for(i = 0; i < mNumToTrain; i++) {

        result.push_back(getPixelValues(mFileList->at(i)));
    }

    qDebug() << "Finished "
             << (mTrainerType == PedRec::POSITIVE ? "POSITIVE" : "NEGATIVE")
             << " training session" << QTime::currentTime().toString();

    return result;
}

PedRec::pixel_vector Trainer::getPixelValues(QString file)
{
    PedRec::pixel_vector pv;
    cv::Mat mat = cv::imread(file.toStdString());

    //check if resizing is required, do if necessary
    if(mTrainerType == PedRec::NEGATIVE) { //only in negative mode
        if(mat.size() != mRequiredSize) { //if size is different
            if(mSizeMode == PedRec::RESIZE) {
                cv::resize(mat,mat, mRequiredSize);
            }
            if(mSizeMode == PedRec::WINDOW) {
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

    if (mat.isContinuous())
    {
        cols = rows*cols;
        rows = 1;
    }

    //THIS IS THE ONLY THING WE LEARNT FROM RDSP-2 UBUNGS!!!!
    for (int r = 0; r < rows; ++r)
    {
        //pointer for pixels
        const uchar *pInput = mat.ptr<uchar>(r);

        for (int c = 0; c < cols; ++c)
        {
            pv.push_back(*pInput);
            ++pInput;
        }
    }

    return pv;
}

void Trainer::showSingleImage(QString file)
{
    cv::Mat img = cv::imread(file.toStdString());
    cv::imshow(file.toStdString(), img);
}
