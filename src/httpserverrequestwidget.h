#ifndef HTTPSERVERREQUESTWIDGET_H
#define HTTPSERVERREQUESTWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListView>
#include <QTextEdit>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QLineEdit>
#include <QRadioButton>
#include <QLabel>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QAuthenticator>
#include <QUrl>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkAccessManager>
#include <QSslSocket>
#include <QStringListModel>
#include <QMenu>
#include <QApplication>
#include <QClipboard>
#include <QMessageBox>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include <QtCore>
#include <QBuffer>
#include <QCheckBox>
#include <QTabWidget>
#include <QElapsedTimer>

#include "mtypes.h"
class HttpServerRquesteWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HttpServerRquesteWidget(QWidget *parent = 0);
    void initState();
    void setRequestData(RequestCtx requestData);
    void setRawRequest(QString rawRequest);
signals:
    void error(QString);
private slots:
    void jsonRequestBodyBeautify(bool beautify);
public slots:

private:
    QVBoxLayout *requestLayout;
    QLabel *requestHeadersLabel;
    QPlainTextEdit *requestHeadersEditor;
    //QLabel *requestHttpStatusLabel;
    QCheckBox *jsonBeautifyCheckbox;
    QLabel *requestTimeLabel;
    QLabel *requestBodyLengthLabel;
    QPlainTextEdit *requestBodyEditor;
    QHBoxLayout *requestJsonBeautifyLayout;

    QVBoxLayout *mainLayout;
    QWidget *requestMainTabWidget;
    QTabWidget *requestTabWidget;

    QWidget *rawRequestTabWidget;
    QVBoxLayout *rawRequestLayout;
    QPlainTextEdit *rawRequestEdit;
};

#endif // HTTPSERVERRESPONSEWIDGET_H
