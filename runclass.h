#ifndef RUNCLASS_H
#define RUNCLASS_H

#include <QtGui>
#include <QRunnable>
#include "user_def.h"
#include "tree_of_admissible_processes.h"
#include "mainwidget.h"

struct DataClass : public QObject
{
    Q_OBJECT
public:
    QString strProjectName;
    QString inputData;
    string input;
    int heurPar;
    bool MCvar;
    int MCsamples;
    QDate startDate;
    QString notes;

    int *mc_study_dats;
    int mc_study_min;
    int mc_study_max;

    tree_of_admissible_processes *tree;
    tree_of_admissible_processes *MCtree;
public slots:
    void destroyTrees();
};

class RunClass : public QObject, public QRunnable
{
    Q_OBJECT
protected:
    void run();
public:
    void colectData(DataClass *dataCl);
    DataClass *data;
private:

    void calc();
    void MonteCarlo();
    void exportData();
signals:
    void changeProgress(int);
    void destroyTrees();
    void boxDone();
private slots:
    void getProgress(int value);
};

#endif // RUNCLASS_H
