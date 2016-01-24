#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>
#include <QtQml>
#include <QUrl>

#include "pr_helper.h"
#include "trainer.h"
#include "arffgenerator.h"
#include "statisticscalculator.h"
#include "bayesianclassifier.h"

/**
 * @brief The Manager class. This class manages the interaction between UI and
 * the logic of the application. Users set project properties like folders,
 * training type and other options and these will be stored in the manager class
 * and then the manager class will perform the logic to accomplish the project
 * based on the user input
 */
class Manager : public QObject
{
    Q_OBJECT

public:
    explicit Manager(QObject *parent = 0);
    int numberOfImagesToTrain() const;
    pr::WorkState state() const {return mState;}
    pr::TrainingFilters filters() const;
    pr::TrainingMethod method() const;
    QString convertToFilePath(QUrl url, QString name);
    pr::SizeMode sizeMode() const;
    QString outputFileName() const;
    pr::RoiRect roiRect() const;

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
    void setRoiRect(QString l, QString t, QString r, QString b);


private:

    //manager state
    //TODO this is useless...no time to implement stop procedure
    pr::WorkState mState;

    //folders, files, lists
    QUrl    mPositiveDataPath;
    QUrl    mNegativeDataPath;
    QUrl    mTestDataPath;
    QUrl    mOutputPath;
    QString mProjectName;

    QStringList mPositiveFilesList;
    QStringList mNegativeFilesList;
    QStringList mTestFilesList;

    //options
    pr::TrainingFilters mFilters;
    pr::TrainingMethod mMethod;
    pr::SizeMode mSizeMode;
    int mNumberOfImagesToTrain;
    pr::RoiRect mRoiRect;

    //functions
    QStringList generateFileList(pr::TrainingType t);
    QStringList generateTestFileList();
};

#endif // MANAGER_H
