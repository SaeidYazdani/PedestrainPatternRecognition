#include "trainer.h"

#include <stdio.h>
#include <iostream>
#include <math.h>

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


    /*
     * TODO here we should do pre filtering...
     * also each operation should have a function for itself
     * to avoid long code in this function!
     */
    if(mFilterGauss) {
        //do gauess
    }

    if(mTrainerType == pr::HOG && mFilterSobel) {
        //do sobel
        qDebug() << "Performing X and Y sobel on " << file;

        cv::Mat dstX = cv::Mat(rows, cols, CV_32FC1);
        cv::Mat dstY = cv::Mat(rows, cols, CV_32FC1);
        cv::Mat matFlaot;

        mat.convertTo(matFlaot, CV_32FC1);


        std::cout << "matFloat BEFORE SOBEL = " << dstX.type();
        std::cout << "dstX BEFORE SOBEL = " << dstX.type();

        cv::Sobel(matFlaot, dstX, -1, 1, 0, 3);
        cv::Sobel(matFlaot, dstY, -1, 0, 1, 3);

        std::cout << "matFloat AFTER SOBEL = " << dstX.type();
        std::cout << "dstX AFTER SOBEL = " << dstX.type();


//        if (mat.isContinuous() && dstX.isContinuous() && dstY.isContinuous()) {
//            cols = rows*cols;
//            rows = 1;
//        }


        pv.clear();


        //std::cout << "MAT\n" << mat << std::endl;
        std::cout << "X\n" << dstX << std::endl;
        std::cout << "Y\n" << dstY << std::endl;

//        for (int r = 0; r < rows; ++r) {
//            //pointer for pixels
//            const uchar *pInput = mat.ptr<uchar>(r);
//            uchar *xInput = dstX.ptr<uchar>(r);
//            uchar *yInput = dstY.ptr<uchar>(r);

//            for (int c = 0; c < cols; ++c) {
//                if(*xInput >= 0) {

//                if(*xInput != 0) {
//                    pv.push_back((pr::MY_FLOAT)(*yInput / *xInput));
//                } else if(*yInput != 0) {

//                    if(*yInput > 0 ) {
//                        pv.push_back(PI_BY_2);
//                    }
//                }

//                } else {

//                }
//                ++pInput;
//            }
//        }



        cv::waitKey(5000);

        return pv;



    }

    if(mFilterFeature) {
        //feature detection stuff
    }



    return pv;
}

