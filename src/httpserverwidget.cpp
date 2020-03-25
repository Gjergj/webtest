#include "httpserverwidget.h"
#include "lib/json.hpp"

using endpoint_type = boost::asio::ip::tcp::endpoint;
using address_type = boost::asio::ip::address;
HttpServerWidget::HttpServerWidget(QWidget *parent) : QWidget(parent),
    projectsFile("HtttpServerProjects.txt")
{
    //per secilin path do kete header, status, status description dhe body
    historyListView = new QListView(this);
    projectListCombo = new QComboBox(this);
    connect(projectListCombo,static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),this,&HttpServerWidget::projectListComboSelected);
    QFuture<void> future = QtConcurrent::run(this,&HttpServerWidget::loadSavedRequestHistory);
    listenIpEdit = new QLineEdit(this);
    QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    // You may want to use QRegularExpression for new code with Qt 5 (not mandatory).
    QRegExp ipRegex ("^" + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange + "$");
    QRegExpValidator *ipValidator = new QRegExpValidator(ipRegex, this);
    listenIpEdit->setValidator(ipValidator);
    listenIpEdit->setPlaceholderText("IP to listen");
    listenIpEdit->setText("0.0.0.0");

    domainEdit = new QLineEdit(this);
    domainEdit->setPlaceholderText("Domain");
    httpCheckBox = new QCheckBox("HTTP server",this);
    httpCheckBox->setChecked(true);
    httpPortSpinBox = new QSpinBox(this);
    httpPortSpinBox->setRange(0,65535);
    httpPortSpinBox->setSingleStep(1);
    httpPortSpinBox->setValue(8081);


    httpsCheckBox = new QCheckBox("HTTPS server",this);
    httpsCheckBox->setChecked(true);

    httpsPortSpinBox = new QSpinBox(this);
    httpsPortSpinBox->setRange(0,65535);
    httpsPortSpinBox->setSingleStep(1);
    httpsPortSpinBox->setValue(443);

    startListenButton = new QPushButton("Start Server",this);
    connect(startListenButton,&QPushButton::clicked,this,&HttpServerWidget::startListen);
    addProjectToHistoryButton = new QPushButton("Save Project",this);
    connect(addProjectToHistoryButton,&QPushButton::clicked,this,&HttpServerWidget::saveProject);
    newProjectButton = new QPushButton("New Project",this);
    connect(newProjectButton,&QPushButton::clicked,this,&HttpServerWidget::newProject);
    mainControlsLayout = new QHBoxLayout();
    mainControlsLayout->addWidget(newProjectButton);
    mainControlsLayout->addWidget(listenIpEdit);
    mainControlsLayout->addWidget(domainEdit);
    mainControlsLayout->addWidget(httpCheckBox);
    mainControlsLayout->addWidget(httpPortSpinBox);
    mainControlsLayout->addWidget(httpsCheckBox);
    mainControlsLayout->addWidget(httpsPortSpinBox);
    mainControlsLayout->addWidget(startListenButton);
    mainControlsLayout->addWidget(addProjectToHistoryButton);
    mainControlsLayout->addStretch(0);

    //deleteSelectedHistory = new QPushButton("Delete",this);
    saveHistory = new QPushButton("Save Projects to File",this);
    connect(saveHistory,&QPushButton::clicked,this,&HttpServerWidget::saveProjectsToFile);

    historyButtonsLayout = new QHBoxLayout();

    //historyButtonsLayout->addWidget(deleteSelectedHistory);
    historyButtonsLayout->addWidget(saveHistory);
    historyButtonsLayout->addWidget(projectListCombo);
    //QPushButton *openHistory;

    historyListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    historyListView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(historyListView,&QListView::customContextMenuRequested,this,&HttpServerWidget::listViewContextMenuEvent);
    connect(historyListView,&QListView::clicked,this,&HttpServerWidget::listViewClicked);
    historyListViewModel = new QStringListModel(this);
    historyListView->setModel(historyListViewModel);


    communicationErrorLabel = new QLabel("Errors",this);
    communicationErrorEditor = new QPlainTextEdit(this);
    historyLayout = new QVBoxLayout();
    historyLayout->addLayout(historyButtonsLayout);
    historyLayout->addWidget(historyListView);
    historyLayout->addWidget(communicationErrorLabel);
    historyLayout->addWidget(communicationErrorEditor);

    httpServerRquesteWidget = new HttpServerRquesteWidget(this);
    connect(httpServerRquesteWidget,&HttpServerRquesteWidget::error,this,&HttpServerWidget::errorMessage);
    httpServerResponseWidget = new HttpServerResponseWidget(this);
    connect(httpServerResponseWidget,&HttpServerResponseWidget::error,this,&HttpServerWidget::errorMessage);
    requestResponseLayout  =new QHBoxLayout();
    requestResponseLayout->addWidget(httpServerRquesteWidget);
    requestResponseLayout->addWidget(httpServerResponseWidget);
    requestResponseLayout->addLayout(historyLayout);

    mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(mainControlsLayout);
    mainLayout->addLayout(requestResponseLayout);

    mainLayout->setSpacing(5);
    mainLayout->setMargin(5);
    setLayout(mainLayout);

    future.waitForFinished();
    communicationErrorEditor->appendPlainText(jsonLoadErrors);
    httpServerPollTimer = new QTimer(this);
    connect(httpServerPollTimer, SIGNAL(timeout()), this, SLOT(processRequest()));
}

