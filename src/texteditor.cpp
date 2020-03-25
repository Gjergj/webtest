#include "texteditor.h"
#include <QDebug>
#include <QTextBlock>
TextEditor::TextEditor(QWidget *parent) : QPlainTextEdit(parent),
    resized(false),
    isTextSelected(false),
    _currentLineNumber(0)
{
    searchWidget = new SearchWidget(this);
    searchWidget->setVisible(false);
    searchWidget->setGeometry(0, this->height() - (searchWidget->height()*1.4),
                              this->width(), searchWidget->height()*1.4);
    connect(searchWidget, &SearchWidget::findText, this, &TextEditor::findNextSearchWidgetSlot);
    connect(searchWidget, &SearchWidget::findPreviousText, this, &TextEditor::findPreviousSearchWidgetSlot);
    connect(searchWidget, &SearchWidget::onSearchTextChanged, this, &TextEditor::highlightSearchText);
    connect(searchWidget, &SearchWidget::replaceText, this, &TextEditor::replaceText);
    connect(searchWidget, &SearchWidget::replaceAllText, this, &TextEditor::replaceAllText);
    connect(this,&TextEditor::copyAvailable,this,[this](bool yes){
        isTextSelected = yes;
    });
    connect(this, &TextEditor::cursorPositionChanged, this, &TextEditor::highlightCurrentLine);

    lineNumberArea = new LineNumberArea(this);
    lineNumberArea->setVisible(false);

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    updateLineNumberAreaWidth(0);
    highlighter = new Highlighter(document());

    //highlightCurrentLine();
}
void TextEditor::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *menu  = editorMenu();
    menu->exec(event->globalPos());
    delete menu;
}

void TextEditor::findNextSlot()
{
    findNext(textCursor().selectedText());
}

void TextEditor::findNextSearchWidgetSlot()
{
    findNext(searchWidget->getFindText());
}

void TextEditor::findPreviousSlot()
{
    findPrevious(textCursor().selectedText());
}
void TextEditor::findPreviousSearchWidgetSlot()
{
    findPrevious(searchWidget->getFindText());
}

void TextEditor::highlightSearchText(QString searchText)
{
    QTextCursor existingCursor = textCursor();
    //QList<QTextEdit::ExtraSelection> extraSelections = this->extraSelections();
    QList<QTextEdit::ExtraSelection> extraSelections;
    this->setExtraSelections(extraSelections);//clear old selections
    QBrush backBrush( Qt::yellow );
    QBrush textBrush( Qt::black );
    QPen outlinePen( Qt::gray, 1 );
    moveCursor(QTextCursor::Start);
    while ( find(searchText) )
    {
        QTextEdit::ExtraSelection extra;
        extra.cursor = textCursor();
        //extra.format.setFontUnderline(true);
        extra.format.setBackground(backBrush);
        extra.format.setForeground(textBrush);
        extra.format.setProperty( QTextFormat::OutlinePen, outlinePen );
        extraSelections.append(extra);
    }
    setExtraSelections(extraSelections);
    setTextCursor(existingCursor);
    find(searchText);
}

void TextEditor::replaceText()
{
    if(findNext(searchWidget->getFindText())){
        textCursor().insertText( searchWidget->getReplaceText() );
    }
}

void TextEditor::replaceAllText()
{
    moveCursor(QTextCursor::Start);
    QString findText = searchWidget->getFindText();
    QString replaceText = searchWidget->getReplaceText();
    while ( find(findText) )
    {
        textCursor().insertText(replaceText);
    }
}

void TextEditor::highlightCurrentLine()
{
    if(!searchWidget->isVisible()){
        QList<QTextEdit::ExtraSelection> extraSelections;
        if (!isReadOnly()) {
            QTextEdit::ExtraSelection selection;

            QColor lineColor = QColor(Qt::yellow).lighter(160);

            selection.format.setBackground(lineColor);
            selection.format.setProperty(QTextFormat::FullWidthSelection, true);
            selection.cursor = textCursor();
            selection.cursor.clearSelection();
            extraSelections.append(selection);
        }
        setExtraSelections(extraSelections);
    }
}

