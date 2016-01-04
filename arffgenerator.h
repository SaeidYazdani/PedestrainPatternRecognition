#ifndef ARFFGENERATOR_H
#define ARFFGENERATOR_H

#include <QTime>
#include <QDebug>
#include <QFile>
#include <QString>
#include <QTextStream>

#include "patrec_types.h"

#include <opencv2/core/core.hpp>

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

    bool generate();

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
    pr::training_vector *mPosVector;
    pr::training_vector *mNegVector;
    cv::Size mImageSize;    

    /*  FUNCTIONS   */
    void generateHeader(QTextStream &out);
    void generatePositiveData(QTextStream &out);
    void generateNegativeData(QTextStream &out);


};

#endif // ARFFGENERATOR_H
