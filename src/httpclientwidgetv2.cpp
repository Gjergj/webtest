#include "httpclientwidgetv2.h"
#include <beast/core/to_string.hpp>
#include "utils.h"
#include "httpclient.h"
#include "notepad.h"
#include "treemodel.h"
#include "sortfilterproxymodel.h"

HttpClientWindowv2::HttpClientWindowv2(Notepad *notepadEditor, QWidget *parent)
    : QWidget(parent),
      _nr_history(0)
{
    lastFileSaveDir = QString::fromStdString(Configurations::get().getHistoryLastSavedDir());
    applicationNodepadEditor = notepadEditor;

    QStringList headers;
    headers << "Type" << "Host" << "Path" << "Reply" << "Time Stamp" << "Url" << "#";
    historyTreeModel = new TreeModel(headers,"");//duhet inicializuar para se te merren te dhenat nga file
    historyTreeViewFilter = new SortFilterProxyModel();
    historyTreeViewFilter->setSource(historyTreeModel);
    //historyTreeViewFilter->setFilterSyntax(SortFilterProxyModel::RegExp);
    //historyTreeViewFilter->setSortingEnabled(true);
    //historyTreeViewFilter->setDynamicSortFilter(true);
    historyTreeViewFilter->setFilterKeyColumn(5);
    //historyTreeViewFilter->setFilterString("POST");
    QFuture<void> future = QtConcurrent::run(this,&HttpClientWindowv2::loadSavedRequestHistory);

    requestHeadersWidget = new RequestHeadersWidget(applicationNodepadEditor, this);
    connect(requestHeadersWidget, &RequestHeadersWidget::sendTheRequest, this, &HttpClientWindowv2::sendRequest);
    responseWindget = new ResponseWidget(applicationNodepadEditor);
    connect(responseWindget, &ResponseWidget::error,this,&HttpClientWindowv2::responseWidgetError);

    saveHistory = new QPushButton("Save");
    connect(saveHistory,&QPushButton::clicked,this,&HttpClientWindowv2::saveHistoryToFile);
    saveAsHistory = new QPushButton("Save As");
    connect(saveAsHistory,&QPushButton::clicked,this,&HttpClientWindowv2::saveAsHistoryToFile);
    openHistory  = new QPushButton("Load");
    connect(openHistory,&QPushButton::clicked,this,&HttpClientWindowv2::loadHistoryFile);

    historyListView = new QTreeView();
    historyListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    historyListView->setContextMenuPolicy(Qt::CustomContextMenu);
    historyListView->setModel(historyTreeViewFilter);
    for (int column = 0; column < historyTreeModel->columnCount(); ++column)
        historyListView->resizeColumnToContents(column);
    historyListView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    connect(historyListView,&QListView::customContextMenuRequested,this,&HttpClientWindowv2::historyListViewContextMenuEvent);
    connect(historyListView,&QListView::clicked,this,&HttpClientWindowv2::historyListViewClicked);

    historyButtonsLayout = new QHBoxLayout();
    historyButtonsLayout->addWidget(saveHistory);
    historyButtonsLayout->addSpacerItem(new QSpacerItem(1,1));
    historyButtonsLayout->addWidget(saveAsHistory);
    historyButtonsLayout->addSpacerItem(new QSpacerItem(1,1));
    historyButtonsLayout->addWidget(openHistory);

    filterEdit = new QLineEdit();
    filterEdit->setPlaceholderText("Search");
    connect(filterEdit, &QLineEdit::textChanged, this, &HttpClientWindowv2::filterHistoryTreeView);
    QVBoxLayout *historyButtonAndListLayout = new QVBoxLayout(this);
    historyButtonAndListLayout->addWidget(filterEdit);
    historyButtonAndListLayout->addLayout(historyButtonsLayout);
    historyButtonAndListLayout->addWidget(historyListView);
    QWidget *historyButtonAndListLayoutWidget = new QWidget(this);
    historyButtonAndListLayoutWidget->setLayout(historyButtonAndListLayout);

    communicationErrorLabel = new QLabel("Communication Log",this);
    communicationErrorEditor = new TextEditor(parent);
    communicationErrorEditor->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(communicationErrorEditor,&TextEditor::customContextMenuRequested,this,&HttpClientWindowv2::communicationErrorEditorContextMenuEvent);
    historyErrorLayout = new QVBoxLayout(this);
    historyErrorLayout->setMargin(0);
    historyErrorLayout->addWidget(communicationErrorLabel);
    historyErrorLayout->addWidget(communicationErrorEditor);
    QWidget *historyErrorLayoutWidget = new QWidget(this);
    historyErrorLayoutWidget->setLayout(historyErrorLayout);

    historySplitter = new QSplitter(this);
    historySplitter->setOrientation(Qt::Vertical);
    historySplitter->addWidget(historyButtonAndListLayoutWidget);
    historySplitter->addWidget(historyErrorLayoutWidget);
    connect(historySplitter, &QSplitter::splitterMoved, [=](int pos, int index) {
        Q_UNUSED(pos);
        Q_UNUSED(index);
        Configurations::get().setHttpClientV2HistorySplitterSizes(historySplitter->sizes());
    });
    QList<int> sizes = Configurations::get().getHttpClientV2HistorySplitterSizes();
    if(sizes.size() > 0){
        historySplitter->setSizes(sizes);
    }

    mainSplitter = new QSplitter(parent);
    mainSplitter->addWidget(requestHeadersWidget);
    mainSplitter->addWidget(responseWindget);
    mainSplitter->addWidget(historySplitter);
    connect(mainSplitter, &QSplitter::splitterMoved, [=](int pos, int index) {
        Q_UNUSED(pos);
        Q_UNUSED(index);
        Configurations::get().setHttpClientV2MainSplitterSizes(mainSplitter->sizes());
    });
    sizes = Configurations::get().getHttpClientV2MainSplitterSizes();
    if(sizes.size() > 0){
        mainSplitter->setSizes(sizes);
    }

    mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(mainSplitter);
    setLayout(mainLayout);

    future.waitForFinished();
    communicationErrorEditor->appendPlainText(jsonLoadErrors);
}

