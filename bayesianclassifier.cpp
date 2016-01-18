#include "bayesianclassifier.h"

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

        pr::KIRE_KHAR_TYPE *pInput = mat.ptr<pr::KIRE_KHAR_TYPE>(r);

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
            meanMatToShow.at<uchar>(i,j) = (uchar) mean.at<pr::KIRE_KHAR_TYPE>(0, k);
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
}

bool BayesianClassifier::isPositive(QString file)
{
    cv::Mat sample = cv::imread(file.toStdString(), CV_LOAD_IMAGE_ANYDEPTH
                                | CV_LOAD_IMAGE_ANYCOLOR);

    //convert sample to a single row Mat
    cv::Mat sampleAsRow = cv::Mat(1, 648, CV_32FC1);
    int i,j, k = 0;
    for(i = 0; i < mSize.height; i++) {
        for(j = 0; j < mSize.width; j++) {
            sampleAsRow.at<pr::KIRE_KHAR_TYPE>(0, k) = (pr::KIRE_KHAR_TYPE)sample.at<uchar>(i,j);
            k++;
        }
    }

    //substract tmp(sample as single row) from mean of samples
    cv::Mat sub;
    cv::transpose((sampleAsRow - mMean), sub);

#ifdef QT_DEBUG
    cout << "SUB TYPE = " << sub.type() << " SIZE " << sub.size()  << endl;
    cout << "SUBTRACTION RESULT\n" << sub << endl;
#endif

    cv::Mat w = cv::Mat(sub.rows, sub.cols, CV_32FC1);

#ifdef QT_DEBUG
    cout << "W = " << w.type() <<  " SIZE " << w.size()  << endl;
#endif

    w = mTEigenVector * sub;

#ifdef QT_DEBUG
    cout << "MULTIPLICATION RESULT\n" << w << endl;
#endif

    cv::Mat wSquared(w);
    for(i = 0; i < w.rows; i++) {
        pr::KIRE_KHAR_TYPE val = w.at<pr::KIRE_KHAR_TYPE>(0,i);
        val *= val;
        w.at<pr::KIRE_KHAR_TYPE>(0,i) = val;
    }

#ifdef QT_DEBUG
    cout << "SQUARE RESULT\n" << w << endl;
#endif

    return false; //hehehehehhe
}


