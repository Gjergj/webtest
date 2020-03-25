#include "httpsniffwidget.h"

#include<QMetaType>
HttpSniffWindow::HttpSniffWindow(QWidget *parent) : QWidget(parent)
{
    qRegisterMetaType<SniffedData>("SniffedData");

    memset(sniffedHistoryFile,0,sizeof(sniffedHistoryFile));
    //strcpy_s(historyFile,sizeof(historyFile),"RequestHistory.txt");
    strcpy(sniffedHistoryFile,"SniffedHistory.txt");
    historyListModel = new QStringListModel();//duhet inicializuar para se te merren te dhenat nga file
    QFuture<void> future = QtConcurrent::run(this,&HttpSniffWindow::loadSavedRequestHistory);


    sourceIpLabel = new QLabel("Source IP: ",this);
    sourceIpEdit = new QLineEdit(this);
    sourcePortLabel = new QLabel("Source Port: ",this);
    sourcePortEdit = new QLineEdit(this);
    destinationIpLabel = new QLabel("Destination IP: ",this);
    destinationIpEdit = new QLineEdit(this);
    destinationPortLabel = new QLabel("Destination Port: ",this);
    destinationPortEdit = new QLineEdit(this);
    packetReceivedTimeLabel = new QLabel("Time Recieved: ",this);
    packetReceivedTimeEdit = new QLineEdit(this);
    packetSizeLabel = new QLabel("Packed Size: ",this);
    packetSizeEdit = new QLineEdit(this);
    statLayout = new QGridLayout();

    statLayout->addWidget(sourceIpLabel,0,0);
    statLayout->addWidget(sourceIpEdit,0,1);
    statLayout->addWidget(sourcePortLabel,0,2);
    statLayout->addWidget(sourcePortEdit,0,3);
    statLayout->addWidget(destinationIpLabel);
    statLayout->addWidget(destinationIpEdit);
    statLayout->addWidget(destinationPortLabel);
    statLayout->addWidget(destinationPortEdit);
    statLayout->addWidget(packetReceivedTimeLabel);
    statLayout->addWidget(packetReceivedTimeEdit);
    statLayout->addWidget(packetSizeLabel);
    statLayout->addWidget(packetSizeEdit);



    startSniffingButton = new QPushButton("Start Sniffing",this);
    networkInterfaceLabel = new QLabel("Select Network Interface",this);
    networkInterfaceCombo = new QComboBox();
    portNumber = new QSpinBox();
    portNumber->setRange(0,65535);
    portNumber->setSingleStep(1);
    portNumber->setValue(80);
    startSniffButtonLayout = new QHBoxLayout();
    startSniffButtonLayout->addWidget(networkInterfaceLabel);
    startSniffButtonLayout->addWidget(networkInterfaceCombo);
    startSniffButtonLayout->addWidget(portNumber);
    startSniffButtonLayout->addWidget(startSniffingButton);

    sniffedDataEdit = new QPlainTextEdit(this);

    sniffLayout = new QVBoxLayout();
    sniffLayout->addWidget(sniffedDataEdit);

    //deleteSelectedHistory = new QPushButton("Delete",this);
    //connect(deleteSelectedHistory,&QPushButton::clicked,this,&HttpSniffWindow::removeSniffFromHistory);
    saveHistory = new QPushButton("Save History",this);
    connect(saveHistory,&QPushButton::clicked,this,&HttpSniffWindow::saveHistoryToFile);
    historyButtonsLayout = new QHBoxLayout();
    //historyButtonsLayout->addWidget(deleteSelectedHistory);
    historyButtonsLayout->addWidget(saveHistory);
    //QPushButton *openHistory;
    historyListView = new QListView(this);
    historyListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    historyListView->setContextMenuPolicy(Qt::CustomContextMenu);
    historyListView->setModel(historyListModel);
    connect(historyListView,&QListView::customContextMenuRequested,this,&HttpSniffWindow::historyListViewContextMenuEvent);
    connect(historyListView,&QListView::clicked,this,&HttpSniffWindow::historyListViewClicked);
    communicationErrorLabel = new QLabel("Errors",this);
    communicationErrorEditor = new QPlainTextEdit(this);
    historyLayout = new QVBoxLayout();
    historyLayout->addLayout(startSniffButtonLayout);
    historyLayout->addLayout(statLayout);
    historyLayout->addLayout(historyButtonsLayout);
    historyLayout->addWidget(historyListView);
    historyLayout->addWidget(communicationErrorLabel);
    historyLayout->addWidget(communicationErrorEditor);

    mainLayout = new QHBoxLayout(this);
    mainLayout->addLayout(sniffLayout);
    mainLayout->addLayout(historyLayout);
    setLayout(mainLayout);

    thread = new QThread;
    sniffer = new httpsniffer();
    interfaces = sniffer->enumerateInterfaces();
    QStringList ifaces;
#ifdef __linux
    for(auto eth : interfaces){
        ifaces.push_back(eth.interfaceName + "  |  " + eth.address);
    }
#else
    for(auto eth : interfaces){
        ifaces.push_back(eth.interfaceDescription + "  |  " + eth.address);
    }
#endif
    networkInterfaceCombo->addItems(ifaces);
    sniffer->moveToThread(thread);
    connect(sniffer, SIGNAL (error(QString)), this, SLOT (errorString(QString)));
    connect(sniffer, SIGNAL (receivedData(SniffedData)), this, SLOT (getSniffedData(SniffedData)));
    connect(thread, &QThread::started, sniffer, &httpsniffer::startSniff);
    connect(sniffer, SIGNAL (finished()), thread, SLOT (quit()));
    //do no delete thread so we can start again
    //connect(sniffer, SIGNAL (finished()), sniffer, SLOT (deleteLater()));
    //connect(thread, SIGNAL (finished()), thread, SLOT (deleteLater()));
    connect(startSniffingButton,&QPushButton::clicked,this,&HttpSniffWindow::startSniffing);
    future.waitForFinished();
    communicationErrorEditor->appendPlainText(jsonLoadErrors);
}

