#ifndef PATREC_TYPES_H
#define PATREC_TYPES_H

#include <QString>
#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


/*
Contains data types and static functions for the project.

functions are implemented in helpers.cpp
*/



//using namespace so we can access enums safely!
namespace pr {

/*  TYPES   */
typedef std::vector<uchar> pixel_vector; //result from pixel values of image
typedef std::vector<pixel_vector> training_vector; //collection of pixel_vector

typedef std::vector<double> double_vector; //for bayeian mean


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
    BAYESIAN,
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


/**
 * @brief showSingleImage
 * @param file
 */
void showSingleImage(QString file);


void gaussFilter(cv::Mat *src, cv::Mat *dst, cv::Size kernelSize);

}
#endif // TRAINER_TYPE_H
