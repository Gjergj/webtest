#include "httpserverresponsewidget.h"

HttpServerResponseWidget::HttpServerResponseWidget(QWidget *parent) : QWidget(parent)
{

    responseBodyEditor = new QPlainTextEdit(this);

    putResponseButton = new QRadioButton("PUT",this);
    postResponseButton = new QRadioButton("POST",this);
    getResponseButton = new QRadioButton("GET",this);
//    connect(getResponseButton, &QRadioButton::toggled, [=](bool checked) {
//        responseBodyEditor->setReadOnly(checked);
//    });
    getResponseButton->setChecked(true);
    patchResponseButton = new QRadioButton("PATCH",this);
    deleteResponseButton = new QRadioButton("DELETE",this);
//    connect(deleteResponseButton, &QRadioButton::toggled, [=](bool checked) {
//        responseBodyEditor->setReadOnly(checked);
//    });
    httpMethodRadioButtonLayout = new QHBoxLayout();
    httpMethodRadioButtonLayout->addWidget(putResponseButton);
    httpMethodRadioButtonLayout->addWidget(postResponseButton);
    httpMethodRadioButtonLayout->addWidget(getResponseButton);
    httpMethodRadioButtonLayout->addWidget(patchResponseButton);
    httpMethodRadioButtonLayout->addWidget(deleteResponseButton);

    urlTextEdit = new QLineEdit(this);
    urlTextEdit->setPlaceholderText("Path");
    addUrlButton = new QPushButton("ADD PATH",this);
    connect(addUrlButton,&QPushButton::clicked,this,&HttpServerResponseWidget::addUrlButtonPressed);
    urlLayout = new QHBoxLayout();
    urlLayout->addWidget(urlTextEdit);
    urlLayout->addWidget(addUrlButton);

    responseStatus = new QSpinBox(this);
    responseStatus->setRange(0,100000);
    responseStatus->setValue(200);
    responseStatus->setSingleStep(1);
    responseDescription = new QLineEdit(this);
    responseDescription->setPlaceholderText("Response Description");
    responseDescription->setText("OK");

    responseStatusLayout = new QHBoxLayout();
    responseStatusLayout->addWidget(responseStatus);
    responseStatusLayout->addWidget(responseDescription);


    responseHeaderNameEdit = new QLineEdit(this);
    responseHeaderNameEdit->setPlaceholderText("Header Name");
    responseHeaderNameLayout = new QVBoxLayout();
    responseHeaderNameLayout->addWidget(responseHeaderNameEdit);

    responseHeaderValueEdit = new QLineEdit(this);
    responseHeaderValueEdit->setPlaceholderText("Header Value");
    responseHeaderValueLayout = new QVBoxLayout();
    responseHeaderValueLayout->addWidget(responseHeaderValueEdit);

    addresponseHeaderButton = new QPushButton("Add Header", this);
    connect(addresponseHeaderButton,&QPushButton::clicked,this,&HttpServerResponseWidget::addHeaderButtonPressed);
    responseHeaderButtonLayout = new QVBoxLayout();
    responseHeaderButtonLayout->addWidget(addresponseHeaderButton);

    responseHeaderLayout = new QHBoxLayout();
    responseHeaderLayout->addLayout(responseHeaderNameLayout);
    responseHeaderLayout->addLayout(responseHeaderValueLayout);
    responseHeaderLayout->addLayout(responseHeaderButtonLayout);


    responseHeaderListLabel = new QLabel("Header List",this);
    responseHeadersListView = new QListView(this);
    responseHeadersListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    responseHeadersListView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(responseHeadersListView,&QListView::customContextMenuRequested,this,&HttpServerResponseWidget::listViewContextMenuEvent);
    connect(responseHeadersListView,&QListView::clicked,this,&HttpServerResponseWidget::listViewClicked);
    responseHeaderListModel = new QStringListModel();
    responseHeadersListView->setModel(responseHeaderListModel);

    urlListView = new QListView(this);
    urlListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    urlListView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(urlListView,&QListView::customContextMenuRequested,this,&HttpServerResponseWidget::urlListViewContextMenuEvent);
    connect(urlListView,&QListView::clicked,this,&HttpServerResponseWidget::urlListViewClicked);
    responseUrlListModel = new QStringListModel(this);
    urlListView->setModel(responseUrlListModel);

    responseBodyLabel = new QLabel("Response Body",this);
    addContentLengthLabel = new ClickableLabel("Content-Length",this) ;
    connect(addContentLengthLabel,&ClickableLabel::clicked,this,&HttpServerResponseWidget::addContentLength);
    bodyControlsLayout = new QHBoxLayout();
    bodyControlsLayout->addWidget(responseBodyLabel);
    bodyControlsLayout->addWidget(addContentLengthLabel);

    headersMainTabLayout = new QVBoxLayout();
    headersMainTabLayout->setMargin(0);

    headersMainTabLayout->addLayout(httpMethodRadioButtonLayout);
    headersMainTabLayout->addLayout(urlLayout);
    headersMainTabLayout->addLayout(responseStatusLayout);
    headersMainTabLayout->addLayout(responseHeaderLayout);
    headersMainTabLayout->addWidget(responseHeaderListLabel);
    headersMainTabLayout->addWidget(responseHeadersListView);
    headersMainTabLayout->addLayout(bodyControlsLayout);
    headersMainTabLayout->addWidget(responseBodyEditor);
    headersMainTabLayout->addWidget(urlListView);

    headersMainTabWidget = new QWidget(this);
    headersMainTabWidget->setLayout(headersMainTabLayout);

    rawResponseEdit = new QPlainTextEdit ();
    rawResponseLayout = new QVBoxLayout();
    rawResponseLayout->setMargin(0);
    rawResponseLayout->addWidget(rawResponseEdit);
    rawResponseTabWidget = new QWidget();
    rawResponseTabWidget->setLayout(rawResponseLayout);

    responseHeaderTabWidget = new QTabWidget(this);
    responseHeaderTabWidget->addTab(headersMainTabWidget,"Response");
    responseHeaderTabWidget->addTab(rawResponseTabWidget,"Raw Response");

    mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setMargin(0);
    mainLayout->addWidget(responseHeaderTabWidget);

    setLayout(mainLayout);
}

