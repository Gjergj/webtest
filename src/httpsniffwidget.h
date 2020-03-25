#ifndef HTTPSNIFF_H
#define HTTPSNIFF_H

#include <QWidget>
#include <QHBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QtDebug>
#include <QThread>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListView>
#include <QStringListModel>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>
#include <QtConcurrent/QtConcurrent>
#include <QFuture>
#include <QMenu>
#include "httpsniffer.h"

class HttpSniffWindow : public QWidget
{
    Q_OBJECT
public:
    explicit HttpSniffWindow(QWidget *parent = 0);
    ~HttpSniffWindow();

signals:

public slots:

private slots:
    void startSniffing();
    void getSniffedData(SniffedData data);
    void errorString(QString);
    void saveHistoryToFile();
    void historyListViewClicked(const QModelIndex &index);
    void loadSavedRequestHistory();
    void removeSniffFromHistory();
    void historyListViewContextMenuEvent(const QPoint &pos);
    void removeAllHistory();
private:
    void insertHistoryListView(QString data);
    void gotDataFromClient();
    QGridLayout *statLayout;
    QLabel *sourceIpLabel;
    QLineEdit *sourceIpEdit;
    QLabel *sourcePortLabel;
    QLineEdit *sourcePortEdit;

    QLabel *destinationIpLabel;
    QLineEdit *destinationIpEdit;
    QLabel *destinationPortLabel;
    QLineEdit *destinationPortEdit;

    QLabel *packetReceivedTimeLabel;
    QLineEdit *packetReceivedTimeEdit;

    QLabel *packetSizeLabel;
    QLineEdit *packetSizeEdit;


    QHBoxLayout *mainLayout;
    QLabel *networkInterfaceLabel;
    QVBoxLayout *sniffLayout;
    QComboBox *networkInterfaceCombo;
    QPushButton *startSniffingButton;
    QHBoxLayout *startSniffButtonLayout;
    QPlainTextEdit *sniffedDataEdit;
    QThread* thread;
    QSpinBox *portNumber;
    httpsniffer *sniffer;
    bool startedSniffing = false;
    int noPacketsReceived = 0;
    QList<InterfaceInfo> interfaces;

    QVBoxLayout *historyLayout;
    QHBoxLayout *historyButtonsLayout;
    //QPushButton *deleteSelectedHistory;
    QPushButton *saveHistory;
    //QPushButton *openHistory;
    QListView *historyListView;
    QStringListModel* historyListModel;

    QPlainTextEdit *communicationErrorEditor;
    QLabel *communicationErrorLabel;
    QList<SniffedData> sniffedDataList;
    char sniffedHistoryFile[255];
    QString jsonLoadErrors;

};

#endif // HTTPSNIFF_H
