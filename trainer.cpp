#include "trainer.h"
#include "training_type.h"

Trainer::Trainer(QObject *parent) : QObject(parent)
{
    //for compatibility with Qt Framework
}

Trainer::Trainer(TrainingType t) : mTrainerType(t)
{
    qDebug() << "Instanciating Trainer class";
    mNumberOfImagesToTrain = 0;
    mImagesPath = QString();
    mOutputPath = QString();
}

void Trainer::setTrainerType(TrainingType t)
{
    this->mTrainerType = t;
}


void Trainer::setImagePath(QString path)
{
    this->mImagesPath = path;
}

void Trainer::setNumberOfImageToTrain(int num)
{
    this->mNumberOfImagesToTrain = num;
}

void Trainer::setOuputPath(QString path)
{
    mOutputPath = path;
}

bool Trainer::performTraining()
{
    qDebug() << "Starting training session" << QTime::currentTime();
    switch(mTrainerType) {
        case POSITIVE:
        qDebug() << "Training type will be POSITIVE";
        break;

        case NEGATIVE:
        qDebug() << "Training type will be NEGATIVE";
        break;

        default:
            qDebug() << "Training type is invalid. returning false";
            return false;
        break;
    }

    return true;
}

QString Trainer::imagesPath() const
{
    return mImagesPath;
}

QString Trainer::outputPath() const
{
    return mOutputPath;
}

int Trainer::numberOfImagesToTrain() const
{
    return mNumberOfImagesToTrain;
}

TrainingType Trainer::trainerType()
{
    return mTrainerType;
}
