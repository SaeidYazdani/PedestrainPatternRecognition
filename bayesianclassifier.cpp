#include "bayesianclassifier.h"

BayesianClassifier::BayesianClassifier()
{
    mPosMeanCalculated = mNegMeanCalculated = false;
}

void BayesianClassifier::setPosVector(pr::training_vector *posVector)
{
    mPosMeanCalculated = false;
    mPosVector = posVector;
}

void BayesianClassifier::setNegVector(pr::training_vector *negVector)
{
    mNegMeanCalculated = false;
    mNegVector = negVector;
}

pr::double_vector *BayesianClassifier::positiveMeanVector()
{
    if(mPosMeanCalculated) {
        return &mPositiveMeanVector;
    }

    calculateMeanVector(mPosVector, mPositiveMeanVector
                        , pr::TrainingType::POSITIVE);
    return &mPositiveMeanVector;
}

pr::double_vector *BayesianClassifier::positiveStdDevVector()
{
    if(!mPosMeanCalculated) {
        qDebug() << "Need to calculate POSITIVE Mean Vector first...";
        calculateMeanVector(mPosVector, mPositiveMeanVector
                            , pr::TrainingType::POSITIVE);
    }

    calculateStdDevVector(mPosVector, mPositiveStdDevVector
                          , pr::TrainingType::POSITIVE);
    return &mPositiveStdDevVector;
}

pr::double_vector *BayesianClassifier::negativeMeanVector()
{
    if(mNegMeanCalculated) {
        return &mNegativeMeanVector;
    }

    calculateMeanVector(mNegVector, mNegativeMeanVector
                        , pr::TrainingType::NEGATIVE);
    return &mNegativeMeanVector;
}

pr::double_vector *BayesianClassifier::negativeStdDevVector()
{
    if(!mNegMeanCalculated) {
        qDebug() << "Need to calculate NEGATIVE Mean Vector first...";
        calculateMeanVector(mNegVector, mNegativeMeanVector
                            , pr::TrainingType::NEGATIVE);
    }

    calculateStdDevVector(mNegVector, mNegativeStdDevVector
                          , pr::TrainingType::NEGATIVE);
    return &mNegativeStdDevVector;
}

void BayesianClassifier::calculateMeanVector(pr::training_vector *data
                                             , pr::double_vector &out
                                             , pr::TrainingType trainingType) {

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

    switch(trainingType) {
        case pr::TrainingType::POSITIVE:
            mPosMeanCalculated = true;
            return;

        case pr::TrainingType::NEGATIVE:
            mNegMeanCalculated = true;
            return;

        default:
            qDebug() << "INVALID enum for TrainingType is given";
            return;
    }
}

void BayesianClassifier::calculateStdDevVector(pr::training_vector *data
                                               , pr::double_vector &out
                                               ,pr::TrainingType trainingType) {

    //done using ostad's psudo code

    //size of images in training_vector members
    unsigned int imgSize = data->at(0).size();
    //size of number of elements in the training vector
    unsigned int totalSize = data->size();

    //pointr to the first pixel of the firs image in training_vector
    unsigned char *pd = &data->at(0).at(0);
    //pointer to the first element of already calculated mean vector
    double *pm = &mPositiveMeanVector.at(0);

    //an array to hold result of variance
    double variance[mPositiveMeanVector.size()] = {0}; //initialize all with 0
    //pointer to the variance array
    double *vp = variance;

    pr::double_vector varianceVector; //for the final result

    //formula = 1/n*sqrt((x11 - m)^2 + (x21 - m)^2 + ... +  (xn1 - m)^2)

    unsigned int i,j; //loop vars
    //go over all pixels left to right
    for(i = 0; i < imgSize; i++) {
        //go from all elements of data top to bottom
        for(j = 0; j < totalSize; j++) {
            pd = &data->at(j).at(i); //get element at row j column i
            *vp += (*pm - (double)*pd) * (*pm - (double)*pd);
        }

        //now we have covered all the Ith element in J column
        *vp /= (double)(totalSize - 1); //size - 1 is the key to success
        *vp = (double)std::sqrt(*vp);

        //increment the pointers
        vp++;
        pm++;
    }

    //convert variance array to vector
    for(i = 0; i < imgSize; i++) {
        varianceVector.push_back(variance[i]);
    }

    //assign the result to the passed argument
    out = varianceVector;

    //set boolean values of calculation done
    switch(trainingType) {
        case pr::TrainingType::POSITIVE:
            mPosMeanCalculated = true;
            return;

        case pr::TrainingType::NEGATIVE:
            mNegMeanCalculated = true;
            return;

        default:
            qDebug() << "INVALID enum for TrainingType is given";
            return;
    }

}

