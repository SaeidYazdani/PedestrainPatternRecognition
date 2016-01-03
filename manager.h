#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>
#include <QtQml>
#include <QUrl>

#include "patrec_types.h"
#include "trainer.h"
#include "arffgenerator.h"
#include "bayesianclassifier.h"

class Manager : public QObject
{
    Q_OBJECT

public:
    explicit Manager(QObject *parent = 0);


    pr::WorkState state() const {return mState;}

    int numberOfImagesToTrain() const;

    pr::TrainingFilters filters() const;
    pr::TrainingMethod method() const;

    QString convertToFilePath(QUrl url, QString name);

    pr::SizeMode sizeMode() const;

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
    pr::WorkState mState;

    //folders, files, lists
    QUrl    mPositiveDataPath;
    QUrl    mNegativeDataPath;
    QUrl    mOutputPath;
    QString mOutputFileName;

    QStringList mPositiveFilesList;
    QStringList mNegativeFilesList;

    //options
    pr::TrainingFilters mFilters;
    pr::TrainingMethod mMethod;
    pr::SizeMode mSizeMode;
    int mNumberOfImagesToTrain;



    //functions


    QStringList generateFileList(pr::TrainingType t);
};

#endif // MANAGER_H