HttpSniffWindow::~HttpSniffWindow()
{

}
void HttpSniffWindow::startSniffing(){

    if(startedSniffing){
        startedSniffing = false;
        startSniffingButton->setText("Start Sniffing.");
        sniffer->stop();
    }else{
        if( (portNumber->value() > 65535) || (portNumber->value() < 0) ){
            errorString("Port number should be biger than 0 and smaller 65535.");
            return;
        }
        startedSniffing = true;
        sniffer->setEthInterface(interfaces.at(networkInterfaceCombo->currentIndex()).interfaceName.toStdString());
        ////#define DEFAULT_CAPFILTER "tcp port 80 or 8080"
        sniffer->setFilter("tcp port " + std::to_string(portNumber->value()));
        startSniffingButton->setText("Stop Sniffing.");
        thread->start();
    }
}
void HttpSniffWindow::insertHistoryListView(QString data){

    historyListModel->insertRows(historyListModel->rowCount(), 1);
    historyListModel->setData(historyListModel->index(historyListModel->rowCount()-1), data);
}

void HttpSniffWindow::gotDataFromClient()
{

}
void HttpSniffWindow::getSniffedData(SniffedData data){

    sniffedDataList.push_back(data);
    insertHistoryListView(data.destinationIp + "  |  "+ QString::number(data.packetSize) + " B");

    sourceIpEdit->setText(data.sourceIp);
    sourcePortEdit->setText(data.sourcePort);
    destinationIpEdit->setText(data.destinationIp);
    destinationPortEdit->setText(data.destinationPort);
    packetReceivedTimeEdit->setText(QDateTime::fromMSecsSinceEpoch(data.packetReceivedTime).toString());
    packetSizeEdit->setText(QString::number(data.packetSize));
    sniffedDataEdit->clear();
    sniffedDataEdit->insertPlainText(QTextCodec::codecForMib(106)->toUnicode(data.data));
    //sniffedDataEdit->insertPlainText(QString::fromWCharArray((wchar_t*)data.data.data(),data.data.size()));
    //qDebug() << QString::fromLatin1(data,size_data);
//    QString DataAsString = QTextCodec::codecForMib(1015)->toUnicode(data.data);
}

void HttpSniffWindow::errorString(QString error){
    communicationErrorEditor->appendPlainText(error);
}