void HttpServerWidget::errorMessage(QString errorMessage)
{
    communicationErrorEditor->appendPlainText(errorMessage);
}


void HttpServerWidget::startListen()
{
    if( (!serverStarted) && (!fillCurrentProjectFromGUI())){
        return;
    }

    if(serverStarted){
        serverStarted = false;
        startListenButton->setText("Start Server");
        httpServerPollTimer->stop();

        if(server != nullptr){
            if(server->isOpen()){
                server->stopServer();
                delete server;
                server = nullptr;
            }
        }

        if(server_ssl != nullptr){
            if(server_ssl->isOpen()){
                server_ssl->stopServer();
                delete server_ssl;
                server_ssl = nullptr;
            }
        }
        projectListCombo->setEnabled(true);
    }else{
        if(currentProject.http){
            server  = new http_sync_server(ios_);
            //connect(server, &http_sync_server::requestReceived,this, &HttpServerWidget::httpServerRequestReceived);
            connect(server, &http_sync_server::error,this, &HttpServerWidget::httpServerRequestError);
            if(server->startServer(currentProject)){
                serverStarted = true;
            }else{
                serverStarted = false;
                delete server;
                server = nullptr;
            }
        }
        if(currentProject.https){
            server_ssl = new https_sync_server(ios_);
            connect(server_ssl, &https_sync_server::error,this, &HttpServerWidget::httpServerRequestError);
            if(server_ssl->startServer(currentProject)){
                serverStarted = true;
            }else{
                if(currentProject.http && serverStarted){ //http opened correctly
                    delete server;
                    server = nullptr;
                }
                serverStarted = false;
                delete server_ssl;
                server_ssl = nullptr;
            }
        }

        if(serverStarted){
            projectListCombo->setDisabled(true);
            startListenButton->setText("Stop Server");
            httpServerPollTimer->start(5);
        }

    }

}

bool HttpServerWidget::fillCurrentProjectFromGUI()
{
    //    if(domainEdit->text().isEmpty()){
    //        errorMessage("Domain should not be empty");
    //        return;
    //    }
    if((!httpCheckBox->isChecked()) && (!httpsCheckBox->isChecked())){
        errorMessage("Select HTTP or HTTPS server");
        return false;
    }
    if(httpPortSpinBox->value() <= 0){
        errorMessage("Port number can not be less than zero");
        return false;
    }
    if(httpsPortSpinBox->value() <= 0){
        errorMessage("Port number can not be less than zero");
        return false;
    }
    if(listenIpEdit->text().isEmpty()){
        errorMessage("IP address should not be empty. Consider using 127.0.0.1");
        return false;
    }
    currentProject.routes = httpServerResponseWidget->getResponseData();

    if(currentProject.routes.size() <= 0){
        errorMessage("Response data should not be empty. There should be at least one path.");
        return false;
    }
    currentProject.domain = domainEdit->text().toStdString();
    if(httpCheckBox->isChecked()){
        currentProject.http = true;
    }else{
        currentProject.http = false;
    }
    if(httpsCheckBox->isChecked()){
        currentProject.https = true;
    }else{
        currentProject.https = false;
    }
    currentProject.httpPort = httpPortSpinBox->value();
    currentProject.httpsPort = httpsPortSpinBox->value();
    currentProject.IPListen = listenIpEdit->text().toStdString();
    return true;
}

void HttpServerWidget::newProject()
{
    httpServerResponseWidget->setResponseData(std::list<RequestCtx>());
}

void HttpServerWidget::saveProject()
{
    fillCurrentProjectFromGUI();
    httpServerProjectList.push_back(currentProject);
    projectListCombo->addItem(QString::fromStdString(currentProject.IPListen) + " | " + QString::fromStdString(currentProject.domain));
}

