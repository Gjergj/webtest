#include "notepad.h"
#include <QJsonParseError>
#include <QJsonDocument>
Notepad::Notepad(QWidget *parent) : QWidget(parent)
{
    editor = new TextEditor(parent);
    editor->lineNumbersVisible(true);
    connect(editor, &TextEditor::selectionChanged, this, &Notepad::selectionSizeChanged);
    connect(editor, &TextEditor::cursorPositionChanged, this, &Notepad::columnNoChanged);
    statusLayout = new QHBoxLayout();
    selectionLabel = new QLabel("Selection");
    selectionText = new QLabel();
    lineLabel = new QLabel("   Line");
    lineText = new QLabel();
    columnLabel = new QLabel("   Column");
    columnText = new QLabel();
    jsonBeautifyCheckbox = new QCheckBox("JSON Formater");
    connect(jsonBeautifyCheckbox, &QCheckBox::toggled, this,&Notepad::jsonResponeBodyBeautify);
    statusLayout->addWidget(selectionLabel);
    statusLayout->addWidget(selectionText);
    statusLayout->addWidget(lineLabel);
    statusLayout->addWidget(lineText);
    statusLayout->addWidget(columnLabel);
    statusLayout->addWidget(columnText);
    statusLayout->addWidget(jsonBeautifyCheckbox);
    statusLayout->addStretch(1);

    mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(editor);
    mainLayout->addLayout(statusLayout);
    setLayout(mainLayout);
}

void Notepad::appendTextToEditor(QString text)
{
    editor->appendPlainText(text);
}

void Notepad::selectionSizeChanged()
{
    int selectionSize = editor->textCursor().selectionEnd() -  editor->textCursor().selectionStart();
    selectionText->setText(QString::number(selectionSize));
}

void Notepad::columnNoChanged()
{
    int columnLabel = editor->textCursor().columnNumber();
    columnText->setText(QString::number(columnLabel));

    lineText->setText(QString::number(editor->getCurrentLineNumber()));
}

void Notepad::jsonResponeBodyBeautify(bool beautify)
{
    if(editor->toPlainText().size() > 0){
        QByteArray ba;
        ba.append(editor->toPlainText());
        QJsonParseError err;
        QJsonDocument jsondoc = QJsonDocument::fromJson(ba,&err);
        if(err.error != QJsonParseError::NoError){
            return;
        }
        if(!jsondoc.isNull()){
            editor->clear();
            if(beautify){
                editor->clearHighlightKeywords();
                editor->addHighlightRegex(R"((.*)[:]\s)");
                editor->appendPlainText(jsondoc.toJson(QJsonDocument::Indented));
            }else{
                editor->appendPlainText(jsondoc.toJson(QJsonDocument::Compact));
            }
        }
    }
}
