#include "bayesianclassifier.h"


/*
        C1	C2	C3	C4
CV_8U	0	8	16	24
CV_8S	1	9	17	25
CV_16U	2	10	18	26
CV_16S	3	11	19	27
CV_32S	4	12	20	28
CV_32F	5	13	21	29
CV_64F	6	14	22	30

Unsigned 8bits uchar 0~255
IplImage: IPL_DEPTH_8U
Mat: CV_8UC1, CV_8UC2, CV_8UC3, CV_8UC4

Signed 8bits char -128~127
IplImage: IPL_DEPTH_8S
Mat: CV_8SC1，CV_8SC2，CV_8SC3，CV_8SC4

Unsigned 16bits ushort 0~65535
IplImage: IPL_DEPTH_16U
Mat: CV_16UC1，CV_16UC2，CV_16UC3，CV_16UC4

Signed 16bits short -32768~32767
IplImage: IPL_DEPTH_16S
Mat: CV_16SC1，CV_16SC2，CV_16SC3，CV_16SC4

Signed 32bits int -2147483648~2147483647
IplImage: IPL_DEPTH_32S
Mat: CV_32SC1，CV_32SC2，CV_32SC3，CV_32SC4

Float 32bits float -1.18*10-38~3.40*10-38
IplImage: IPL_DEPTH_32F
Mat: CV_32FC1，CV_32FC2，CV_32FC3，CV_32FC4

Double 64bits double
Mat: CV_64FC1，CV_64FC2，CV_64FC3，CV_64FC4

Unsigned 1bit bool
IplImage: IPL_DEPTH_1U

 */


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

    //calculation of mean and covariance
    cv::Mat covar, mean;
    cv::calcCovarMatrix(mData, covar, mean
                        , CV_COVAR_NORMAL | CV_COVAR_ROWS, CV_8UC1);

    //THESE WILL BE SIGNED FLOAT!!!!!!!!
    mCovariance = covar;
    mMean = mean;

    //SHOW MEAN as image
    cv::Mat meanMatToShow = cv::Mat(mSize.height, mSize.width, CV_8UC1);

    int i,j, k = 0;
    for(i = 0; i < mSize.height; i++) {
        for(j = 0; j < mSize.width; j++) {
            meanMatToShow.at<uchar>(i,j) = (uchar) mean.at<float>(0, k);
            k++;
        }
    }
    pr::showSingleImage(mClassName +  " => MEAN OF SAMPLES", meanMatToShow);

    //SHOW COVAR as image
    cv::Mat covarNorm;
    //cout << mCovariance << endl;
    cv::normalize(mCovariance, covarNorm, 0, 255, NORM_MINMAX, CV_8UC1);
    //cout << covarNorm << endl;
    pr::showSingleImage(mClassName +  " => NORMALIZED COVARIANCE OF SAMPLES", covarNorm);

    //Eigenvalues calculations
    cv::Mat eigenvals, eigenvector;
    cv::eigen(mCovariance, eigenvals, eigenvector);

    mEigenValues = eigenvals;
    mEigenVector = eigenvector; //B

    //    cv::Mat eigenValsNorm;
    //    cv::normalize(eigenvector, eigenValsNorm, 0, 255, NORM_MINMAX, CV_8UC1);
    //    pr::showSingleImage(mClassName + " => NORMALIZED EIGEN VECTOR", eigenValsNorm);

    //cout << "EIGEN VALUES\t=>\n" << eigenvals << endl;
    //cout << "EIGEN VECTOR\t=>\n" << eigenvector << endl;

    cv::Mat eigenVecTrans;

    cv::transpose(mEigenVector, eigenVecTrans);
    mTEigenVector = eigenVecTrans; //BT

    //cout << "EIGEN VECTOR TRANSPOSED\t=>\n" << eigenVecTrans << endl;

}

bool BayesianClassifier::isPositive(QString file)
{
    cv::Mat sample = cv::imread(file.toStdString(), CV_LOAD_IMAGE_ANYDEPTH
                                | CV_LOAD_IMAGE_ANYCOLOR);

    switch(sample.type()) {

    case CV_8UC1:
        cout << "CV_8UC1" << endl;
    break;

    case CV_8UC2:
        cout << "CV_8UC2" << endl;
    break;

    case CV_8UC3:
        cout << "CV_8UC3" << endl;
    break;

    default:
        cout << "I DONT KNOW SHIT!" << endl;
    break;

    }


    switch(mMean.type()) {

    case CV_8UC1:
        cout << "CV_8UC1" << endl;
    break;

    case CV_8UC2:
        cout << "CV_8UC2" << endl;
    break;

    case CV_8UC3:
        cout << "CV_8UC3" << endl;
    break;

    case CV_16UC1:
        cout << "CV_16UC1" << endl;
    break;

    case CV_16UC2:
        cout << "CV_16UC2" << endl;
    break;

    case CV_16UC3:
        cout << "CV_16UC3" << endl;
    break;

    default:
        cout << "I DONT KNOW SHIT! search me => " << mMean.type() << endl;
    break;

    }

    cv::Mat tmp = cv::Mat(1, 648, sample.type());
    int i,j, k, l = 0;
    for(i = 0; i < mSize.height; i++) {
        for(j = 0; j < mSize.width; j++) {
            tmp.at<uchar>(0, k) = sample.at<uchar>(i,j);
            k++;
        }
    }

    cv::Mat mean(mMean);
    mean.convertTo(mean, sample.type());

    cout << "MEAN\n" << mMean << endl;
    cout << "SAMPLE\n" << sample << endl;
    cout << "TMP\n" << tmp << endl;

    cv::Mat sub = tmp - mean;

    cout << "SUBTRACTION RESULT\n" << sub << endl;

    return false; //hehehehehhe
}


