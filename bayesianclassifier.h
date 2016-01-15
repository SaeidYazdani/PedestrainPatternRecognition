#ifndef BAYESIANCLASSIFIER_H
#define BAYESIANCLASSIFIER_H

#include <opencv2/core/core.hpp>

class BayesianClassifier
{

public:

    BayesianClassifier();
    BayesianClassifier(cv::Mat *positiveData, cv::Mat *negativeData);

    cv::Mat *positiveData() const;
    void setPositiveData(cv::Mat *positiveData);

    cv::Mat *negativeData() const;
    void setNegativeData(cv::Mat *negativeData);


private:

    cv::Mat *mPositiveData;
    cv::Mat *mNegativeData;
};

#endif // BAYESIANCLASSIFIER_H
