#include "responsewidget.h"
#include "configurations.h"
#include "notepad.h"
ResponseWidget::ResponseWidget(Notepad *notepadEditor, QWidget *parent)
    : QWidget(parent),
      applicationNodepadEditor(notepadEditor)
{
    responseHeadersEditor = new TextEditor(this);
    responseHeadersEditor->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(responseHeadersEditor,&TextEditor::customContextMenuRequested,this,&ResponseWidget::responseHeadersEditorContextMenuEvent);

    jsonBeautifyCheckbox = new QCheckBox("JSON Formater",this);
    responseHttpStatusLabel = new QLabel("Response Status",this);
    responseTimeLabel  = new QLabel("Response Time",this);
    responseBodyLengthLabel = new QLabel("Body Length",this);
    reponseJsonBeautifyLayout = new QHBoxLayout();
    reponseJsonBeautifyLayout->addWidget(responseTimeLabel);
    reponseJsonBeautifyLayout->addWidget(responseHttpStatusLabel);
    reponseJsonBeautifyLayout->addWidget(responseBodyLengthLabel);
    reponseJsonBeautifyLayout->addWidget(jsonBeautifyCheckbox);

    responseBodyEditor = new TextEditor(this);
    responseBodyEditor->lineNumbersVisible(true);
    responseBodyEditor->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(responseBodyEditor,&TextEditor::customContextMenuRequested,this,&ResponseWidget::responseBodyEditorContextMenuEvent);

    QVBoxLayout *bodyLayout = new QVBoxLayout(this);
    bodyLayout->setMargin(0);
    bodyLayout->addLayout(reponseJsonBeautifyLayout);
    bodyLayout->addWidget(responseBodyEditor);
    QWidget *bodyLayoutWidget = new QWidget(this);
    bodyLayoutWidget->setLayout(bodyLayout);

    responseLayout = new QVBoxLayout();
    responseHeadersLabel = new QLabel("Response Headers",this);
    responseLayout->setMargin(0);
    responseLayout->addWidget(responseHeadersLabel);
    responseLayout->addWidget(responseHeadersEditor);
    QWidget *responseLayoutWidget = new QWidget(this);
    responseLayoutWidget->setLayout(responseLayout);

    connect(jsonBeautifyCheckbox, &QCheckBox::toggled, this,&ResponseWidget::jsonResponeBodyBeautify);

    mainSplitter = new QSplitter(this);
    mainSplitter->setOrientation(Qt::Vertical);
    mainSplitter->addWidget(responseLayoutWidget);
    mainSplitter->addWidget(bodyLayoutWidget);

    rawResponseEdit = new QPlainTextEdit ();

    responseStatisticsEdit = new QPlainTextEdit();
    bodyWebView = new QWebEngineView(this);

    responseTabWidget = new QTabWidget(this);
    responseTabWidget->addTab(mainSplitter,"Response");
    responseTabWidget->addTab(rawResponseEdit,"Raw Response");
    responseTabWidget->addTab(responseStatisticsEdit,"Statistics");
    responseTabWidget->addTab(bodyWebView, "Body WebView");
    connect(responseTabWidget, &QTabWidget::currentChanged, this, &ResponseWidget::tabChanged);

    connect(mainSplitter, &QSplitter::splitterMoved, [=](int pos, int index) {
        Q_UNUSED(pos);
        Q_UNUSED(index);
        Configurations::get().setHttpClientV2ResponseSplitterSizes(mainSplitter->sizes());
    });
    QList<int> sizes = Configurations::get().getHttpClientV2ResponseSplitterSizes();
    if(sizes.size() > 0){
        mainSplitter->setSizes(sizes);
    }

    mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(responseTabWidget);

    setLayout(mainLayout);
}


void ResponseWidget::initState()
{
    responseTimeLabel->setText("Respone Time");
    responseBodyLengthLabel->setText("Body Length");
    responseHttpStatusLabel->setText("Response Status");
    responseBodyEditor->clear();
    responseHeadersEditor->clear();
    responseStatisticsEdit->clear();
    jsonBeautifyCheckbox->setCheckState(Qt::CheckState::Unchecked);
}

