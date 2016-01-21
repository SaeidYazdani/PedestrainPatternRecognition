#include "pr_helper.h"


namespace pr {

QString getFileTypeEnumAsQString(pr::FileType fileType) {

    switch(fileType) {

    case pr::FileType::COMPLETE:
        return QString("COMPLETE");

    case pr::FileType::POSITIVE_ONLY:
        return QString("POSITIVE_ONLY");

    case pr::FileType::NEGATIVE_ONLY:
        return QString("NEGATIVE_ONLY");

    default:
        return QString("UNSPECIFIED");
    }
}


cv::Size getImageSize(QString file)
{
    return cv::imread(file.toStdString()).size();
}


int getImageType(QString file)
{
    return cv::imread(file.toStdString()).type();
}

void convertVectorToImageAndShow(cv::Size size, int type
                                 , pr::double_vector *vector
                                 , QString windowName) {

    //NOTE type of the benchmark photos are "16"
    qDebug() << "Converting MeanVector to Mat of size w=" << size.width
             << " h=" << size.height << " and type=" << type
             << "  Vector size=" << vector->size();

    //building a Mat object from vector pointer
    cv::Mat out = cv::Mat(*vector).reshape(0, size.height);
    out.convertTo(out, CV_8UC1);

    //show image in window
    cv::namedWindow(windowName.toStdString(), cv::WINDOW_NORMAL);
    cv::imshow(windowName.toStdString(), out);
}

void convertVectorToImageAndShow(cv::Size size, int type
                                 , pr::pixel_vector *vector
                                 , QString windowName) {

    //NOTE type of the benchmark photos are "16"
    qDebug() << "Converting MeanVector to Mat of size w=" << size.width
             << " h=" << size.height << " and type=" << type
             << "  Vector size=" << vector->size();

    //building a Mat object from vector pointer
    cv::Mat out = cv::Mat(*vector).reshape(0, size.height);
    out.convertTo(out, CV_8UC1);

    //show image in window
    cv::namedWindow(windowName.toStdString(), cv::WINDOW_NORMAL);
    cv::imshow(windowName.toStdString(), out);
}

void showSingleImage(QString file) {
    cv::Mat img = cv::imread(file.toStdString());
    cv::imshow(file.toStdString(), img);
}

void showSingleImage(QString windowName, cv::Mat mat) {

    qDebug() << "Showing " << windowName;

    if(mat.type() != CV_8UC1) {
        std::cout << "Conversion to CV_8UC1 is necessary for" << windowName.toStdString()
                  << "Correct display is not guaranteed!" << std::endl;
    }

    cv::namedWindow(windowName.toStdString(), cv::WINDOW_NORMAL);
    cv::imshow(windowName.toStdString(), mat);
}

void gaussFilter(cv::Mat *src, cv::Mat *dst, cv::Size kernelSize) {
    //TODO implement this
}


bool saveResultVectorAsCVS(result_vector *rv, QString path, QString name
                           , bool writeFullPath) {

    QFile file(path.append(QDir::separator()).append(name).append(".cvs"));

    file.open(QIODevice::WriteOnly | QIODevice::Truncate);

    //check if file has been opened
    if(!file.isOpen()) {
        qDebug() << "Error opening " << name << " for writing!";
        return false;
    }

    QTextStream out(&file);
    pr::TestResult tr;
    unsigned int i;

    if(writeFullPath) {
        for(i = 0; i < rv->size(); i++) {
            tr = rv->at(i);

            out << i+1 << "," << tr.fileName << "," << tr.q << ","
                << tr.result << "\n";
        }
    } else { //just to save branch instructions :D :D :D :D :O
        for(i = 0; i < rv->size(); i++) {
            tr = rv->at(i);
            out << QFileInfo(tr.fileName).fileName()
                << "," << tr.q << ","
                << "," << tr.criterion << ","
                << tr.result << "\n";
        }
    }

    out.flush();
    file.close();

    return true;
}

}
