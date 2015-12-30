#include "manager.h"
#include "trainer.h"

manager::manager(QObject *parent) : QObject(parent)
{

}

void manager::start(QString posPath, QString negPath, QString outputPath, int numToTrain)
{
    Trainer trainerPositive(POSITIVE);
    trainerPositive.setImagePath(posPath);
    trainerPositive.setNumberOfImageToTrain(numToTrain);
    trainerPositive.setOuputPath(outputPath);
    trainerPositive.performTraining();

    Trainer trainerNegative(NEGATIVE);
    trainerNegative.setImagePath(posPath);
    trainerNegative.setNumberOfImageToTrain(numToTrain);
    trainerNegative.setOuputPath(outputPath);
    trainerNegative.performTraining();
}

void manager::stop()
{
    switch (mState) {
    case IDLE:
            //do nothing
        break;

    case WORKING:
        //TODO stop the work if requested by user
        break;

    default:
        //do nothing?
        break;
    }
}
