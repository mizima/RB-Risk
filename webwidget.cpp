#include "webwidget.h"

WebWidget::WebWidget(QWidget *parent)
{
    view = new QWebView();
    view->resize(1060,600);
    view->setWindowTitle("Result");
}


void WebWidget::showWidget(QString file)
{
    //view->load(QUrl("http://www.google.com"));
    view->load(QUrl::fromLocalFile(file));
    view->show();
}
