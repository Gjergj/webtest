#include "requestheaderswidget.h"
#include "configurations.h"
#include "notepad.h"
#include <QQuickView>
#include <QQmlContext>
#include <QQuickWidget>
#include <documenthandler.h>
RequestHeadersWidget::RequestHeadersWidget(Notepad *notepadEditor, QWidget *parent) :
    QWidget(parent), _modifyHeadersFromGui(true)
{
    applicationNodepadEditor = notepadEditor;

    requestHeaderNameLayout = new QVBoxLayout();
    requestHeaderNameLayout->setMargin(0);

    requestHeaderLayout = new QHBoxLayout();
    requestHeaderLayout->setMargin(0);
    requestHeaderLayout->addLayout(requestHeaderNameLayout);


    requestHeaderModel = new RequestHeaderModel(&reqData);
    headerListModel = new HeaderListModel();
    urlencodedBodyModel = new UrlencodedBodyModel();
    queryStringModel = new QueryStringModel();
    QQuickWidget *container = new QQuickWidget(parent);
    container->rootContext()->setContextProperty("requestHeaderModel", requestHeaderModel);
    container->rootContext()->setContextProperty("requestHeaderWidget", this);
    container->rootContext()->setContextProperty("headerListModel", headerListModel);
    container->rootContext()->setContextProperty("urlencodedBodyModel", urlencodedBodyModel);
    container->rootContext()->setContextProperty("queryStringModel", queryStringModel);
    qmlRegisterType<DocumentHandler>("org.webtest.example", 1, 0, "DocumentHandler");
    container->setResizeMode(QQuickWidget::SizeRootObjectToView);
    container->setAttribute(Qt::WA_AlwaysStackOnTop);
    container->setFocusPolicy(Qt::ClickFocus);
    //container->setClearColor(Qt::transparent);
    container->setSource(QUrl("qrc:/qml/main.qml"));
    requestHeaderLayout->addWidget(container);

    QWidget* headersLayoutWidget = new QWidget(this);
    headersLayoutWidget->setLayout(requestHeaderLayout);

    headersMainTabLayout = new QVBoxLayout();
    headersMainTabLayout->setMargin(0);
    headersMainTabLayout->addWidget(headersLayoutWidget);

    headersMainTabWidget = new QWidget(this);
    headersMainTabWidget->setLayout(headersMainTabLayout);

    rawRequestEdit = new QPlainTextEdit ();
    rawRequestLayout = new QVBoxLayout();
    rawRequestLayout->setMargin(0);
    rawRequestLayout->addWidget(rawRequestEdit);
    rawRequestTabWidget = new QWidget();
    rawRequestTabWidget->setLayout(rawRequestLayout);

    requestHeaderTabWidget = new QTabWidget(this);
    requestHeaderTabWidget->addTab(headersMainTabWidget,"Request");
    requestHeaderTabWidget->addTab(rawRequestTabWidget,"Raw Request");

    mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setMargin(0);
    mainLayout->addWidget(requestHeaderTabWidget);

    setLayout(mainLayout);
}

void RequestHeadersWidget::setRequestData(RequestCtx requestData)
{
    removeAllHeaders();
    reqData = requestData;
    reqData.headers.clear();
    setHttpMethod(QString::fromStdString(reqData.requestType));
    setUrl(QString::fromStdString(reqData.url));
    setBodyText(QString::fromStdString(reqData.body));
    removeAllHeaders();//workaround, kur caktohet body ne qml shtohet Content-Length
    setModifyHeadersFromGui(false);
    for(auto it : requestData.headers){
        if(!headerListModel->contains(it.first))
            headerListModel->insertHeader(it.first);
        requestHeaderModel->insertHeader(it.first,it.second);
    }
    setModifyHeadersFromGui(true);
}

RequestCtx RequestHeadersWidget::getRequestData()
{
    reqData.requestType = _httpMethod.toStdString();
    reqData.url = getUrl().toStdString();
    reqData.body = getBodyText().toStdString();
    reqData.version = _httpVersion;
    return reqData;
}

void RequestHeadersWidget::setRawRequest(QString rawRequest)
{
    rawRequestEdit->clear();
    rawRequestEdit->appendPlainText(rawRequest);
}

void RequestHeadersWidget::rawWindgetVisible(bool visible)
{
    if(!visible)
        requestHeaderTabWidget->removeTab(1);
}

int RequestHeadersWidget::addHeader(const QString headerField, QString headerValue)
{
    headerListModel->insertHeader(headerField.toStdString());
    requestHeaderModel->insertHeader(headerField.toStdString(),headerValue.toStdString());
    return 0;
}

void RequestHeadersWidget::removeAllHeaders()
{
    requestHeaderModel->removeHeaders(0,requestHeaderModel->rowCount());
}

void RequestHeadersWidget::addContentLength()
{
    addHeader("Content-Length",QString::number(_bodyText.length()));
}

void RequestHeadersWidget::sendToNotepadEditor(QString text)
{
    applicationNodepadEditor->appendTextToEditor(text);
}

