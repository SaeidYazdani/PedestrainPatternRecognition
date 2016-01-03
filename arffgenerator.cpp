#include "arffgenerator.h"

ArffGenerator::ArffGenerator()
{

}

pr::training_vector *ArffGenerator::posVector() const
{
    return mPosVector;
}

void ArffGenerator::setPosVector(pr::training_vector *posVector)
{
    mPosVector = posVector;
}

pr::training_vector *ArffGenerator::negVector() const
{
    return mNegVector;
}

void ArffGenerator::setNegVector(pr::training_vector *negVector)
{
    mNegVector = negVector;
}

QString ArffGenerator::path() const
{
    return mPath;
}

void ArffGenerator::setPath(const QString &path)
{
    mPath = path;
}

cv::Size ArffGenerator::imageSize() const
{
    return mImageSize;
}

void ArffGenerator::setImageSize(const cv::Size &imageSize)
{
    mImageSize = imageSize;
}

bool ArffGenerator::generate()
{
    QFile file(mPath);
    //open file handle and ensure create and overwrite access
    file.open(QIODevice::WriteOnly | QIODevice::Truncate);

    //check if file has been opened
    if(!file.isOpen()) {
        qDebug() << "Error opening " << mPath << " for writing!";
        return false;
    }

    QTextStream out(&file);

    //hedaer
    generateHeader(out);
    //positive data
    generatePositiveData(out);
    //negative data
    generateNegativeData(out);

    //flush the stream and close the file
    out.flush();
    file.close();

    return true;
}


void ArffGenerator::generateHeader(QTextStream &out)
{
    out << "@RELATION pedestrain\n"; //first line of the file

    //attributes -> pixel_row_col e.g. pixel_0_11 is pixel value @row 0, col 11
    int i,j;

    for(i = 0; i < mImageSize.height; i++) {
        for(j = 0; j < mImageSize.width; j++) {
            out << "@ATTRIBUTE " << "pixel_" << i << "_" << j << " NUMERIC\n";
        }
    }

    //Classes
    out << "@ATTRIBUTE Class {POSITIVE,NEGATIVE}\n";

    //Data start
    out << "@DATA\n";
}

void ArffGenerator::generatePositiveData(QTextStream &out)
{
    unsigned int i,j;

    for(i = 0; i < mPosVector->size(); i++) {

        pr::pixel_vector pv = mPosVector->at(i);

        for(j = 0; j < pv.size(); j++) {
            out << pv.at(j) << ",";
        }

        out << "POSITIVE\n";
    }
}

void ArffGenerator::generateNegativeData(QTextStream &out)
{
    unsigned int i,j;

    for(i = 0; i < mNegVector->size(); i++) {

        pr::pixel_vector pv = mNegVector->at(i);

        for(j = 0; j < pv.size(); j++) {
            out << pv.at(j) << ",";
        }

        out << "NEGATIVE\n";
    }
}

