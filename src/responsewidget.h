#ifndef RESPONSEWIDGET_H
#define RESPONSEWIDGET_H

#include <QWidget>
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
#include <QSplitter>
#include <QWebEngineView>
#include "mtypes.h"
#include "texteditor.h"

class Notepad;
class ResponseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ResponseWidget(Notepad *notepadEditor, QWidget *parent = 0);
    void initState();
    void setResponseData(ResponseDataCtx responseData);
    void setRawResponse(QString rawResponse);
    void rawWindgetVisible(bool visible);
    void addStatistic(QString statLine);
signals:
    void error(QString);
public slots:
private slots:
    void jsonResponeBodyBeautify(bool beautify);
    void responseBodyEditorContextMenuEvent(const QPoint &pos);
    void responseHeadersEditorContextMenuEvent(const QPoint &pos);
    void sendToNotepadEditor();
    void sendToNotepadEditorFromResponseHeadersEditor();
    void tabChanged(int index);
private:
    QVBoxLayout *responseLayout;
    QLabel *responseHeadersLabel;
    TextEditor *responseHeadersEditor;
    QLabel *responseHttpStatusLabel;
    QCheckBox *jsonBeautifyCheckbox;
    QLabel *responseTimeLabel;
    QLabel *responseBodyLengthLabel;
    TextEditor *responseBodyEditor;
    QWebEngineView *bodyWebView;
    QHBoxLayout *reponseJsonBeautifyLayout;

    QVBoxLayout *mainLayout;
    QTabWidget *responseTabWidget;

    QPlainTextEdit *responseStatisticsEdit;
    QPlainTextEdit *rawResponseEdit;
    QSplitter *mainSplitter;
    Notepad *applicationNodepadEditor;
    bool isJsonData(QString &data);
};

#endif // RESPONSEWIDGET_H
