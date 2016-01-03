#include "patrec_types.h"

#include <QDebug>

namespace pr {


cv::Size getImageSize(QString file)
{
    return cv::imread(file.toStdString()).size();
}


int getImageType(QString file)
{
    return cv::imread(file.toStdString()).type();
}

void convertMeanVectorToImageAndShow(cv::Size size, int type
                                     , pr::double_vector *vector
                                     , QString windowName) {

    //NOTE type of the benchmark photos are "16"
    qDebug() << "Converting MeanVector to Mat of size w=" << size.width
             << " h=" << size.height << " and type=" << type
             << "  Vector size=" << vector->size();

    //first convert from double to uchar
    uchar pv[vector->size()];
    for(unsigned int i = 0; i < vector->size(); i++) {
        pv[i] = (uchar) vector->at(i);
    }

    //create Matrix
    cv::Mat out(size, CV_8UC1);
    memcpy(out.data, &pv, vector->size()*sizeof(uchar));

    //show image in window
    cv::namedWindow(windowName.toStdString(), cv::WINDOW_AUTOSIZE);
    cv::imshow(windowName.toStdString(), out);
}

void showSingleImage(QString file) {
    cv::Mat img = cv::imread(file.toStdString());
    cv::imshow(file.toStdString(), img);
}

void gaussFilter(cv::Mat *src, cv::Mat *dst, cv::Size kernelSize) {
    //TODO implement this
}


}
