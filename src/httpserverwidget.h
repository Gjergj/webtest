#ifndef HTTPSERVERWIDGET_H
#define HTTPSERVERWIDGET_H

#include <QObject>
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
#include <QSpinBox>
#include <QComboBox>
#include <thread>
#include "httpserverrequestwidget.h"
#include "httpserverresponsewidget.h"
#include <boost/asio.hpp>
#include "httpserver.h"
#include "httpsserver.h"
class http_sync_server;

class HttpServerWidget : public QWidget
{

    Q_OBJECT
public:
    explicit HttpServerWidget(QWidget *parent = 0);

signals:

public slots:
private slots:
    void errorMessage(QString errorMessage);
    void startListen();
    void newProject();
    void saveProject();
    void listViewContextMenuEvent(const QPoint &pos);
    void listViewClicked(const QModelIndex &index);
    void projectListComboSelected(int index);
    void removeProjectPressed();
    void removeAllProjects();
    void httpServerRequestReceived(QString requestMessage);
    void httpServerRequestError(QString errorMessage);
    void processRequest();
    void saveProjectsToFile();
private:
    bool fillCurrentProjectFromGUI();
    void loadSavedRequestHistory();
    QLineEdit *domainEdit;
    QCheckBox *httpCheckBox;
    QSpinBox *httpPortSpinBox;
    QCheckBox *httpsCheckBox;
    QSpinBox *httpsPortSpinBox;
    QLineEdit *listenIpEdit;
    QPushButton *startListenButton;
    QPushButton *addProjectToHistoryButton;
    QPushButton *newProjectButton;
    QHBoxLayout *mainControlsLayout;

    HttpServerRquesteWidget *httpServerRquesteWidget;

    HttpServerResponseWidget *httpServerResponseWidget;
    QHBoxLayout *requestResponseLayout;

    QVBoxLayout *historyLayout;
    QHBoxLayout *historyButtonsLayout;
    //QPushButton *deleteSelectedHistory;
    QComboBox *projectListCombo;
    QPushButton *saveHistory;
    //QPushButton *openHistory;
    QListView *historyListView;
    QStringListModel* historyListViewModel;
    QPlainTextEdit *communicationErrorEditor;
    QLabel *communicationErrorLabel;

    QVBoxLayout *mainLayout;

    QString jsonLoadErrors;
    HttpServerProjectCtx currentProject;
    QList<HttpServerProjectCtx> httpServerProjectList;
    QList<ServerRequestResponse> serverRequestResponseList;
    bool serverStarted = false;

    http_sync_server *server = nullptr;
    https_sync_server *server_ssl = nullptr;
    boost::asio::io_service ios_;
    std::thread thread_;
    bool thread_started = false;
    QTimer *httpServerPollTimer;
    QString projectsFile;
};

#endif // HTTPSERVERWIDGET_H
