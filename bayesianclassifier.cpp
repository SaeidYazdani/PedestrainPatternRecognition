#include "bayesianclassifier.h"


BayesianClassifier::BayesianClassifier(QString className)
{
    mClassName = className;
}

void BayesianClassifier::createAggregateFromTrainingVector(pr::training_vector tv)
{
    //building a Mat object from vector pointer
    cv::Mat mat(tv.size(), tv.at(0).size(), CV_8UC1);

    int rows = mat.rows;
    int cols = mat.cols;

    for (int r = 0; r < rows; ++r) {

        uchar *pInput = mat.ptr<uchar>(r);

        for (int c = 0; c < cols; ++c) {
            *pInput = tv.at(r)[c];
            ++pInput;
        }
    }

    mData = mat;
}

cv::Mat BayesianClassifier::data() const
{
    return mData;
}
