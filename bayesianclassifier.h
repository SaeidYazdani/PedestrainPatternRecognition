#ifndef BAYESIANCLASSIFIER_H
#define BAYESIANCLASSIFIER_H

#include <iostream>

#include <QString>
#include <QDebug>
#include <QFile>

#include <opencv2/core/core.hpp>

#include "pr_helper.h"
#include "statisticscalculator.h"

#define PI_2 6.28318530718

class BayesianClassifier
{

public:

    BayesianClassifier(QString className);

    void performCalculations(pr::training_vector tv);
    cv::Size size() const;
    void setSize(const cv::Size &size);

    int type() const;
    void setType(int type);

    pr::result_vector performTest(QStringList files);

private:

    QString mClassName;

    cv::Size mSize;

    int mNSpace;
    int mType;

    cv::Mat mData;
    cv::Mat mMean;
    cv::Mat mTMean; //mean transposed
    cv::Mat mVariance;
    cv::Mat mCovariance;
    cv::Mat mTCovariance;
    cv::Mat mEigenVector;
    cv::Mat mEigenValues;
    cv::Mat mTEigenVector; //eigen vector transposed

    double mDetCovar; //determinan of covariance

    double mLnDet = 0; //ln of determinant

    void calculateLnOfDetOfCovar();

    //functions
    pr::TestResult isPositive(QString files);
};

#endif // BAYESIANCLASSIFIER_H