void HttpServerResponseWidget::setResponseData(std::list<RequestCtx> routeData)
{
    removeAllUrl();
    httpRoutes = routeData;

    for(auto route :httpRoutes){
        insertUrlListView(QString::fromStdString(route.path));
    }
    if(httpRoutes.size() > 0)
        setHeaderData(httpRoutes.front());

}
void HttpServerResponseWidget::setHeaderData(RequestCtx routeData)
{
    currentRoute.clear();
    currentRoute = routeData;
    if(routeData.requestType == "PUT"){
       putResponseButton->setChecked(true);
    }else if(routeData.requestType == "GET"){
        getResponseButton->setChecked(true);
    }else if(routeData.requestType == "POST"){
        postResponseButton->setChecked(true);
    }else if(routeData.requestType == "PATCH"){
        patchResponseButton->setChecked(true);
    }else if(routeData.requestType == "DELETE"){
        deleteResponseButton->setChecked(true);
    }
    urlTextEdit->setText(QString::fromStdString(routeData.path));
    responseStatus->setValue(routeData.responseStatusCode);
    responseDescription->setText(QString::fromStdString(routeData.responseStatusDescription));

    for(auto header : routeData.headers){
        insertHeaderListView(QString::fromStdString(header.first) + ":" + QString::fromStdString(header.second));
    }
    responseBodyEditor->appendPlainText(QString::fromStdString(routeData.body));
}

std::list<RequestCtx> HttpServerResponseWidget::getResponseData()
{
    return httpRoutes;
}

void HttpServerResponseWidget::setRawResponse(QString rawResponse)
{
    rawResponseEdit->clear();
    rawResponseEdit->appendPlainText(rawResponse);
}


void HttpServerResponseWidget::insertHeaderListView(QString data)
{
    responseHeaderNameEdit->clear();
    responseHeaderValueEdit->clear();

    responseHeaderListModel->insertRows(responseHeaderListModel->rowCount(), 1);
    responseHeaderListModel->setData(responseHeaderListModel->index(responseHeaderListModel->rowCount()-1), data);

}

void HttpServerResponseWidget::removeAllHeaders()
{
    currentRoute.headers.clear();
    responseHeaderListModel->removeRows(0,responseHeaderListModel->rowCount());
}

void HttpServerResponseWidget::addUrlButtonPressed()
{
    if(urlTextEdit->text().isEmpty()){
        emit error("Path can not be empty.");
        return;
    }
    if(responseDescription->text().isEmpty()){
        emit error("Response Description can not be empty.");
        return;
    }
    if(putResponseButton->isChecked()){
        currentRoute.requestType = putResponseButton->text().toStdString();
    }else if(getResponseButton->isChecked()){
        currentRoute.requestType = getResponseButton->text().toStdString();
    }else if(postResponseButton->isChecked()){
        currentRoute.requestType = postResponseButton->text().toStdString();
    }else if(patchResponseButton->isChecked()){
        currentRoute.requestType = patchResponseButton->text().toStdString();
    }else if(deleteResponseButton->isChecked()){
        currentRoute.requestType = deleteResponseButton->text().toStdString();
    }
    //todo workaround per http, browser lidhet vazhdimisht dhe bllokon server qe punon me poll.
    //http 1.1 client mban lidhjen hapur gjithe kohen, default
    responseHeaderNameEdit->setText("Connection");
    responseHeaderValueEdit->setText("close");
    addHeaderButtonPressed();

    currentRoute.path = urlTextEdit->text().toStdString();
    currentRoute.responseStatusCode = responseStatus->value();
    currentRoute.responseStatusDescription = responseDescription->text().toStdString();
//    if(putResponseButton->isChecked() ||
//       postResponseButton->isChecked() ||
//       patchResponseButton->isChecked() ){
//        currentRoute.responseData.responseBody = responseBodyEditor->toPlainText();
//        currentRoute.responseData.responseBodyLength = currentRoute.responseData.responseBody.length();
//    }
    currentRoute.body = responseBodyEditor->toPlainText().toStdString();
    currentRoute.bodyLength = currentRoute.body.length();
    insertUrlListView(QString::fromStdString(currentRoute.path));
    httpRoutes.push_back(currentRoute);
    clearGUI();
    currentRoute.clear();
}