HttpClientWindowv2::~HttpClientWindowv2()
{

}
void HttpClientWindowv2::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
}

void HttpClientWindowv2::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
}
void HttpClientWindowv2::sendRequest(){

    currentSession.clear();
    communicationErrorEditor->clear();

    currentSession.requestData = requestHeadersWidget->getRequestData();
    if(currentSession.requestData.url.empty()){
        communicationErrorEditor->appendPlainText("Url empty");
        return;
    }
    QUrl url(QString::fromStdString(currentSession.requestData.url));
    if(!url.isValid()){
        communicationErrorEditor->appendPlainText(url.errorString());
        return;
    }
    responseWindget->initState();

    try
    {
        communicationErrorEditor->appendPlainText("Sending Request");

        //#ifdef __linux
        //        std::string uu = currentSession.requestData.url.toStdString();
        //        qDebug() << QString::fromStdString(uu);
        //        HttpClient cl(uu);
        //#else
        HttpClient cl;
        cl.setScheme(url.scheme().toStdString());
        currentSession.requestData.host = url.host().toStdString();
        cl.setHost(currentSession.requestData.host);
        currentSession.requestData.protocolType = url.scheme().toStdString();
        if(currentSession.requestData.protocolType == "http")
            cl.setPort(url.port(80));
        else
            cl.setPort(url.port(443));
        std::string decodedPath = url.path(QUrl::FullyDecoded).toStdString();
        currentSession.requestData.path = decodedPath;
        cl.setPath(decodedPath);
        std::string decodedQuery = url.query(QUrl::FullyEncoded).toStdString();
        cl.setQuery(decodedQuery);
        //#endif
        cl.setHttpVersion(currentSession.requestData.version);

        cl.setMethod(currentSession.requestData.requestType);
        //todo kur eshte put dhe delete clienti nuk duhet te dergoje body dhe content-lenght
        //ndoshta duhet te jete e konfigurueshme, strict dhe loose
//        if( (currentSession.requestData.requestType == "POST" ) ||
//            (currentSession.requestData.requestType == "PUT" ) ||
//                (currentSession.requestData.requestType == "PATCH" )
//                ){
//            cl.setRequestBody(currentSession.requestData.body);
//        }else{
//            //todo remove Content-Length
//        }
        cl.setRequestBody(currentSession.requestData.body);
        for(auto header : currentSession.requestData.headers){
            cl.insertHeader(header.first,header.second);
        }

        cl.setProxyHost(Configurations::get().getProxyHost());
        cl.setProxyPort(Configurations::get().getProxyPort());
        std::string errormsg;
        std::vector<std::string> requestStats;
        responseTime.restart();
        if(!cl.request(requestStats, errormsg)){
            communicationErrorEditor->appendPlainText(QString::fromStdString(errormsg));
            return;
        }

        requestHeadersWidget->setRawRequest(QString::fromStdString(cl.getRawRequest()));
        currentSession.responseData.responseTime = responseTime.elapsed();
        communicationErrorEditor->appendPlainText("finished");
        responseWindget->setRawResponse(QString::fromStdString(cl.getRawResponse()));
        std::map<std::string,std::string> respHeaders = cl.getResponseHeaders();

        for(auto header : respHeaders){
            currentSession.responseData.headers[header.first] = header.second;
        }

        currentSession.responseData.responseBodyLength = cl.getResponseBodySize();
        currentSession.responseData.responseStatusDescription = cl.getResponseReason();
        currentSession.responseData.responseStatusCode = cl.getResponseStatus();
        currentSession.responseData.responseBody = cl.getResponseBody();

        responseWindget->setResponseData(currentSession.responseData);
        for(auto stat : requestStats){
            responseWindget->addStatistic(QString::fromStdString(stat));
        }
        currentSession.requestData.requestTime = QDateTime::currentMSecsSinceEpoch();
        insertHistoryListView(currentSession);
        requestHistory.push_back(currentSession);
    }catch (std::exception ex){
        communicationErrorEditor->appendPlainText(QString::fromStdString(ex.what()));
    }
}
void HttpClientWindowv2::responseWidgetError(QString error)
{
    communicationErrorEditor->appendPlainText(error);
}

