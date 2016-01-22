#include "bayesianclassifier.h"

#include <math.h>

using namespace cv;
using namespace std;

BayesianClassifier::BayesianClassifier(QString className
                                       , pr::TrainingMethod method)
{
    mClassName = className;
    mTrainingMethod = method;
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

pr::TrainingMethod BayesianClassifier::trainingMethod() const
{
    return mTrainingMethod;
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


    //TODO move this to manager
    //show mean and covar only in grayscale mode
    if(mTrainingMethod == pr::TrainingMethod::GRAYSCALE) {
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
    }

    //Eigenvalues calculations
    cv::Mat eigenvals, eigenvector;
    cv::eigen(mCovariance, eigenvals, eigenvector);

    mEigenValues = eigenvals;
    mEigenVector = eigenvector; //B

    cv::Mat eigenVecTrans;

    cv::transpose(mEigenVector, eigenVecTrans);
    mTEigenVector = eigenVecTrans; //BT

    cout << "MEAN FOR OSTAD\n" << mean << endl;
}


//TODO this should be changes so that an already made feature vector
//to be passed instead of list of files.....idiot!
pr::result_vector BayesianClassifier::performTest(QStringList files)
{
    pr::result_vector results;
    for(int i = 0; i < files.size(); i++) {
        pr::TestResult tr;
        switch(mTrainingMethod) {
        case pr::GRAYSCALE:
            tr = isPositiveGrayScale(files.at(i));
            break;
        case pr::HOG:
            tr = isPositiveHog(files.at(i));
            break;
        case pr::EXTRA:
            //NOT IMPLEMENTATIOOOOON
            break;

        }
        cout << tr.fileName.toStdString() << "," << tr.q << endl;
        results.push_back(tr);
    }

    return results;
}

//TODO kesafat kari kardim we should do something to clean this mess up
pr::TestResult BayesianClassifier::isPositiveHog(QString file) {

    cv::Mat mat = cv::imread(file.toStdString(), CV_LOAD_IMAGE_ANYDEPTH
                             | CV_LOAD_IMAGE_ANYCOLOR);

    int rows = mat.rows;
    int cols = mat.cols;

    //perform X and Y sobel
    cv::Mat dstX = cv::Mat(rows, cols, CV_32FC1);
    cv::Mat dstY = cv::Mat(rows, cols, CV_32FC1);


    cv::Mat matFlaot; //TODO do directly to reduce memory footprint
    mat.convertTo(matFlaot, CV_32FC1);

    cv::Sobel(matFlaot, dstX, -1, 1, 0, 3); //X
    cv::Sobel(matFlaot, dstY, -1, 0, 1, 3); //Y

    //check if everything is continues in memory
    if (mat.isContinuous() && dstX.isContinuous()
            && dstY.isContinuous() && matFlaot.isContinuous()) {
        cols = rows*cols;
        rows = 1;
    }

    pr::pixel_vector pv;

    //get feature degrees
    for (int r = 0; r < rows; ++r) {

        //pointer for pixels
        const pr::MY_FLOAT *xInput = dstX.ptr<pr::MY_FLOAT>(r);
        const pr::MY_FLOAT *yInput = dstY.ptr<pr::MY_FLOAT>(r);

        for (int c = 0; c < cols; ++c) {

            pv.push_back((pr::MY_FLOAT)atan2(*yInput, *xInput)
                         * ((pr::MY_FLOAT)180/PI));
            ++xInput;
            ++yInput;
        }
    }

    //initializing hog for 0 to 360
    unsigned int initHog[360] = {0}; //important to be inited with 0!!!

    //increment occuring degrees
    for(unsigned int i = 0; i < pv.size(); i++) {
        initHog[180 + (int)pv.at(i)]++;
    }

    pr::pixel_vector finalHog(36, 0);

    //TODO convert to constants to save cycles
    for(int i = 0; i < 36; i++) {
        for(int j = 0; j < 10; j++) {
            finalHog.at(i) += initHog[i*10+j];
        }
    }
    //done with hog

    //convert hog to a single row Mat
    cv::Mat sampleAsRow = cv::Mat(1, finalHog.size(), CV_32FC1);
    int i= 0;

    for(i = 0; i < finalHog.size(); i++) {
        sampleAsRow.at<pr::MY_FLOAT>(0, i)  = finalHog.at(i);
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
    result.featureVectorName = pr::getTrainingMethodAsString(mTrainingMethod);
    result.fileName = file;
    result.q = q;
    result.result = false;
    result.criterion = 0;
    return result;
}















//TODO this does not work with bigger images!!!
pr::TestResult BayesianClassifier::isPositiveGrayScale(QString file)
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
    result.featureVectorName = pr::getTrainingMethodAsString(mTrainingMethod);
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
