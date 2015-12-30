#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>

class manager : public QObject
{
    Q_OBJECT

    Q_ENUMS(WorkState)

public:
    explicit manager(QObject *parent = 0);
    enum WorkState {IDLE, WORKING, DONE};

    WorkState state() const {return mState;}

signals:


public slots:
    void start(QString posPath, QString negPath, QString outputPath, int numToTrain);
    void stop();


private:
    WorkState mState;
    QString mPositiveDataPath;
    QString mNegativeDataPath;
    int     mNumberToTrain;
};

#endif // MANAGER_H
