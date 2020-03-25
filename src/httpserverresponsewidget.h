#ifndef HTTPSERVERRESPONSEWIDGET_H
#define HTTPSERVERRESPONSEWIDGET_H

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
#include <QSpinBox>
#include <QComboBox>
#include "clickablelabel.h"
#include "mtypes.h"
class HttpServerResponseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HttpServerResponseWidget(QWidget *parent = 0);
    void setResponseData(std::list<RequestCtx> routeData);
    std::list<RequestCtx> getResponseData();
    void setRawResponse(QString rawResponse);
signals:
    void error(QString errorMessage);
public slots:
private slots:
    void addHeaderButtonPressed();
    void listViewContextMenuEvent(const QPoint &pos);
    void removeHeaderPressed();
    void listViewClicked(const QModelIndex &index);
    void copyHeadersToClipBoard();
    void removeAllHeaders();
    void addUrlButtonPressed();
    void urlListViewContextMenuEvent(const QPoint &pos);
    void urlListViewClicked(const QModelIndex &index);
    void addContentLength();
    void removeUrlPressed();
    void removeAllUrl();
private:
    void insertHeaderListView(QString data);
    void insertUrlListView(QString data);
    void clearGUI();
    void addHeader(QPair<QString,QString> header);
    void headerInListViewSelected(int position);
    void removeHeader(int position);
    void setHeaderData(RequestCtx routeData);

    QRadioButton *putResponseButton;
    QRadioButton *postResponseButton;
    QRadioButton *getResponseButton;
    QRadioButton *patchResponseButton;
    QRadioButton *deleteResponseButton;
    QHBoxLayout *httpMethodRadioButtonLayout;

    //QLineEdit *urlTextEdit;

    QVBoxLayout *mainLayout;
    QSpinBox *responseStatus;
    QLineEdit *responseDescription;
    QHBoxLayout *responseStatusLayout;
    QTabWidget *responseHeaderTabWidget;
    QListView *responseHeadersListView;
    QLabel *responseHeaderListLabel;
    QVBoxLayout *responseHeaderNameLayout;
    QLineEdit *responseHeaderNameEdit;
    QVBoxLayout *responseHeaderValueLayout;
    QLineEdit *responseHeaderValueEdit;
    QStringListModel* responseHeaderListModel;
    QHBoxLayout *responseHeaderLayout;
    QPushButton *addresponseHeaderButton;
    QVBoxLayout *responseHeaderButtonLayout;

    QListView *urlListView;
    QStringListModel* responseUrlListModel;
    QLineEdit *urlTextEdit;
    QPushButton *addUrlButton;
    QHBoxLayout *urlLayout;

    QLabel *responseBodyLabel;
    ClickableLabel *addContentLengthLabel;
    QPlainTextEdit *responseBodyEditor;
    QHBoxLayout *bodyControlsLayout;

    QWidget *headersMainTabWidget;
    QVBoxLayout *headersMainTabLayout;

    QWidget *rawResponseTabWidget;
    QVBoxLayout *rawResponseLayout;
    QPlainTextEdit *rawResponseEdit;

    RequestCtx currentRoute;
    std::list<RequestCtx> httpRoutes;
};

#endif // HTTPSERVERRESPONSEWIDGET_H
