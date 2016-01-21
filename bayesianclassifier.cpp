#include "bayesianclassifier.h"

#include <math.h>

using namespace cv;
using namespace std;

BayesianClassifier::BayesianClassifier(QString className)
{
    mClassName = className;
}

cv::Size BayesianClassifier::size() const
{
    return mSize;
}

void BayesianClassifier::setSize(const cv::Size &size)
{
    mSize = size;
    mNSpace = size.height * size.width;
}

int BayesianClassifier::type() const
{
    return mType;
}

void BayesianClassifier::setType(int type)
{
    mType = type;
}



void BayesianClassifier::performCalculations(pr::training_vector tv)
{

    qDebug() << "Size of training vector " << tv.size() <<  "x"
             << tv.at(0).size();

    //building a Mat object from vector pointer
    cv::Mat mat(tv.size(), tv.at(0).size(), CV_32FC1);

    int rows = mat.rows;
    int cols = mat.cols;

    for (int r = 0; r < rows; ++r) {

        pr::MY_FLOAT *pInput = mat.ptr<pr::MY_FLOAT>(r);

        for (int c = 0; c < cols; ++c) {
            *pInput = tv.at(r)[c];
            ++pInput;
        }
    }

    mData = mat;

    //calculation of mean and covariance
    cv::Mat covar, mean;
    cv::calcCovarMatrix(mData, covar, mean
                        , CV_COVAR_NORMAL | CV_COVAR_ROWS, CV_32FC1);

    //THESE WILL BE SIGNED FLOAT!!!!!!!!
    mCovariance = covar;
    mMean = mean;

    //SHOW MEAN as image
    cv::Mat meanMatToShow = cv::Mat(mSize.height, mSize.width, CV_8UC1);

    int i,j, k = 0;
    for(i = 0; i < mSize.height; i++) {
        for(j = 0; j < mSize.width; j++) {
            meanMatToShow.at<uchar>(i,j) = (uchar) mean.at<pr::MY_FLOAT>(0, k);
            k++;
        }
    }
    pr::showSingleImage(mClassName +  " => MEAN OF SAMPLES", meanMatToShow);

    //SHOW COVAR as image
    cv::Mat covarNorm;
    cv::normalize(mCovariance, covarNorm, 0, 255, NORM_MINMAX, CV_8UC1);
    pr::showSingleImage(mClassName +  " => NORMALIZED [0,255] COVARIANCE OF SAMPLES", covarNorm);

    //Eigenvalues calculations
    cv::Mat eigenvals, eigenvector;
    cv::eigen(mCovariance, eigenvals, eigenvector);

    mEigenValues = eigenvals;
    mEigenVector = eigenvector; //B

    cv::Mat eigenVecTrans;

    cv::transpose(mEigenVector, eigenVecTrans);
    mTEigenVector = eigenVecTrans; //BT

    //FIXME this might crash
    //calculateLnOfDetOfCovar(); // to claculate mLnDet

//    cout << mCovariance.size() << endl;

//    mDetCovar = cv::determinant(mCovariance);

    //cout << "COVARIANCE DETERMINANT = " << mDetCovar << endl;
}

pr::result_vector BayesianClassifier::performTest(QStringList files)
{
    pr::result_vector results;
    for(int i = 0; i < files.size(); i++) {
        pr::TestResult tr;
        tr = isPositive(files.at(i));
        cout << tr.fileName.toStdString() << "," << tr.q << endl;
        results.push_back(tr);
    }

    return results;
}

pr::TestResult BayesianClassifier::isPositive(QString file)
{
    cv::Mat sample = cv::imread(file.toStdString(), CV_LOAD_IMAGE_ANYDEPTH
                                | CV_LOAD_IMAGE_ANYCOLOR);

    //convert sample to a single row Mat
    cv::Mat sampleAsRow = cv::Mat(1, 648, CV_32FC1);
    int i,j, k = 0;
    for(i = 0; i < mSize.height; i++) {
        for(j = 0; j < mSize.width; j++) {
            sampleAsRow.at<pr::MY_FLOAT>(0, k) = (pr::MY_FLOAT)sample.at<uchar>(i,j);
            k++;
        }
    }

    //substract tmp(sample as single row) from mean of samples
    cv::Mat sub;
    cv::transpose((sampleAsRow - mMean), sub);

    cv::Mat w = cv::Mat(sub.rows, sub.cols, CV_32FC1);

    w = mTEigenVector * sub;

    cv::Mat wSquared(w);
    for(i = 0; i < w.rows; i++) {
        pr::MY_FLOAT val = w.at<pr::MY_FLOAT>(0,i);
        val *= val;
        w.at<pr::MY_FLOAT>(0,i) = val;
    }

    pr::MY_FLOAT q = 0;
    for(i = 0; i < wSquared.rows; i++) {
        q += wSquared.at<pr::MY_FLOAT>(0, i)
                / mEigenValues.at<pr::MY_FLOAT>(0,i);
    }


    pr::TestResult result;
    result.fileName = file;
    result.q = q;
    result.result = false;
    //TODO must of the calculation can be converted to constants!!!!
//    result.criterion =
//            (-0.5) * (mEigenValues.rows * mEigenValues.cols)
//            * 1.83787706641 - 0.5 * mLnDet - 0.5 * (double)q;

    result.criterion = 0;

    //ln(2*pi) = 1.83787706641

    return result;
}

void BayesianClassifier::calculateLnOfDetOfCovar()
{
    mLnDet = 0;

    for(int i = 0; i < mEigenValues.rows; i++) {
        mLnDet += log(mEigenValues.at<pr::MY_FLOAT>(0,i));
    }

    cout << "LN det of eigenVals => " << mLnDet << endl;
}