QString RequestHeadersWidget::setQueryStringToUrl(QString oldUrl, QString querystring)
{
    QUrl currentUrl(oldUrl);
    currentUrl.setQuery(querystring);
    return currentUrl.toString();
}

QString RequestHeadersWidget::getQueryStringFromUrl(QString currentUrl)
{
    QUrl url(currentUrl);
    return url.query();
}

void RequestHeadersWidget::sendRequest()
{
    emit sendTheRequest();
}

void RequestHeadersWidget::updateContentLenght()
{
    int rowCount = requestHeaderModel->rowCount();
    int contentLengthRow = -1;
    for(int i = 0; i < rowCount; i++){
        std::string headerName = requestHeaderModel->getData(i,0).toString().toStdString();
        if(headerName == "Content-Length"){
            contentLengthRow = i;
            break;
        }
    }

    if(contentLengthRow < 0){
        addContentLength();
    }
    else{
        int length = _bodyText.length();
        requestHeaderModel->setData(contentLengthRow, 1, QString::number(length));
    }
}

void RequestHeadersWidget::copyHeadersAllToClipBoard()
{
    if(reqData.headers.size() > 0){
        QString allHeaders;
        for(auto header : reqData.headers){
            allHeaders += QString::fromStdString(header.first) + ":" + QString::fromStdString(header.second);
            allHeaders += "\r";
        }
        QClipboard *clipboard = QApplication::clipboard();
        QMimeData *mime = new QMimeData();
        mime->setData("text/plain;value:headers",allHeaders.toUtf8());
        clipboard->setMimeData(mime);
    }
}

void RequestHeadersWidget::copyHeadersToClipBoard(QList<int> rowsIndex)
{
    if(rowsIndex.size() > 0){
        QString allHeaders;
        for(auto header : rowsIndex){
            auto rowIt  = reqData.headers.begin();
            std::advance(rowIt, header);
            allHeaders += QString::fromStdString(rowIt->first) + ":" + QString::fromStdString(rowIt->second);
            allHeaders += "\r";
        }
        QClipboard *clipboard = QApplication::clipboard();
        QMimeData *mime = new QMimeData();
        mime->setData("text/plain;value:headers",allHeaders.toUtf8());
        clipboard->setMimeData(mime);
    }
}

void RequestHeadersWidget::pasteHeadersFromClipBoard()
{
    QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mime = clipboard->mimeData();
    QByteArray clipboardData = mime->data("text/plain;value:headers");
    QList<QByteArray> headersData = clipboardData.split('\r');
    for(auto header : headersData){
        QList<QByteArray> head = header.split(':');
        if(head.size() > 1){
            addHeader(QString(head.at(0)),QString(head.at(1)));
        }
    }
}

QString RequestHeadersWidget::getBodyText() const
{
    return _bodyText;
}

int RequestHeadersWidget::getSpliterSize()
{
    _spliterSize = Configurations::get().getHttpClientV2RequestSplitterSizes();
    return _spliterSize;
}

QString RequestHeadersWidget::getHttpVersion()
{
    if(_httpVersion == 10){
        return "HTTP 1.0";
    }else if(_httpVersion == 11){
        return "HTTP 1.1";
    }else{
        return "mmmmmmmmm";
    }
}

QString RequestHeadersWidget::getUrl() const
{
    return _url;
}

QString RequestHeadersWidget::getHttpMethod() const
{
    return _httpMethod;
}

bool RequestHeadersWidget::getmodifyHeadersFromGui() const
{
    return _modifyHeadersFromGui;
}

void RequestHeadersWidget::setBodyText(const QString &arg)
{
    if (_bodyText != arg) {
        _bodyText = arg;
        emit bodyTextChanged();
    }
}

void RequestHeadersWidget::setSpliterSize(int size)
{
    _spliterSize = size;
    Configurations::get().setHttpClientV2RequestSplitterSizes(_spliterSize);
    emit spliterSizeChanged();
}

void RequestHeadersWidget::setHttpVersion(QString version)
{
    if(version == "HTTP 1.0"){
        _httpVersion = 10;
    }else if(version == "HTTP 1.1"){
        _httpVersion = 11;
    }else{
        _httpVersion = 100;
    }
    emit httpVersionChanged();
}

void RequestHeadersWidget::setUrl(QString url)
{
    _url = url;
    emit urlChanged();
}

void RequestHeadersWidget::setHttpMethod(QString httpMethod)
{
    if(httpMethod != _httpMethod){
        _httpMethod = httpMethod;
        emit httpMethodChanged();
    }
}

void RequestHeadersWidget::setModifyHeadersFromGui(bool modifyHeadersFromGui)
{
    if(modifyHeadersFromGui != modifyHeadersFromGui){
        modifyHeadersFromGui = _modifyHeadersFromGui;
        emit modifyHeadersFromGuiChanged();
    }
}

void RequestHeadersWidget::removeHeader(int row)
{
    requestHeaderModel->removeHeader(row);
}

void RequestHeadersWidget::listViewClicked(const QModelIndex &index){
    int selectedindex = index.row();
    auto it = reqData.headers.begin();
    std::advance(it, selectedindex);
}

