#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>
#include <QtQml>
#include <QUrl>

#include "pedrecog_types.h"
#include "trainer.h"
#include "arffgenerator.h"

class Manager : public QObject
{
    Q_OBJECT

public:
    explicit Manager(QObject *parent = 0);


    PedRec::WorkState state() const {return mState;}

    int numberOfImagesToTrain() const;

    PedRec::TrainingFilters filters() const;
    PedRec::TrainingMethod method() const;

    QString convertToFilePath(QUrl url, QString name);

    PedRec::SizeMode sizeMode() const;

    QString outputFileName() const;

signals:


public slots:
    bool start();
    void stop();
    bool checkDataFolder(QUrl posPath, int whichFolder);
    bool checkOutputFolder(QUrl outPath);
    void setNumberOfImagesToTrain(QString num);
    void setMethod(int method);
    void setFilters(int filters);
    void setSizeMode(int mode);
    void setOutputFileName(QString name);


private:

    //manager state
    //TODO this is useless...no time to implement stop procedure
    PedRec::WorkState mState;

    //folders, files, lists
    QUrl    mPositiveDataPath;
    QUrl    mNegativeDataPath;
    QUrl    mOutputPath;
    QString mOutputFileName;

    QStringList mPositiveFilesList;
    QStringList mNegativeFilesList;

    //options
    PedRec::TrainingFilters mFilters;
    PedRec::TrainingMethod mMethod;
    PedRec::SizeMode mSizeMode;
    int mNumberOfImagesToTrain;



    //functions


    QStringList generateFileList(PedRec::TrainingType t);
};

#endif // MANAGER_H
