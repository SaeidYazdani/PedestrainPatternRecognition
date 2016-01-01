#ifndef TRAINER_H
#define TRAINER_H

#include <QObject>
#include <QDebug>
#include <QTime>
#include <QUrl>
#include <QString>

#include "pedrecog_types.h"


class Trainer : public QObject
{

public:

    /*  CONSTRUCTORS    */
    explicit Trainer(QObject *parent = 0);

    Trainer(PedRec::TrainingType type);


    /*  FUNCTIONS   */

    /**
     * @brief performTraining
     * @return
     */
    PedRec::training_vector performTraining();

    /**
     * @brief getPixelValues goes through pixels and read and store
     * pixel values in an int array
     * @param file the file to load
     * @return pointer to beggining of result int array
     */
    PedRec::pixel_vector getPixelValues(QString file);

    /**
     * @brief showSingleImage
     * @param file
     */
    void showSingleImage(QString file);


    //SETTERS

    /**
     * @brief setTrainerType
     * @param trainerType
     */
    void setTrainerType(PedRec::TrainingType trainerType);

    /**
     * @brief Set the pre-filters that should be done
     * @param gauss
     * @param sobel
     * @param feature
     */
    void setFilters(bool gauss, bool sobel, bool feature);


    //GETTERS

    /**
     * @brief trainerType
     * @return
     */
    PedRec::TrainingType trainerType();


    QStringList *fileList() const;
    void setFileList(QStringList *fileList);

    int getNumToTrain() const;
    void setNumToTrain(int numToTrain);

private:
    /*  MEMBERS */
    PedRec::TrainingType mTrainerType;
    QStringList *mFileList;

    int mNumToTrain;
    bool mFilterGauss, mFilterSobel, mFilterFeature;


    /*  FUNCTIONS   */
};

#endif // TRAINER_H
