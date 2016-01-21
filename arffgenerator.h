#ifndef ARFFGENERATOR_H
#define ARFFGENERATOR_H

#include <QTime>
#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QString>
#include <QTextStream>

#include <iostream>

#include "pr_helper.h"

#include <opencv2/core/core.hpp>

/**
 * @brief The ArffGenerator class
 *
 * This class is responsible for saving training_vectors as arff file. also
 * it can save single double_vectors in a file.
 *
 * This class is also can read back from an arff file or vector file to reproduce
 * training_vector or double_vector
 */
class ArffGenerator
{

public:
    ArffGenerator();

    ArffGenerator(QString file);

    pr::training_vector *posVector() const;
    void setPosVector(pr::training_vector *posVector);

    pr::training_vector *negVector() const;
    void setNegVector(pr::training_vector *negVector);

    QString path() const;
    void setPath(const QString &path);

    QString projectName() const;
    void setProjectName(QString &projectName);

    /**
     * @brief generateCompleteArff generate an arff file
     * @param fileType which data should be written in the file
     * @return true if OK, false if something went wrong
     */
    bool generateArff(pr::FileType fileType);

    /**
     * @brief saveSingleVector aveSingleVector saves a single vector to disk. This can be for
     * example positive mean vector or negative standard deviation vector
     * @param vector_name name to append after project name
     * @param fractions number of digits after the decimal point
     * @return
     */
    bool saveSingleVector(QString vector_name, pr::double_vector *vector
                          , int fractions = 4);

    cv::Size imageSize() const;
    void setImageSize(const cv::Size &imageSize);

    /**
     * @brief loadFromFile Reads an ARFF file and convert and return training_vector
     * @param file
     * @return
     */
    pr::training_vector loadFromFile(QString file);

private:
    /*  MEMBERS */
    QString mPath;
    QString mProjectName;
    pr::training_vector *mPosVector;
    pr::training_vector *mNegVector;
    cv::Size mImageSize;    

    /*  FUNCTIONS   */
    void generateHeader(QTextStream &out, pr::FileType fileType);
    void generatePositiveData(QTextStream &out);
    void generateNegativeData(QTextStream &out);


};

#endif // ARFFGENERATOR_H