void HttpClientWindowv2::saveAsHistoryToFile()
{
    if(lastFileSaveDir.isEmpty()){
        lastFileSaveDir = QString::fromStdString(Utils::getDataDirectory());
    }
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save File"), lastFileSaveDir, tr("Text Files (*.txt *.json)"));
    if(!fileName.isEmpty()){
        QFileInfo filePath(fileName);

        lastFileSaveDir = filePath.path();
        historyFileNameAndPath = fileName;
        Configurations::get().setHistoryLastSavedDir(lastFileSaveDir.toStdString());
        saveHistoryToFile();
    }
}

void HttpClientWindowv2::loadHistoryFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open File"), lastFileSaveDir, tr("Text Files (*.txt *.json)"));
    if(!fileName.isEmpty()){
        historyFileNameAndPath = fileName;
        loadSavedRequestHistory();
    }
}

void HttpClientWindowv2::communicationErrorEditorContextMenuEvent(const QPoint &pos)
{
    QPoint globalPos = communicationErrorEditor->mapToGlobal(pos);
    QMenu *menu  = communicationErrorEditor->editorMenu();
    if(!communicationErrorEditor->textCursor().selectedText().isEmpty()){
        menu->addAction("Send To Notepad",  this, &HttpClientWindowv2::sendToNotepadEditor);
    }
    menu->exec(globalPos);
    delete menu;
}

void HttpClientWindowv2::sendToNotepadEditor()
{
    applicationNodepadEditor->appendTextToEditor(communicationErrorEditor->textCursor().selectedText());
}

void HttpClientWindowv2::filterHistoryTreeView(const QString &text)
{
    historyTreeViewFilter->setFilterRegExp(QRegExp(text, Qt::CaseInsensitive, QRegExp::Wildcard));
}
void HttpClientWindowv2::insertHistoryListView(HttpSession &data){
    QModelIndex index = historyTreeModel->index(historyTreeModel->rowCount()-1,0,QModelIndex());
    QAbstractItemModel *model = historyListView->model();

    if (!model->insertRow(index.row()+1, index.parent()))
        return;

    QModelIndex child = historyTreeModel->index(index.row()+1, 0, index.parent());
    historyTreeModel->setData(child, QVariant(QString::fromStdString(data.requestData.requestType)), Qt::EditRole);

    child = historyTreeModel->index(index.row()+1, 1, index.parent());
    historyTreeModel->setData(child, QVariant(QString::fromStdString(data.requestData.host)), Qt::EditRole);

    child = historyTreeModel->index(index.row()+1, 2, index.parent());
    historyTreeModel->setData(child, QVariant(QString::fromStdString(data.requestData.path)) , Qt::EditRole);

    child = historyTreeModel->index(index.row()+1, 3, index.parent());
    historyTreeModel->setData(child, QVariant(data.responseData.responseStatusCode), Qt::EditRole);

    child = historyTreeModel->index(index.row()+1, 4, index.parent());
    historyTreeModel->setData(child, QVariant(QDateTime::fromMSecsSinceEpoch(data.requestData.requestTime).toString("yyyy-dd-M h:m:s.z")), Qt::EditRole);

    child = historyTreeModel->index(index.row()+1, 5, index.parent());
    historyTreeModel->setData(child, QVariant(QString::fromStdString(data.requestData.url)), Qt::EditRole);
    //historyListView->selectionModel()->setCurrentIndex(model->index(0, 0, index),QItemSelectionModel::SelectCurrent);

    child = historyTreeModel->index(index.row()+1, 6, index.parent());
    historyTreeModel->setData(child, QVariant(_nr_history), Qt::EditRole);

    index = historyTreeModel->index(historyTreeModel->rowCount()-1,0,QModelIndex());
    model = historyListView->model();
    if (!historyTreeModel->insertRow(0, index))
        return;

    child = historyTreeModel->index(0, 1, index);
    historyTreeModel->setData(child, QVariant(QString::fromStdString(data.requestData.protocolType)), Qt::EditRole);

    child = historyTreeModel->index(0, 2, index);
    historyTreeModel->setData(child, QVariant(data.requestData.version), Qt::EditRole);

    _nr_history++;
    //historyListView->selectionModel()->setCurrentIndex(child,QItemSelectionModel::Clear);//otherwise all rows would be selected
    //historyListView->selectionModel()->setCurrentIndex(child,QItemSelectionModel::SelectCurrent);//select last(current) row so that the others are added below
    //historyListModel->insertRows(historyListModel->rowCount(), 1);
    //historyListModel->setData(historyListModel->index(historyListModel->rowCount()-1), data);
}

