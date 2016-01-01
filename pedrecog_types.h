#ifndef TRAINER_TYPE_H
#define TRAINER_TYPE_H


//using namespace so we can access enums safely!
namespace PedRec {

/*  TYPES   */
typedef std::vector<uchar> pixel_vector; //result from pixel values of image
typedef std::vector<pixel_vector> training_vector; //collection of pixel_vector

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
    HAG,
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

}
#endif // TRAINER_TYPE_H
