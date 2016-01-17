#ifndef BAYESIANCLASSIFIER_H
#define BAYESIANCLASSIFIER_H

#include <QString>
#include <QDebug>
#include <QFile>

#include "patrec_types.h"
#include "iostream"

#include "patrec_types.h"
#include "statisticscalculator.h"
#include <opencv2/core/core.hpp>

class BayesianClassifier
{

public:

    BayesianClassifier(QString className);

    void performCalculations(pr::training_vector tv);

    bool isPositive(QString file);

    cv::Size size() const;
    void setSize(const cv::Size &size);

    int type() const;
    void setType(int type);

private:

    QString mClassName;

    cv::Size mSize;
    int mType;

    cv::Mat mData;
    cv::Mat mMean;
    cv::Mat mTMean; //mean transposed
    cv::Mat mVariance;
    cv::Mat mCovariance;
    cv::Mat mEigenVector;
    cv::Mat mEigenValues;
    cv::Mat mTEigenVector; //eigen vector transposed
};

#endif // BAYESIANCLASSIFIER_H