void ResponseWidget::setResponseData(ResponseDataCtx responseData)
{
    initState();

    responseHeadersEditor->clear();

    QStringList keywordPatterns;
    for(auto const i : responseData.headers){
        keywordPatterns << QString::fromStdString(i.first);
    }
    responseHeadersEditor->clearHighlightKeywords();
    responseHeadersEditor->setHightlightKeywords(keywordPatterns);
    for(auto const i : responseData.headers){
        QString headerKey = QString::fromStdString(i.first);
        QString headerValue = QString::fromStdString(i.second);
        responseHeadersEditor->appendPlainText(headerKey + ":" + headerValue);
    }
    QString responseBody = QString::fromStdString(responseData.responseBody);

    responseBodyEditor->appendPlainText(responseBody);
    bodyWebView->setHtml(responseBody);
    jsonBeautifyCheckbox->setChecked(true);
    responseTimeLabel->setText("Response Time: " + QString::number(responseData.responseTime) + "ms");
    responseHttpStatusLabel->setText(QString::number(responseData.responseStatusCode) +" " + QString::fromStdString(responseData.responseStatusDescription));
    responseBodyLengthLabel->setText("Body Length: " + QString::number(responseData.responseBody.length()) + " B");
}

void ResponseWidget::setRawResponse(QString rawResponse)
{
    rawResponseEdit->clear();
    rawResponseEdit->appendPlainText(rawResponse);
}

void ResponseWidget::rawWindgetVisible(bool visible)
{
    if(!visible)
        responseTabWidget->removeTab(1);
}

void ResponseWidget::addStatistic(QString statLine)
{
    responseStatisticsEdit->appendPlainText(statLine);
}

void ResponseWidget::jsonResponeBodyBeautify(bool beautify)
{
    if(responseBodyEditor->toPlainText().size() > 0){
        QByteArray ba;
        ba.append(responseBodyEditor->toPlainText());
        QJsonParseError err;
        QJsonDocument jsondoc = QJsonDocument::fromJson(ba,&err);
        if(err.error != QJsonParseError::NoError){
            //emit error("JSON parsing error: " + err.errorString());
            jsonBeautifyCheckbox->setChecked(false);
            return;
        }
        if(!jsondoc.isNull()){
            responseBodyEditor->clear();
            if(beautify){
                responseBodyEditor->clearHighlightKeywords();
                responseBodyEditor->addHighlightRegex(R"((.*)[:]\s)");
                responseBodyEditor->appendPlainText(jsondoc.toJson(QJsonDocument::Indented));
            }else{
                responseBodyEditor->appendPlainText(jsondoc.toJson(QJsonDocument::Compact));
            }
        }
    }
}

void ResponseWidget::responseBodyEditorContextMenuEvent(const QPoint &pos)
{
    QPoint globalPos = responseBodyEditor->mapToGlobal(pos);
    QMenu *menu  = responseBodyEditor->editorMenu();
    if(!responseBodyEditor->textCursor().selectedText().isEmpty()){
        menu->addAction("Send To Notepad",  this, &ResponseWidget::sendToNotepadEditor);
    }
    menu->exec(globalPos);
    delete menu;
}

void ResponseWidget::responseHeadersEditorContextMenuEvent(const QPoint &pos)
{
    QPoint globalPos = responseHeadersEditor->mapToGlobal(pos);
    QMenu *menu  = responseHeadersEditor->editorMenu();
    if(!responseHeadersEditor->textCursor().selectedText().isEmpty()){
        menu->addAction("Send To Notepad",  this, &ResponseWidget::sendToNotepadEditorFromResponseHeadersEditor);
    }
    menu->exec(globalPos);
    delete menu;
}

void ResponseWidget::sendToNotepadEditor()
{
    applicationNodepadEditor->appendTextToEditor(responseBodyEditor->textCursor().selectedText());
}

void ResponseWidget::sendToNotepadEditorFromResponseHeadersEditor()
{
    applicationNodepadEditor->appendTextToEditor(responseHeadersEditor->textCursor().selectedText());
}

void ResponseWidget::tabChanged(int index)
{
    if(index == 3){//webview

    }
}

bool ResponseWidget::isJsonData(QString &data)
{
    QByteArray ba;
    ba.append(data);
    QJsonParseError err;
    QJsonDocument jsondoc = QJsonDocument::fromJson(ba,&err);
    if(err.error != QJsonParseError::NoError){
        return false;
    }
    return true;
}
