#ifndef TRAINER_H
#define TRAINER_H

#include <QObject>
#include <QDebug>
#include <QTime>

#include "training_type.h"

class Trainer : public QObject
{

public:
    /*  CONSTRUCTORS    */
    explicit Trainer(QObject *parent = 0);

    Trainer(TrainingType type);


    /*  FUNCTIONS   */

    //SETTERS

    /**
     * @brief setTrainerType
     * @param trainerType
     */
    void setTrainerType(TrainingType trainerType);

    /**
     * @brief setImagePath
     * @param path
     */
    void setImagePath(QString path);

    /**
     * @brief setNumberOfImageToTrain
     * @param num
     */
    void setNumberOfImageToTrain(int num);

    /**
     * @brief setOuputPath
     * @param path
     */
    void setOuputPath(QString path);

    /**
     * @brief performTraining
     * @return true if job is done, false if something went wrong
     */
    bool performTraining();

    //GETTERS

    /**
     * @brief imagesPath
     * @return
     */
    QString imagesPath() const;

    /**
     * @brief outputPath
     * @return
     */
    QString outputPath() const;

    /**
     * @brief numberOfImagesToTrain
     * @return
     */
    int numberOfImagesToTrain() const;

    /**
     * @brief trainerType
     * @return
     */
    TrainingType trainerType();


private:
    /*  MEMBERS */
    TrainingType mTrainerType;
    QString mImagesPath;
    QString mOutputPath;
    int     mNumberOfImagesToTrain;

    /*  FUNCTIONS   */


};

#endif // TRAINER_H