void HttpClientWindowv2::removeRequestFromHistory(){
    QAbstractItemModel *model = historyListView->model();
    QModelIndexList selectedList = historyListView->selectionModel()->selectedRows();
    //iterate in reverse beacause, foreward iteration would not work. Just think about it
    std::for_each(selectedList.rbegin(),selectedList.rend(),[this](QModelIndex selectedItem){
        if(selectedItem.parent().isValid()){
            selectedItem = selectedItem.parent();
        }
        //funksionon duke marre parasysh dy nivele
        int selectedindex = selectedItem.row();
        requestHistory.removeAt(selectedindex);
        historyTreeModel->removeRow(selectedindex,selectedItem.parent());
    });
}

void HttpClientWindowv2::historyListViewClicked(const QModelIndex &index){
    QModelIndex indexi;// = historyTreeModel->selectionModel()->currentIndex();
    QAbstractItemModel *model = historyListView->model();
    if(index.parent().isValid()){//duke marre parasysh dy nivele te tree
        indexi = model->parent(index);
    }else{
        indexi = index;
    }
    int selectedindex = indexi.row();
    HttpSession hist = requestHistory.at(selectedindex);
    requestHeadersWidget->setRequestData(hist.requestData);
    responseWindget->setResponseData(hist.responseData);
}

void HttpClientWindowv2::removeAllHistory()
{
    QAbstractItemModel *model = historyListView->model();
    model->removeRows(0,model->rowCount());
    requestHistory.clear();
    _nr_history = 0;
    //historyListModel->removeRows(0,historyListModel->rowCount());
}


void HttpClientWindowv2::saveHistoryToFile(){
    if(historyFileNameAndPath.isEmpty()){
        saveAsHistoryToFile();
    }else{
        saveHistoryFile();
    }

}

void HttpClientWindowv2::historyListViewContextMenuEvent(const QPoint &pos)
{
    QPoint globalPos = historyListView->mapToGlobal(pos);

    // Create menu and insert some actions
    QMenu myMenu;
    myMenu.addAction("Delete",  this, &HttpClientWindowv2::removeRequestFromHistory);
    myMenu.addAction("Erase All History",  this, &HttpClientWindowv2::removeAllHistory);

    // Show context menu at handling position
    myMenu.exec(globalPos);
}
void HttpClientWindowv2::loadSavedRequestHistory(){
    QFile histFile(historyFileNameAndPath);
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
            _nr_history = 0;
            requestHistory.clear();
            historyTreeModel->removeRows(0,historyTreeModel->rowCount());
            for (auto it = historyJsonDoc["RequestSessionHistory"].begin(); it != historyJsonDoc["RequestSessionHistory"].end(); ++it){
                HttpSession reqSession;
                reqSession.deSerialize(*it);
                requestHistory.push_back(reqSession);
                insertHistoryListView(reqSession);
            }
        }catch (std::exception ex){
            jsonLoadErrors = QString::fromStdString(ex.what());
        }
    }
}

int HttpClientWindowv2::saveHistoryFile(QString fileNameAndPath)
{
    nlohmann::json allHistory;
    for(auto hist : requestHistory){
        nlohmann::json singleHistory;
        hist.serialise(singleHistory);
        allHistory.push_back(singleHistory);
    }
    nlohmann::json root;
    root["RequestSessionHistory"] = allHistory;

    QFile file;
    if(fileNameAndPath.isEmpty())
            file.setFileName(historyFileNameAndPath);
    else{
        file.setFileName(fileNameAndPath);
    }
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::information(this, tr("Unable to open file"), file.errorString());
        return -1;
    }
    std::string jsonstring = root.dump(4);
    file.write(jsonstring.c_str(),jsonstring.length());
    file.close();
    return 0;
}
