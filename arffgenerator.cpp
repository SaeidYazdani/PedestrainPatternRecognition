#include "arffgenerator.h"

ArffGenerator::ArffGenerator()
{

}

ArffGenerator::ArffGenerator(QString file) : mPath(file)
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

QString ArffGenerator::projectName() const
{
    return mProjectName;
}

void ArffGenerator::setProjectName(QString &projectName)
{
    mProjectName = projectName;
}


cv::Size ArffGenerator::imageSize() const
{
    return mImageSize;
}

void ArffGenerator::setImageSize(const cv::Size &imageSize)
{
    mImageSize = imageSize;
}

bool ArffGenerator::generateArff(pr::FileType fileType)
{

    QString path;

    switch(fileType) {

    case pr::FileType::COMPLETE:
        path = QString(mPath).append(QDir::separator())
                .append(mProjectName).append("_all.arff");
        break;

    case pr::FileType::POSITIVE_ONLY:
        path = QString(mPath).append(QDir::separator())
                .append(mProjectName).append("_positive.arff");
        break;

    case pr::FileType::NEGATIVE_ONLY:
        path = QString(mPath).append(QDir::separator())
                .append(mProjectName).append("_negative.arff");
        break;
    }

    QFile file(path);

    qDebug() << "Generating ARFF file"
             << file.fileName()
             << QTime::currentTime().toString();


    //open file handle and ensure create and overwrite access
    file.open(QIODevice::WriteOnly | QIODevice::Truncate);

    //check if file has been opened
    if(!file.isOpen()) {
        qDebug() << "Error opening " << mPath << " for writing!";
        return false;
    }

    QTextStream out(&file);

    //hedaer
    generateHeader(out, fileType);

    //actual data
    //Too lazy to convert FileType to a flag enum...lol
    //anyways we have only 3 cases so who caaaaaaaaaaares
    if(fileType == pr::FileType::COMPLETE) {
        generatePositiveData(out);
        generateNegativeData(out);
    } else {

        if(fileType == pr::FileType::POSITIVE_ONLY)
            generatePositiveData(out);

        if(fileType == pr::FileType::NEGATIVE_ONLY)
            generateNegativeData(out);
    }

    //flush the stream and close the file
    out.flush();
    file.close();

    qDebug() << "Arff file is written to disk"
             << QTime::currentTime().toString();

    return true;
}


void ArffGenerator::generateHeader(QTextStream &out, pr::FileType fileType)
{
    //first add some comments to the arff file :P

    out << "%\tPROJECT=" << mProjectName << "\n"
        << "%\tTYPE=" << pr::getFileTypeEnumAsQString(fileType) << "\n";

    out << "@RELATION pedestrain\n"; //first line of the file

    //attributes -> pixel_row_col e.g. pixel_0_11 is pixel value @row 0, col 11
    int i,j;

    for(i = 0; i < mImageSize.height; i++) {
        for(j = 0; j < mImageSize.width; j++) {
            out << "@ATTRIBUTE " << "pixel_" << i << "_" << j << " NUMERIC\n";
        }
    }

    //Classes
    switch (fileType) {
    case pr::FileType::COMPLETE:
        out << "@ATTRIBUTE Class {POSITIVE,NEGATIVE}\n";
        break;

    case pr::FileType::POSITIVE_ONLY:
        out << "@ATTRIBUTE Class {POSITIVE}\n";
        break;

    case pr::FileType::NEGATIVE_ONLY:
        out << "@ATTRIBUTE Class {NEGATIVE}\n";
        break;
    }

    //Data start
    out << "@DATA\n";
}

void ArffGenerator::generatePositiveData(QTextStream &out)
{
    unsigned int i,j;
    unsigned int vectorSize = mPosVector->size();
    unsigned int elementSize = mPosVector->at(0).size();

    for(i = 0; i < vectorSize; i++) {
        for(j = 0; j < elementSize; j++) {
            out << mPosVector->at(i).at(j) << ",";
        }
        out << "POSITIVE\n";
    }
}

void ArffGenerator::generateNegativeData(QTextStream &out)
{
    unsigned int i,j;
    unsigned int vectorSize = mNegVector->size();
    unsigned int elementSize = mNegVector->at(0).size();

    for(i = 0; i < vectorSize; i++) {
        for(j = 0; j < elementSize; j++) {
            out <<  mNegVector->at(i).at(j) << ",";
        }
        out << "NEGATIVE\n";
    }
}

bool ArffGenerator::saveSingleVector(QString name
                                     , pr::double_vector *vector
                                     , int fractions)
{
    QString path = QString(mPath).append(QDir::separator())
            .append(mProjectName).append("_" + name + ".vector");

    QFile file(path);

    qDebug() << "Generating VECTOR file for " << name << "in" << file.fileName()
             << QTime::currentTime().toString();


    //open file handle and ensure create and overwrite access
    file.open(QIODevice::WriteOnly | QIODevice::Truncate);

    //check if file has been opened
    if(!file.isOpen()) {
        qDebug() << "Error opening " << mPath << " for writing!";
        return false;
    }

    QTextStream out(&file);

    out << "%VECTOR FILE\n";

    for(unsigned int i; i < vector->size(); i++) {
        out << vector->at(i) << ",";
    }

    out <<"END\n";

    out.flush();
    file.close();

    qDebug() << "Generating VECTOR is completed"
             << QTime::currentTime().toString();

    return true;
}
