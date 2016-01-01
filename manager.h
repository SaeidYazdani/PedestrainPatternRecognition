#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>
#include <QtQml>
#include <QUrl>

#include "trainer.h"
#include "training_type.h"

class Manager : public QObject
{
    Q_OBJECT

public:
    explicit Manager(QObject *parent = 0);


    PedRecog::WorkState state() const {return mState;}

    int numberOfImagesToTrain() const;

    PedRecog::TrainingFilters filters() const;
    PedRecog::TrainingMethod method() const;

    QString convertToFilePath(QUrl url, QString name);

signals:


public slots:
    bool start();
    void stop();
    bool checkDataFolder(QUrl posPath, int whichFolder);
    bool checkOutputFolder(QUrl outPath);
    void setNumberOfImagesToTrain(QString num);
    void setMethod(int method);
    void setFilters(int filters);


private:

    //manager sate
    PedRecog::WorkState mState;

    //folders, files, lists
    QUrl    mPositiveDataPath;
    QUrl    mNegativeDataPath;
    QUrl    mOutputPath;
    QStringList mPositiveFilesList;
    QStringList mNegativeFilesList;

    //options
    PedRecog::TrainingFilters mFilters;
    PedRecog::TrainingMethod mMethod;
    int mNumberOfImagesToTrain;



    //functions


    QStringList generateFileList(PedRecog::TrainingType t);
};

#endif // MANAGER_H
