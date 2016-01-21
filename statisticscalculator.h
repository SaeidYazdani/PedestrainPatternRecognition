#ifndef STATISTICSCALCULATOR_H
#define STATISTICSCALCULATOR_H

#include <QDebug>

#include "pr_helper.h"
#include <numeric>

#include<opencv2/core/core.hpp>


class StatisticsCalculator
{

public:
    StatisticsCalculator();



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
     * @brief positiveCovarVector
     * @return
     */
    pr::double_vector *positiveCovarVector();

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

    /**
     * @brief negativeCovarVector
     * @return
     */
    pr::double_vector *negativeCovarVector();

private:
    pr::training_vector *mPosVector;
    pr::training_vector *mNegVector;

    pr::double_vector mPositiveMeanVector;
    pr::double_vector mPositiveStdDevVector;
    pr::double_vector mPositiveCovarVector;

    pr::double_vector mNegativeMeanVector;
    pr::double_vector mNegativeStdDevVector;
    pr::double_vector mNegativeCovarVector;


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

    void calculateCovarVector(pr::training_vector *data
                              , pr::double_vector &out
                              , pr::TrainingType trainingType);

};

#endif // STATISTICSCALCULATOR_H
