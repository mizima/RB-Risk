#include "mainwidget.h"
#include "runclass.h"

MainWidget::MainWidget(QWidget *parent, RunClass *runCl, QThreadPool *tPool, DataClass *dataCl)
{
    lProjectName = new QLabel("Project Name");
    leProjectName = new QLineEdit();
    leProjectName->setText("default");
    leProjectName->setAlignment(AlignHCenter);
    lLoad = new QLabel("Load input");
    pbLoad = new QPushButton("Load");
    lHeuristic = new QLabel("Heuristic parameter");
    sbHeuristic = new QSpinBox();
    sbHeuristic->setAlignment(AlignRight);
    lMCvariant = new QLabel("MC variant");
    cbMCvariant = new QComboBox();
    cbMCvariant->addItem("ASAP");
    cbMCvariant->addItem("Fixed time");
    lMCsamples = new QLabel("MC samples");
    sbMCsamples = new QSpinBox();
    sbMCsamples->setAlignment(AlignRight);
    sbMCsamples->setMaximum(1000000000);
    sbMCsamples->setValue(100);
    lProjectStart = new QLabel("Project starting date");
    deProjectStart = new QDateEdit();
    QDate *curDate = new QDate;
    deProjectStart->setDate(curDate->currentDate());
    deProjectStart->setAlignment(AlignRight);
    pbProgress = new QProgressBar();
    lRun = new QLabel("Run");
    pbRun = new QPushButton("Run");
    lShow = new QLabel("Show results");
    pbShow = new QPushButton("Show");
    teNotes = new QTextEdit();
    teNotes->setText("Notes");

    layProjectName = new QHBoxLayout();
    layLoad = new QHBoxLayout();
    layHeuristic = new QHBoxLayout();
    layMCvariant = new QHBoxLayout();
    layMCsamples = new QHBoxLayout();
    layProjectStart = new QHBoxLayout();
    layRun = new QHBoxLayout();
    layShow = new QHBoxLayout();

    layAll = new QVBoxLayout();

    layProjectName->addWidget(lProjectName);
    layProjectName->addWidget(leProjectName);
    layLoad->addWidget(lLoad);
    layLoad->addWidget(pbLoad);
    layHeuristic->addWidget(lHeuristic);
    layHeuristic->addWidget(sbHeuristic);
    layMCvariant->addWidget(lMCvariant);
    layMCvariant->addWidget(cbMCvariant);
    layMCsamples->addWidget(lMCsamples);
    layMCsamples->addWidget(sbMCsamples);
    layProjectStart->addWidget(lProjectStart);
    layProjectStart->addWidget(deProjectStart);
    layRun->addWidget(lRun);
    layRun->addWidget(pbRun);
    layShow->addWidget(lShow);
    layShow->addWidget(pbShow);

    layAll->addLayout(layProjectName);
    layAll->addLayout(layLoad);
    layAll->addLayout(layHeuristic);
    layAll->addLayout(layMCvariant);
    layAll->addLayout(layMCsamples);
    layAll->addLayout(layProjectStart);
    layAll->addLayout(layRun);
    layAll->addWidget(pbProgress);
    layAll->addLayout(layShow);
    layAll->addWidget(teNotes);

    resize(250,350);
    setLayout(layAll);
    setWindowTitle("BM risk prediction");

    connect(pbLoad, SIGNAL(clicked()), this, SLOT(loadInput()));
    connect(pbRun, SIGNAL(clicked()), this, SLOT(Run()));
    connect(pbShow, SIGNAL(clicked()), this, SLOT(ShowOutput()));

    data = dataCl;

    data->strProjectName = "default";
    data->inputData = "";
    data->input = data->inputData.toStdString();
    data->heurPar = 0;
    data->MCvar = 0;
    data->MCsamples = 100;
    data->startDate = curDate->currentDate();
    data->notes = "";
    data->mc_study_max = 0;
    data->mc_study_min = 0;

    runClass = runCl;
    threadPool = tPool;
    connect(runClass, SIGNAL(changeProgress(int)), pbProgress, SLOT(setValue(int)));
}

void MainWidget::loadInput()
{
    data->inputData = QFileDialog::getOpenFileName(this, "Open input file", "", "Formated text file (*.txt)");
    data->input = data->inputData.toStdString();
}

void MainWidget::Run()
{
    data->strProjectName = leProjectName->text();
    data->heurPar = sbHeuristic->value();
    data->MCvar = cbMCvariant->currentIndex();
    data->MCsamples = sbMCsamples->value();
    data->startDate = deProjectStart->date();
    data->notes = teNotes->toPlainText();

    threadPool->start(runClass);

}

void MainWidget::ShowOutput()
{
    QString helpString;
    helpString += "projects\\" + leProjectName->text() + "\\output.html";
    QDir dir;
    dir.setPath(helpString);
    dir.makeAbsolute();
    helpString = dir.path();

    emit showWebWidget(helpString);
}

void MainWidget::getBoxDone()
{
    QMessageBox::information(this, "Status", "Done");
}