void HttpSniffWindow::saveHistoryToFile()
{
    QJsonArray allHistory;
    for(auto hist : sniffedDataList){
        //QJsonArray headersJsonArray;
        QJsonObject singleHistory;
        singleHistory["SourceIP"] = hist.sourceIp;
        singleHistory["DestinationIP"] = hist.destinationIp;
        singleHistory["SourcePort"] = hist.sourcePort;
        singleHistory["DestinationPort"] = hist.destinationPort;

        singleHistory["Timestamp"] = hist.timestamp;
        singleHistory["Data"] = QString(hist.data);
        singleHistory["PacketReceivedTime"] = QString::number(hist.packetReceivedTime);
        singleHistory["PacketSize"] = QString::number(hist.packetSize);

        allHistory.push_back(singleHistory);
    }
    QJsonObject root;
    QJsonDocument historyJsonDoc;
    root["SniffedHistory"] = allHistory;
    historyJsonDoc.setObject(root);
    QFile file(sniffedHistoryFile);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::information(this, tr("Unable to open file"), file.errorString());
        return;
    }
    std::string jsonstring = historyJsonDoc.toJson(QJsonDocument::JsonFormat::Compact).toStdString();
    file.write(jsonstring.c_str(),jsonstring.length());
    file.close();
}

void HttpSniffWindow::historyListViewClicked(const QModelIndex &index)
{
    int selectedindex = index.row();
    SniffedData hist = sniffedDataList.at(selectedindex);

    sourceIpEdit->setText(hist.sourceIp);
    sourcePortEdit->setText(hist.sourcePort);
    destinationIpEdit->setText(hist.destinationIp);
    destinationPortEdit->setText(hist.destinationPort);
    packetReceivedTimeEdit->setText(QDateTime::fromMSecsSinceEpoch(hist.packetReceivedTime).toString());
    packetSizeEdit->setText(QString::number(hist.packetSize));
    sniffedDataEdit->clear();
    sniffedDataEdit->insertPlainText(QTextCodec::codecForMib(106)->toUnicode(hist.data.data(),hist.data.size()));
    //sniffedDataEdit->insertPlainText(QString::fromWCharArray((wchar_t*)hist.data.data(),hist.data.size()));
    //sniffedDataEdit->insertPlainText(QTextCodec::codecForMib(1015)->toUnicode(hist.data));

}

void HttpSniffWindow::loadSavedRequestHistory()
{
    QFile histFile(sniffedHistoryFile);
    QJsonParseError parserError;
    if(histFile.exists()){
        if(!histFile.open(QIODevice::ReadOnly)){
            jsonLoadErrors += "Could not open history file.";
            return;
        }

        QByteArray rawJson = histFile.readAll();
        if(rawJson.size() == 0){
            jsonLoadErrors += "Empty history.";
            return;
        }
        QJsonDocument historyJsonDoc(QJsonDocument::fromJson(rawJson,&parserError));
        if(parserError.error != QJsonParseError::NoError){
            jsonLoadErrors += "Could not load history: " + parserError.errorString();
            return;
        }

        QJsonObject root = historyJsonDoc.object() ;
        QJsonArray allHistory = root["SniffedHistory"].toArray();

        for(int i = 0; i < allHistory.size(); i++){
            SniffedData sniffSession;
            QJsonObject singleHistory = allHistory[i].toObject();
            sniffSession.sourceIp = singleHistory["SourceIP"].toString();
            sniffSession.destinationIp = singleHistory["DestinationIP"].toString();
            sniffSession.sourcePort = singleHistory["SourcePort"].toString();
            sniffSession.destinationPort = singleHistory["DestinationPort"].toString();

            sniffSession.timestamp = singleHistory["Timestamp"].toString();
            sniffSession.data = QByteArray(singleHistory["Data"].toString().toStdString().c_str());
            sniffSession.packetReceivedTime = singleHistory["PacketReceivedTime"].toString().toULongLong();
            sniffSession.packetSize = singleHistory["PacketSize"].toString().toULongLong();

            sniffedDataList.push_back(sniffSession);
            insertHistoryListView(sniffSession.destinationIp + "  |  "+ QString::number(sniffSession.packetSize) + " B");
        }
    }
}

void HttpSniffWindow::removeSniffFromHistory()
{
    QModelIndex selectedItem = historyListView->currentIndex();
    int selectedindex = selectedItem.row();
    sniffedDataList.removeAt(selectedindex);
    historyListModel->removeRows(selectedindex,1);
}

void HttpSniffWindow::historyListViewContextMenuEvent(const QPoint &pos)
{
    QPoint globalPos = historyListView->mapToGlobal(pos);

    // Create menu and insert some actions
    QMenu myMenu;
    myMenu.addAction("Delete",  this, &HttpSniffWindow::removeSniffFromHistory);
    myMenu.addAction("Erase All History",  this, &HttpSniffWindow::removeAllHistory);

    // Show context menu at handling position
    myMenu.exec(globalPos);
}

void HttpSniffWindow::removeAllHistory()
{
    sniffedDataList.clear();
    historyListModel->removeRows(0,historyListModel->rowCount());
}