void HttpServerWidget::listViewContextMenuEvent(const QPoint &pos)
{
    QPoint globalPos = historyListView->mapToGlobal(pos);

    // Create menu and insert some actions
    QMenu myMenu;
    myMenu.addAction("Delete",  this, &HttpServerWidget::removeProjectPressed);
    myMenu.addAction("Erase All Projects",  this, &HttpServerWidget::removeAllProjects);

    // Show context menu at handling position
    myMenu.exec(globalPos);
}

void HttpServerWidget::listViewClicked(const QModelIndex &index)
{
    currentProject.clear();
    int selectedindex = index.row();

    ServerRequestResponse reqresp = serverRequestResponseList.at(selectedindex);
    httpServerResponseWidget->setRawResponse(reqresp.rawResponse);
    httpServerRquesteWidget->setRawRequest(reqresp.rawRequest);
    httpServerRquesteWidget->setRequestData(reqresp.reqData);
}

void HttpServerWidget::projectListComboSelected(int index)
{
    currentProject = httpServerProjectList.at(index);
    httpServerResponseWidget->setResponseData(currentProject.routes);

    domainEdit->setText(QString::fromStdString(currentProject.domain));

    httpCheckBox->setChecked(currentProject.http);
    httpsCheckBox->setChecked(currentProject.https);
    httpPortSpinBox->setValue(currentProject.httpPort);
    httpsPortSpinBox->setValue(currentProject.httpsPort);
    listenIpEdit->setText(QString::fromStdString(currentProject.IPListen));
}

void HttpServerWidget::removeProjectPressed()
{
    QModelIndex selectedItem = historyListView->currentIndex();
    int selectedindex = selectedItem.row();
    //httpServerProjectList.removeAt(selectedindex);
    serverRequestResponseList.removeAt(selectedindex);
    historyListViewModel->removeRows(selectedindex,1);
}

void HttpServerWidget::removeAllProjects()
{
    serverRequestResponseList.clear();
    historyListViewModel->removeRows(0,historyListViewModel->rowCount());
}

void HttpServerWidget::httpServerRequestReceived(QString requestMessage)
{
    communicationErrorEditor->appendPlainText(requestMessage);
}

void HttpServerWidget::httpServerRequestError(QString errorMessage)
{
    communicationErrorEditor->appendPlainText(errorMessage);
}

void HttpServerWidget::processRequest()
{
    ios_.poll();

    if(!httpServerRequestResponseQueue.Empty()){
        ServerRequestResponse reqresp = httpServerRequestResponseQueue.TryPop();
        httpServerResponseWidget->setRawResponse(reqresp.rawResponse);
        httpServerRquesteWidget->setRawRequest(reqresp.rawRequest);
        httpServerRquesteWidget->setRequestData(reqresp.reqData);
        communicationErrorEditor->appendPlainText(reqresp.errorData);
        serverRequestResponseList.push_back(reqresp);

        QString listHist = QString::fromStdString(currentProject.IPListen) + QString::fromStdString(reqresp.reqData.url);
        historyListViewModel->insertRows(historyListViewModel->rowCount(), 1);
        historyListViewModel->setData(historyListViewModel->index(historyListViewModel->rowCount()-1), listHist);
    }
}

void HttpServerWidget::saveProjectsToFile()
{
    nlohmann::json allProjects;
    for(auto proj : httpServerProjectList){
        nlohmann::json projectJson;
        proj.serialise(projectJson);
        allProjects.push_back(projectJson);
    }
    nlohmann::json root;
    root["HttpServerProjects"] = allProjects;
    QFile file(projectsFile);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::information(this, tr("Unable to open file"), file.errorString());
        return;
    }
    std::string jsonstring = root.dump(4);
    file.write(jsonstring.c_str(),jsonstring.length());
    file.close();
}


void HttpServerWidget::loadSavedRequestHistory(){
    QFile histFile(projectsFile);
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
        try{
            nlohmann::json historyJsonDoc = nlohmann::json::parse(rawJson.toStdString());
            for (auto it = historyJsonDoc["HttpServerProjects"].begin(); it != historyJsonDoc["HttpServerProjects"].end(); ++it)
            {
                HttpServerProjectCtx reqSession;
                reqSession.deSerialize(*it);
                httpServerProjectList.push_back(reqSession);
                projectListCombo->addItem(QString::fromStdString(reqSession.IPListen) + " | " + QString::fromStdString(reqSession.domain));
            }
        }catch (std::exception ex){
            jsonLoadErrors = QString::fromStdString(ex.what());
        }
    }
}
