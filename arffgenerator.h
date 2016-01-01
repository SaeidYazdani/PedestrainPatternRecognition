#ifndef ARFFGENERATOR_H
#define ARFFGENERATOR_H

#include <QDebug>
#include <QFile>
#include <QString>
#include <QTextStream>

#include "pedrecog_types.h"

#include <opencv2/core/core.hpp>

class ArffGenerator
{

public:
    ArffGenerator();

    PedRec::training_vector *posVector() const;
    void setPosVector(PedRec::training_vector *posVector);

    PedRec::training_vector *negVector() const;
    void setNegVector(PedRec::training_vector *negVector);

    QString path() const;
    void setPath(const QString &path);

    bool generate();

    cv::Size imageSize() const;
    void setImageSize(const cv::Size &imageSize);

private:
    /*  MEMBERS */
    QString mPath;
    PedRec::training_vector *mPosVector;
    PedRec::training_vector *mNegVector;
    cv::Size mImageSize;

    /*  FUNCTIONS   */
    void generateHeader(QTextStream &out);
    void generatePositiveData(QTextStream &out);
    void generateNegativeData(QTextStream &out);


};

#endif // ARFFGENERATOR_H
