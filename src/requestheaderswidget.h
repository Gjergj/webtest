#ifndef REQUESTHEADERSWIDGET_H
#define REQUESTHEADERSWIDGET_H

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
#include <QComboBox>
#include <QSplitter>
#include "clickablelabel.h"
#include "mtypes.h"
#include "texteditor.h"
#include "requestheadermodel.h"
#include "headerlistmodel.h"
#include "urlencodedbodymodel.h"
#include "querystringmodel.h"
class Notepad;
class RequestHeadersWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString bodyText READ getBodyText WRITE setBodyText NOTIFY bodyTextChanged)
    Q_PROPERTY(int spliterSize READ getSpliterSize WRITE setSpliterSize NOTIFY spliterSizeChanged)
    Q_PROPERTY(QString httpVersion READ getHttpVersion WRITE setHttpVersion NOTIFY httpVersionChanged)
    Q_PROPERTY(QString url READ getUrl WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(QString httpMethod READ getHttpMethod WRITE setHttpMethod NOTIFY httpMethodChanged)
    Q_PROPERTY(bool modifyHeadersFromGui READ getmodifyHeadersFromGui WRITE setModifyHeadersFromGui NOTIFY modifyHeadersFromGuiChanged)
public:
    explicit RequestHeadersWidget(Notepad *notepadEditor, QWidget *parent = 0);

    void setRequestData(RequestCtx requestData);
    RequestCtx getRequestData();
    void setRawRequest(QString rawRequest);
    void rawWindgetVisible(bool);
    Q_INVOKABLE int addHeader(const QString headerField, QString headerValue);
    Q_INVOKABLE void removeHeader(int position);
    Q_INVOKABLE void removeAllHeaders();
    Q_INVOKABLE void copyHeadersAllToClipBoard();
    Q_INVOKABLE void copyHeadersToClipBoard(QList<int> rowsIndex);
    Q_INVOKABLE void pasteHeadersFromClipBoard();
    Q_INVOKABLE void updateContentLenght();
    Q_INVOKABLE void sendToNotepadEditor(QString text);
    Q_INVOKABLE QString setQueryStringToUrl(QString oldUrl, QString querystring);
    Q_INVOKABLE QString getQueryStringFromUrl(QString currentUrl);
    Q_INVOKABLE void sendRequest();

    QString getBodyText() const;
    int getSpliterSize();
    QString getHttpVersion();
    QString getUrl() const;
    QString getHttpMethod() const;
    bool getmodifyHeadersFromGui() const;
signals:
    
public slots:

public Q_SLOTS:
    void setBodyText(const QString &arg);
    void setSpliterSize(int size);
    void setHttpVersion(QString version);
    void setUrl(QString url);
    void setHttpMethod(QString httpMethod);
    void setModifyHeadersFromGui(bool modifyHeadersFromGui);
Q_SIGNALS:
    void bodyTextChanged();
    void spliterSizeChanged();
    void httpVersionChanged();
    void urlChanged();
    void httpMethodChanged();
    void modifyHeadersFromGuiChanged();
    void sendTheRequest();
private slots:
    void listViewClicked(const QModelIndex &index);
    void addContentLength();

private:
    //std::vector<std::pair<std::string,int>> httpversions;

    void addHeader(QPair<QString,QString> header);
    void headerInListViewSelected(int position);

    QVBoxLayout *mainLayout;
    QTabWidget *requestHeaderTabWidget;

    QVBoxLayout *requestHeaderNameLayout;
    QHBoxLayout *requestHeaderLayout;

    QWidget *headersMainTabWidget;
    QVBoxLayout *headersMainTabLayout;

    QWidget *rawRequestTabWidget;
    QVBoxLayout *rawRequestLayout;
    QPlainTextEdit *rawRequestEdit;
    RequestCtx reqData;
    Notepad *applicationNodepadEditor;
    RequestHeaderModel *requestHeaderModel;
    UrlencodedBodyModel *urlencodedBodyModel;
    HeaderListModel *headerListModel;
    QueryStringModel *queryStringModel;
    QString _bodyText;
    int _spliterSize;
    int _httpVersion;//10 ose 11
    QString _url;
    QString _httpMethod;
    bool _modifyHeadersFromGui;
};

#endif // REQUESTHEADERSWIDGET_H
