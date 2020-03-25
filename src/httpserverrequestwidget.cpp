#include "httpserverrequestwidget.h"

HttpServerRquesteWidget::HttpServerRquesteWidget(QWidget *parent) : QWidget(parent)
{
    jsonBeautifyCheckbox = new QCheckBox("JSON Formater",this);
    requestTimeLabel  = new QLabel("Request Time",this);
    requestBodyLengthLabel = new QLabel("Body Length",this);
    requestJsonBeautifyLayout = new QHBoxLayout();
    requestJsonBeautifyLayout->addWidget(requestTimeLabel);
    requestJsonBeautifyLayout->addWidget(requestBodyLengthLabel);
    requestJsonBeautifyLayout->addWidget(jsonBeautifyCheckbox);

    requestHeadersEditor = new QPlainTextEdit(this);
    requestBodyEditor = new QPlainTextEdit(this);
    requestLayout = new QVBoxLayout();
    requestHeadersLabel = new QLabel("Request Headers",this);
    requestHeadersLabel->setMargin(0);
    requestLayout->addWidget(requestHeadersLabel);
    requestLayout->addWidget(requestHeadersEditor);
    requestLayout->addLayout(requestJsonBeautifyLayout);
    requestLayout->addWidget(requestBodyEditor);
    requestLayout->setMargin(0);


    connect(jsonBeautifyCheckbox, &QCheckBox::toggled, this,&HttpServerRquesteWidget::jsonRequestBodyBeautify);

    requestMainTabWidget = new QWidget(this);
    requestMainTabWidget->setLayout(requestLayout);

    rawRequestEdit = new QPlainTextEdit ();
    rawRequestLayout = new QVBoxLayout();
    rawRequestLayout->setMargin(0);
    rawRequestLayout->addWidget(rawRequestEdit);
    rawRequestTabWidget = new QWidget();
    rawRequestTabWidget->setLayout(rawRequestLayout);

    requestTabWidget = new QTabWidget(this);
    requestTabWidget->addTab(requestMainTabWidget,"Request");
    requestTabWidget->addTab(rawRequestTabWidget,"Raw Request");

    mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setMargin(0);
    mainLayout->addWidget(requestTabWidget);

    setLayout(mainLayout);
}


void HttpServerRquesteWidget::initState()
{
    requestTimeLabel->setText("Respone Time");
    requestBodyLengthLabel->setText("Body Length");
    //requestHttpStatusLabel->setText("request Status");
    requestBodyEditor->clear();
    requestHeadersEditor->clear();
    jsonBeautifyCheckbox->setCheckState(Qt::CheckState::Unchecked);
}

void HttpServerRquesteWidget::setRequestData(RequestCtx requestData)
{
    initState();

    requestHeadersEditor->clear();
    for(auto header : requestData.headers){
        requestHeadersEditor->appendPlainText(QString::fromStdString(header.first) + ":" + QString::fromStdString(header.second));
    }

    requestBodyEditor->appendPlainText(QString::fromStdString(requestData.body));
    requestTimeLabel->setText("Request Time: " + QString::number(requestData.requestTime) + "ms");
    requestBodyLengthLabel->setText("Body Length: " + QString::number(requestData.body.length()) + " B");
}

void HttpServerRquesteWidget::setRawRequest(QString rawRequest)
{
    rawRequestEdit->clear();
    rawRequestEdit->appendPlainText(rawRequest);
}


void HttpServerRquesteWidget::jsonRequestBodyBeautify(bool beautify)
{
    QByteArray ba;
    ba.append(requestBodyEditor->toPlainText());
    QJsonParseError err;
    QJsonDocument jsondoc = QJsonDocument::fromJson(ba,&err);
    if(err.error != QJsonParseError::NoError){
        emit error("JSON parsing error: " + err.errorString());
        return;
    }
    if(!jsondoc.isNull()){
        requestBodyEditor->clear();
        if(beautify){
            requestBodyEditor->appendPlainText(jsondoc.toJson(QJsonDocument::Indented));
        }else{
            requestBodyEditor->appendPlainText(jsondoc.toJson(QJsonDocument::Compact));
        }
    }
}
