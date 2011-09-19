#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#undef __STRICT_ANSI__

#include <QtGui>
#include <QDate>
#include "user_def.h"
#include "runclass.h"


using namespace Qt;

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = 0, RunClass *runCl = 0, QThreadPool *tPool = 0, DataClass *dataCl = 0);

    RunClass *runClass;
    QThreadPool *threadPool;

signals:
    void showWebWidget(QString);

public slots:
    void loadInput();
    void Run();
    void ShowOutput();
    void getBoxDone();

public: //projít
    DataClass *data;

    //ui
    QLabel *lProjectName;
    QLineEdit *leProjectName;
    QLabel *lLoad;
    QPushButton *pbLoad;
    QLabel *lHeuristic;
    QSpinBox *sbHeuristic;
    QLabel *lMCvariant;
    QComboBox *cbMCvariant;
    QLabel *lMCsamples;
    QSpinBox *sbMCsamples;
    QLabel *lProjectStart;
    QDateEdit *deProjectStart;
    QProgressBar *pbProgress;
    QLabel *lRun;
    QPushButton *pbRun;
    QLabel *lShow;
    QPushButton *pbShow;
    QTextEdit *teNotes;

    QHBoxLayout *layProjectName;
    QHBoxLayout *layLoad;
    QHBoxLayout *layHeuristic;
    QHBoxLayout *layMCvariant;
    QHBoxLayout *layMCsamples;
    QHBoxLayout *layProjectStart;
    QHBoxLayout *layRun;
    QHBoxLayout *layShow;

    QVBoxLayout *layAll;
};

#endif // MAINWIDGET_H
