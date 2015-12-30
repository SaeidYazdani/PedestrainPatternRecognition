#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>
#include <QUrl>

class Manager : public QObject
{
    Q_OBJECT

    Q_ENUMS(WorkState)

public:
    explicit Manager(QObject *parent = 0);
    enum WorkState {IDLE, WORKING, DONE};

    WorkState state() const {return mState;}

signals:


public slots:
    void start(QString posPath, QString negPath, QString outputPath, int numToTrain);
    void stop();
    bool checkDataFolder(QUrl posPath);
    bool checkOutputFolder(QUrl outPath);


private:
    WorkState mState;
    QUrl    mPositiveDataPath;
    QUrl    mNegativeDataPath;
    QUrl    mOutputPath;
    int     mNumberToTrain;
};

#endif // MANAGER_H
