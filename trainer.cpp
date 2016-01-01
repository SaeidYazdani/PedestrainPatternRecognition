#include "trainer.h"
#include "training_type.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

Trainer::Trainer(QObject *parent) : QObject(parent)
{
    //for compatibility with Qt Framework
}

Trainer::Trainer(PedRecog::TrainingType t) : mTrainerType(t)
{
    qDebug() << "Instanciating Trainer class for "
             << (t == PedRecog::POSITIVE ? "POSITIVE" : "NEGATIVE");

}

void Trainer::setTrainerType(PedRecog::TrainingType t)
{
    this->mTrainerType = t;
}

void Trainer::setFilters(bool gauss, bool sobel, bool feature)
{
    mFilterGauss = gauss;
    mFilterSobel = sobel;
    mFilterFeature = feature;
}

PedRecog::TrainingType Trainer::trainerType()
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

PedRecog::training_vector Trainer::performTraining()
{
    PedRecog::training_vector result;
    int i;

    qDebug() << "Starting "
             << (mTrainerType == PedRecog::POSITIVE ? "POSITIVE" : "NEGATIVE")
             <<  "training session" << QTime::currentTime();

    for(i = 0; i < mNumToTrain; i++) {
        result.push_back(getPixelValues(mFileList->at(i)));
    }

    qDebug() << "Finished training session" << QTime::currentTime();

    return result;
}

PedRecog::pixel_vector Trainer::getPixelValues(QString file)
{
    //THIS IS THE ONLY THING WE LEARNT FROM RDSP-2 UBUNGS!!!!

    PedRecog::pixel_vector pv;
    cv::Mat mat = cv::imread(file.toStdString());

    int rows = mat.rows;
    int cols = mat.cols;

    if (mat.isContinuous())
    {
        cols = rows*cols;
        rows = 1;
    }

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
