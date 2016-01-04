#ifndef BAYESIANCLASSIFIER_H
#define BAYESIANCLASSIFIER_H

#include <QDebug>

#include "patrec_types.h"
#include <numeric>


class BayesianClassifier
{
public:
    BayesianClassifier();

    /**
     * @brief setPosVector
     * @param posVector
     */
    void setPosVector(pr::training_vector *posVector);

    /**
     * @brief setNegVector
     * @param negVector
     */
    void setNegVector(pr::training_vector *negVector);


    /**
     * @brief positiveMeanVector
     * @return
     */
    pr::double_vector *positiveMeanVector();

    /**
     * @brief positiveStdDevVector
     * @return
     */
    pr::double_vector *positiveStdDevVector();

    /**
     * @brief negativeMeanVector
     * @return
     */
    pr::double_vector *negativeMeanVector();

    /**
     * @brief negativeStdDevVector
     * @return
     */
    pr::double_vector *negativeStdDevVector();

private:
    pr::training_vector *mPosVector;
    pr::training_vector *mNegVector;

    pr::double_vector mPositiveMeanVector;
    pr::double_vector mPositiveStdDevVector;

    pr::double_vector mNegativeMeanVector;
    pr::double_vector mNegativeStdDevVector;


    //these booleans indicate if calculation of positive and negative
    //mean vectors are done or not. if not, and calculate std dev is requested
    //then mean vectors will be calculated first because std dev relies on them
    bool mPosMeanCalculated, mNegMeanCalculated;


    void calculateMeanVector(pr::training_vector *data
                             , pr::double_vector &out
                             , pr::TrainingType trainingType);

    void calculateStdDevVector(pr::training_vector *data
                               , pr::double_vector &out
                               , pr::TrainingType trainingType);
};

#endif // BAYESIANCLASSIFIER_H
