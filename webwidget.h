#ifndef WEBWIDGET_H
#define WEBWIDGET_H

#include <QWidget>
#include <QtWebKit/QtWebKit>

class WebWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WebWidget(QWidget *parent = 0);

signals:

public slots:
    void showWidget(QString file);
private:
    QWebView *view;

};

#endif // WEBWIDGET_H