void HttpServerResponseWidget::urlListViewContextMenuEvent(const QPoint &pos)
{
    QPoint globalPos = urlListView->mapToGlobal(pos);

    // Create menu and insert some actions
    QMenu myMenu;
    myMenu.addAction("Delete",  this, &HttpServerResponseWidget::removeUrlPressed);
    myMenu.addAction("Erase All",  this, &HttpServerResponseWidget::removeAllUrl);

    // Show context menu at handling position
    myMenu.exec(globalPos);
}
void HttpServerResponseWidget::removeUrlPressed()
{
    QModelIndex selectedItem = urlListView->currentIndex();
    int selectedindex = selectedItem.row();
    auto it = httpRoutes.begin();
    std::advance(it,selectedindex);
    httpRoutes.erase(it);
    responseUrlListModel->removeRows(selectedindex,1);
}

void HttpServerResponseWidget::removeAllUrl()
{
    clearGUI();
    currentRoute.clear();
    httpRoutes.clear();

    responseUrlListModel->removeRows(0,responseUrlListModel->rowCount());
}


void HttpServerResponseWidget::urlListViewClicked(const QModelIndex &index)
{
    clearGUI();
    int selectedindex = index.row();
    auto it = httpRoutes.begin();
    std::advance(it,selectedindex);
    RequestCtx r = *it;
    setHeaderData(r);
}

void HttpServerResponseWidget::addContentLength()
{
    responseHeaderNameEdit->setText("Content-Length");
    responseHeaderValueEdit->setText(QString::number(responseBodyEditor->document()->toPlainText().length()));
    addHeaderButtonPressed();
}


void HttpServerResponseWidget::insertUrlListView(QString data){

    responseUrlListModel->insertRows(responseUrlListModel->rowCount(), 1);
    responseUrlListModel->setData(responseUrlListModel->index(responseUrlListModel->rowCount()-1), data);
}

void HttpServerResponseWidget::clearGUI()
{
    urlTextEdit->clear();
    responseStatus->setValue(200);
    responseDescription->setText("OK");
    removeAllHeaders();
    responseHeaderNameEdit->clear();
    responseHeaderValueEdit->clear();
    responseBodyEditor->clear();    
}

void HttpServerResponseWidget::copyHeadersToClipBoard()
{
    if(currentRoute.headers.size() > 0){
        QString allHeaders;
        for(auto header : currentRoute.headers){
            allHeaders += QString::fromStdString(header.first) + ":" + QString::fromStdString(header.second);
            allHeaders += "\r\n";
        }
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setText(allHeaders);
    }
}

void HttpServerResponseWidget::addHeaderButtonPressed()
{
    if(responseHeaderNameEdit->text().isEmpty() || responseHeaderValueEdit->text().isEmpty()){
        emit error("Header name or value can not be empty.");
        return;
    }
    QString headerName = responseHeaderNameEdit->text();
    QString headerValue = responseHeaderValueEdit->text();
    currentRoute.headers.push_back(std::make_pair(headerName.toStdString(), headerValue.toStdString()));
    insertHeaderListView(headerName + ":" +headerValue);
}

void HttpServerResponseWidget::listViewContextMenuEvent(const QPoint &pos){
    QPoint globalPos = responseHeadersListView->mapToGlobal(pos);

    // Create menu and insert some actions
    QMenu myMenu;
    myMenu.addAction("Copy Headers to Clipboard", this, &HttpServerResponseWidget::copyHeadersToClipBoard);
    myMenu.addAction("Delete",  this, &HttpServerResponseWidget::removeHeaderPressed);
    myMenu.addAction("Erase All Headers",  this, &HttpServerResponseWidget::removeAllHeaders);

    // Show context menu at handling position
    myMenu.exec(globalPos);
}

void HttpServerResponseWidget::removeHeaderPressed()
{
    QModelIndex selectedItem = responseHeadersListView->currentIndex();
    int selectedindex = selectedItem.row();
    auto it = currentRoute.headers.begin();
    std::advance(it, selectedindex);
    currentRoute.headers.erase(it);
    responseHeaderListModel->removeRows(selectedindex,1);
}

void HttpServerResponseWidget::listViewClicked(const QModelIndex &index){
    int selectedindex = index.row();
    auto it = currentRoute.headers.begin();
    std::advance(it, selectedindex);

    responseHeaderNameEdit->setText(QString::fromStdString(it->first));
    responseHeaderValueEdit->setText(QString::fromStdString(it->second));
}
