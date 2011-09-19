#include <QtGui>
#include "mainwidget.h"
#include "webwidget.h"
#include "runclass.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QThreadPool *threadPool = QThreadPool::globalInstance();
    RunClass r;
    r.setAutoDelete(false);
    DataClass d;
    r.colectData(&d);
    MainWidget m(0, &r, threadPool, &d);
    WebWidget w;
    QObject::connect(&m, SIGNAL(showWebWidget(QString)), &w, SLOT(showWidget(QString)));
    QObject::connect(&r, SIGNAL(destroyTrees()), &d, SLOT(destroyTrees()));
    QObject::connect(&r, SIGNAL(boxDone()), &m, SLOT(getBoxDone()));
    m.show();

    return app.exec();
}
