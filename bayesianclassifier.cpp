#include "bayesianclassifier.h"

BayesianClassifier::BayesianClassifier()
{

}

BayesianClassifier::BayesianClassifier(cv::Mat *positiveData
                                       , cv::Mat *negativeData)
{
    mPositiveData = positiveData;
    mNegativeData = negativeData;
}

cv::Mat *BayesianClassifier::positiveData() const
{
    return mPositiveData;
}

void BayesianClassifier::setPositiveData(cv::Mat *positiveData)
{
    mPositiveData = positiveData;
}

cv::Mat *BayesianClassifier::negativeData() const
{
    return mNegativeData;
}

void BayesianClassifier::setNegativeData(cv::Mat *negativeData)
{
    mNegativeData = negativeData;
}
