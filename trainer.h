#ifndef TRAINER_H
#define TRAINER_H

#include <QObject>
#include <QDebug>
#include <QTime>
#include <QUrl>
#include <QString>

#include "pr_helper.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


class Trainer : public QObject
{

public:

    /*  CONSTRUCTORS    */
    explicit Trainer(QObject *parent = 0);
    /**
     * @brief Trainer
     * @param type type of training, POSITIVE or NEGATIVE
     */
    Trainer(pr::TrainingType type);


    /*  FUNCTIONS   */

    /**
     * @brief performTraining
     * @return
     */
    pr::training_vector performTraining();

    /**
     * @brief getPixelValues goes through pixels and read and store
     * pixel values in an int array
     * @param file the file to load
     * @return pointer to beggining of result int array
     */
    pr::pixel_vector getPixelValues(QString file);

    //SETTERS

    /**
     * @brief setTrainerType
     * @param trainerType
     */
    void setTrainerType(pr::TrainingType trainerType);

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
    pr::TrainingType trainerType();

    /**
     * @brief fileList
     * @return
     */
    QStringList *fileList() const;

    /**
     * @brief setFileList
     * @param fileList
     */
    void setFileList(QStringList *fileList);

    /**
     * @brief getNumToTrain
     * @return
     */
    int getNumToTrain() const;

    /**
     * @brief setNumToTrain
     * @param numToTrain
     */
    void setNumToTrain(int numToTrain);

    pr::SizeMode getSizeMode() const {return mSizeMode;}
    void setSizeMode(const pr::SizeMode &sizeMode);

    cv::Size getRequiredSize() const;
    void setRequiredSize(const cv::Size &requiredSize);

private:
    /*  MEMBERS */
    pr::TrainingType mTrainerType;
    pr::SizeMode mSizeMode;
    QStringList *mFileList;

    int mNumToTrain;
    bool mFilterGauss, mFilterSobel, mFilterFeature;
    cv::Size mRequiredSize;


    /*  FUNCTIONS   */
};

#endif // TRAINER_H
