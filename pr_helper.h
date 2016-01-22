#ifndef PR_HELPER_H
#define PR_HELPER_H

#include <stdio.h>
#include <iostream>
#include <vector>

#include <QTime>
#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QString>
#include <QTextStream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define PI 3.14159265359
#define PI_BY_2 1.57079632679

//FOR HOG
#define HOG_CHANNEL 72 //72
#define HOG_BEAN    5  //5


/**
 * Contains types and helper functions for pattern recognition!
 */
namespace pr {

/*  TYPES   */
//previously it was named KIRE_KHAR_TYPE and we had to rename it because
//it was too long!
typedef float MY_FLOAT; //by ostad to be easily chaned later if necessary
typedef std::vector<MY_FLOAT> pixel_vector; //result from pixel values of image
typedef std::vector<pixel_vector> training_vector; //collection of pixel_vector
typedef std::vector<double> double_vector; //for bayeian mean

/*  STRUCTS */
struct NegativeFolder {
    QString name;
    int count;
    QStringList *filesList;
};

/**
 * @brief Contains result of a test in classifiers
 */
struct TestResult {
    /**
     * @brief The image file full path and name of the image
     */
    QString fileName;
    /**
     * @brief Type of the used feature vector
     */
    QString featureVectorName;
    /**
     * @brief q
     */
    pr::MY_FLOAT q;
    /**
     * @brief Criterion
     */
    double criterion;
    /**
     * @brief result is/was positive or negative?
     */
    bool result;
    //TODO add a field to contain name of the class which it was most propable
};

typedef std::vector<NegativeFolder> folder_vector;
typedef std::vector<TestResult> result_vector;



/**
 * @brief The TrainingType enum
 */
enum TrainingType {
    POSITIVE,
    NEGATIVE
};

/**
 * @brief The TrainingMethod enum
 */
enum TrainingMethod {
    GRAYSCALE,
    HOG,
    EXTRA
};

/**
 * @brief The TrainingFilters enum
 * @note This is flagged enum!
 */
enum TrainingFilters {
    NONE    = 0,
    GAUSS   = 1,
    SOBEL   = 2,
    FEATURE = 4
};

/**
 * @brief The SizeMode enum
 */
enum SizeMode {
    RESIZE,
    WINDOW
};

/**
 * @brief The WorkState enum
 */
enum WorkState {
    IDLE,
    WORKING,
    DONE
};

/**
 * @brief The FileType enum. To specify type of ARFF file.
 */
enum FileType {
    COMPLETE, //positive and negative values
    POSITIVE_ONLY,
    NEGATIVE_ONLY
};

        /*  SOME STATIC METHODS!    */

//enum related methods
/**
 * @brief getFileTypeEnumAsQString
 * @param fileType
 * @return
 */
QString getFileTypeEnumAsQString(pr::FileType fileType);

QString getTrainingMethodAsString(pr::TrainingMethod tm);


/**
 * @brief getImageType
 * @param file
 * @return
 */
int getImageType(QString file);

/**
 * @brief getImageSize
 * @param file
 * @return QPoint
 */
cv::Size getImageSize(QString file);

/**
 * @brief convertMeanVectorToImageAndShow
 * @param size
 * @param type
 * @param vector
 * @param windowName
 */
void convertVectorToImageAndShow(cv::Size size, int type
                                     , pr::double_vector *vector
                                     , QString windowName);

void convertVectorToImageAndShow(cv::Size size, int type
                                     , pr::pixel_vector *vector
                                     , QString windowName);


/**
 * @brief showSingleImage
 * @param file
 */
void showSingleImage(QString file);

/**
 * @brief showSingleImage
 * @param windowName name of the window
 * @param mat pointer of a Mat
 */
void showSingleImage(QString windowName, cv::Mat mat);

void gaussFilter(cv::Mat *src, cv::Mat *dst, cv::Size kernelSize);

bool saveResultVectorAsCVS(result_vector *rv, QString path, QString name
                           , bool writeFullName = false);

}
#endif // PR_HELPER_H