QMenu *TextEditor::editorMenu()
{
    QMenu *menu = createStandardContextMenu();
    if(!textCursor().selectedText().isEmpty()){
        menu->addAction("Find Next",  this, &TextEditor::findNextSlot);
        menu->addAction("Find Previous",  this, &TextEditor::findPreviousSlot);
    }
    return menu;
}

void TextEditor::focusInEvent(QFocusEvent *e)
{
    Q_UNUSED(e);
    searchWidget->clearFocus();
    setFocus();
    removeEventFilter(searchWidget);
}
void TextEditor::focusOutEvent(QFocusEvent *e)
{
    Q_UNUSED(e);
    if(searchWidget->hasFocus()){
        searchWidget->setFocus();
        clearFocus();
        installEventFilter(searchWidget);
    }
}
bool TextEditor::findNext(QString searchText)
{
    return find(searchText);
}

bool TextEditor::findPrevious(QString searchText)
{
    return find(searchText,QTextDocument::FindBackward);
}
void TextEditor::keyPressEvent(QKeyEvent* f_event)
{
    if((f_event->key() == Qt::Key_F) && f_event->modifiers().testFlag(Qt::ControlModifier) /* && f_event->modifiers().testFlag(Qt::ControlModifier) */)
    {
        f_event->accept();
        if(!textCursor().selectedText().isEmpty()){
            searchWidget->setFindText(textCursor().selectedText());
        }
        searchWidget->setVisible(true);
        searchWidget->setFocus();
        installEventFilter(searchWidget);
    }else if(f_event->key() == Qt::Key_Escape){
        searchWidget->setVisible(false);
        searchWidget->clearFindTextEdit();
        removeEventFilter(searchWidget);
        f_event->accept();
    }else if(searchWidget->isVisible() && searchWidget->hasFocus()){
        f_event->accept();
    }else{
        QPlainTextEdit::keyPressEvent(f_event);
    }
}

void TextEditor::resizeEvent(QResizeEvent *event)
{
    QPlainTextEdit::resizeEvent(event);

    if(lineNumberArea->isVisible()){
        QRect cr = contentsRect();
        lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
    }

    if(resized){
        searchWidget->setGeometry(lineNumberAreaWidth(), event->size().height() - (searchWidget->height()),
                                  event->size().width(), searchWidget->height());
    }
    else{
        resized = true;
        searchWidget->setGeometry(lineNumberAreaWidth(), event->size().height() - (searchWidget->height()*1.4),
                                  event->size().width(), searchWidget->height()*1.4);
    }
}

void TextEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    if(lineNumberArea->isVisible()){
        setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
    }
}
void TextEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if(lineNumberArea->isVisible()){
        if (dy)
            lineNumberArea->scroll(0, dy);
        else
            lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

        if (rect.contains(viewport()->rect()))
            updateLineNumberAreaWidth(0);
    }
}

void TextEditor::setHightlightKeywords(QStringList keywordList)
{
    highlighter->setHightlightKeywords(keywordList);
}

void TextEditor::clearHighlightKeywords()
{
    highlighter->clearHighlightKeywords();
}

void TextEditor::addHighlightKeyword(QString keyword)
{
    highlighter->addHighlightKeyword(keyword);
}

void TextEditor::addHighlightRegex(QString regex)
{
    highlighter->addHighlightRegex(regex);
}

void TextEditor::lineNumbersVisible(bool visible)
{
    lineNumberArea->setVisible(visible);
}

size_t TextEditor::getCurrentLineNumber()
{
    return _currentLineNumber;
}
int TextEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;

    return space;
}
void TextEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);

    //![extraAreaPaintEvent_0]

    //![extraAreaPaintEvent_1]
    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();
    //![extraAreaPaintEvent_1]

    //![extraAreaPaintEvent_2]
    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
        _currentLineNumber = blockNumber ;
    }
}
