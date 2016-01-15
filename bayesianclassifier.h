#ifndef BAYESIANCLASSIFIER_H
#define BAYESIANCLASSIFIER_H

#include <QString>

#include "patrec_types.h"
#include <opencv2/core/core.hpp>

class BayesianClassifier
{

public:

    BayesianClassifier(QString className);

    void createAggregateFromTrainingVector(pr::training_vector tv);

    cv::Mat data() const;

private:

    QString mClassName;

    cv::Mat mData;
    cv::Mat mMatMean;
    cv::Mat mMatVariance;
    cv::Mat mMatCovariance;
    cv::Mat mEigenVector;
};

#endif // BAYESIANCLASSIFIER_H
