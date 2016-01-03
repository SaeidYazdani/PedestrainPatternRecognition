#include "bayesianclassifier.h"

BayesianClassifier::BayesianClassifier()
{
    mNewNegData = mNewPosData = true;
}

void BayesianClassifier::setPosVector(pr::training_vector *posVector)
{
    mNewPosData = true;
    mPosVector = posVector;
}

void BayesianClassifier::setNegVector(pr::training_vector *negVector)
{
    mNewNegData = true;
    mNegVector = negVector;
}

pr::double_vector *BayesianClassifier::positiveMeanVector()
{
    if(!mNewPosData) {
        return &mPositiveMeanVector;
    }

    calculateMeanVector(mPosVector, mPositiveMeanVector);
    mNewPosData = false; //to return the already calculated later
    return &mPositiveMeanVector;
}

pr::double_vector *BayesianClassifier::positiveStdDevVector()
{
    if(!mNewPosData) {
        return &mPositiveStdDevVector;
    }

    calculateStdDevVector(mPosVector, mPositiveStdDevVector);
    mNewPosData = false; //to return the already calculated later
    return &mPositiveStdDevVector;
}

pr::double_vector *BayesianClassifier::negativeMeanVector()
{
    if(!mNewNegData) {
        return &mNegativeMeanVector;
    }

    calculateMeanVector(mNegVector, mNegativeMeanVector);
    return &mPositiveMeanVector;
}

pr::double_vector *BayesianClassifier::negativeStdDevVector()
{
    if(!mNewNegData) {
        return &mNegativeStdDevVector;
    }

    calculateStdDevVector(mNegVector, mNegativeStdDevVector);
    return &mNegativeStdDevVector;
}

void BayesianClassifier::calculateMeanVector(pr::training_vector *data
                                             , pr::double_vector &out)
{
    //note: assumption is all elements in vector have the same size!
    //this should make no problem as long as the images resulted in the
    //vector and its sub vectors all had the same size!

    //the code is taken from
    //http://stackoverflow.com/questions/14924912

    unsigned int dataCount = data->size();
    unsigned int elementsCount = data->at(0).size();

    pr::double_vector colSums(elementsCount);

    std::for_each(data->begin(), data->end(),

        [&](const std::vector<uchar>& row)
        {
            // Use transform overload that takes two input ranges.
            // Note that colsums is the second input range as well as the output range.
            // We take each element of the row and add it to the corresponding
            // element of colsums vector:
            std::transform(row.begin(), row.end(), colSums.begin(), colSums.begin(),
                           [](double d1, double d2) { return d1 + d2; });
        });

    //now get the mean value
    for (unsigned int i = 0; i < elementsCount; i++) {
        colSums[i] /= dataCount;
    }

    out = colSums;
}

void BayesianClassifier::calculateStdDevVector(pr::training_vector *data, pr::double_vector &out)
{
    //TODO implement this...should be very similar to mean vector calculation

}

