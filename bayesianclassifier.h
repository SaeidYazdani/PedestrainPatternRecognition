#ifndef BAYESIANCLASSIFIER_H
#define BAYESIANCLASSIFIER_H


#include "patrec_types.h"


class BayesianClassifier
{
public:
    BayesianClassifier();

    void setPosVector(pr::training_vector *posVector);

    void setNegVector(pr::training_vector *negVector);



    pr::double_vector *positiveMeanVector();
    pr::double_vector *positiveStdDevVector();

    pr::double_vector *negativeMeanVector();
    pr::double_vector *negativeStdDevVector();

private:
    pr::training_vector *mPosVector;
    pr::training_vector *mNegVector;

    pr::double_vector mPositiveMeanVector;
    pr::double_vector mPositiveStdDevVector;

    pr::double_vector mNegativeMeanVector;
    pr::double_vector mNegativeStdDevVector;

    //if setPosVector or setNegVector are changed, these will be true
    //so we caluclate again. after calculation these will be false so
    //for getting the values old calculated values will be returned
    //to be optimized ;)
    bool mNewPosData, mNewNegData;

    void calculateMeanVector(pr::training_vector *data
                             , pr::double_vector &out);

    void calculateStdDevVector(pr::training_vector *data
                               , pr::double_vector &out);
};

#endif // BAYESIANCLASSIFIER_H
