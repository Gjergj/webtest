#ifndef HTTPCLIENTWINDOWV2_H
#define HTTPCLIENTWINDOWV2_H

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
#include <QCheckBox>
#include <QRegExp>
#include <QRegularExpressionMatch>
#include <QFileDialog>
#include <QSplitter>
#include <QTreeView>
#include <QTreeView>
#include <beast/http.hpp>
#include <beast/websocket.hpp>
#include <beast/http/message.hpp>
#include <beast/http.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <beast/core/to_string.hpp>

#include <zlib.h>

#include "requestheaderswidget.h"
#include "responsewidget.h"
#include "configurations.h"
#include "treemodel.h"
#include "texteditor.h"

//#include "types.h"

class SortFilterProxyModel;
class HttpClientWindowv2 : public QWidget
{
    Q_OBJECT
public:
    explicit HttpClientWindowv2(Notepad *notepadEditor, QWidget *parent = 0);
    ~HttpClientWindowv2();
signals:

private slots:

    void sendRequest();
    void removeRequestFromHistory();
    void historyListViewClicked(const QModelIndex &index);
    void removeAllHistory();
    void saveHistoryToFile();
    void historyListViewContextMenuEvent(const QPoint &pos);
    void responseWidgetError(QString error);
    void saveAsHistoryToFile();
    void loadHistoryFile();
    void communicationErrorEditorContextMenuEvent(const QPoint &pos);
    void sendToNotepadEditor();
    void filterHistoryTreeView(const QString &text);
private:
    void insertHistoryListView(HttpSession &data);
    void loadSavedRequestHistory();
    int saveHistoryFile(QString fileNameAndPath = QString());
    QByteArray gzipDecompress( QByteArray compressData );
    void resizeEvent(QResizeEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);
    QHBoxLayout *mainLayout;
    QSplitter *mainSplitter;


    //QPushButton *sendButton;
    QHBoxLayout *mainControlsLayout;

    RequestHeadersWidget *requestHeadersWidget;
    QVBoxLayout *requestLayout;

    ResponseWidget *responseWindget;

    QHBoxLayout *historyButtonsLayout;
    QPushButton *saveHistory;
    QPushButton *saveAsHistory;
    QPushButton *openHistory;
    QTreeView *historyListView;
    QVBoxLayout *historyErrorLayout;
    TreeModel* historyTreeModel;
    QSplitter *historySplitter;

    TextEditor *communicationErrorEditor;
    QLabel *communicationErrorLabel;

    QList<HttpSession> requestHistory;
    QString historyFileNameAndPath;
    QString jsonLoadErrors;

    QBuffer patchRequestIoDevice;
    QElapsedTimer responseTime;
    HttpSession currentSession;
    QString lastFileSaveDir;
    Notepad *applicationNodepadEditor;
    SortFilterProxyModel *historyTreeViewFilter;
    QLineEdit *filterEdit;
    int _nr_history;
};

#endif // HTTPCLIENTWINDOW_H
