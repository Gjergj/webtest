#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QObject>
#include <QPlainTextEdit>
#include <QWidget>
#include <QMenu>
#include <QContextMenuEvent>
#include "searchwidget.h"
#include "highlighter.h"
class QPaintEvent;
class QResizeEvent;
class QSize;
class TextEditor : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit TextEditor(QWidget *parent = 0);
    QMenu *editorMenu();
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();
    void setHightlightKeywords(QStringList keywordList);
    void clearHighlightKeywords();
    void addHighlightKeyword(QString keyword);
    void addHighlightRegex(QString regex);
    void lineNumbersVisible(bool visible);
    size_t getCurrentLineNumber();
private slots:
    void findNextSlot();
    void findNextSearchWidgetSlot();
    void findPreviousSlot();
    void findPreviousSearchWidgetSlot();
    void highlightSearchText(QString searchText);
    void replaceText();
    void replaceAllText();
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &, int);

protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
private:
    void focusInEvent(QFocusEvent *e);
    void focusOutEvent(QFocusEvent *e);
    void keyPressEvent(QKeyEvent* f_event);
    void contextMenuEvent(QContextMenuEvent *event);
    bool findNext(QString searchText);
    bool findPrevious(QString searchText);
    bool resized;
    SearchWidget *searchWidget;
    bool isTextSelected;
    QWidget *lineNumberArea;
    size_t _currentLineNumber;
    Highlighter *highlighter;
};

class LineNumberArea : public QWidget
{
public:
    LineNumberArea(TextEditor *editor) : QWidget(editor) {
        codeEditor = editor;
    }

    QSize sizeHint() const Q_DECL_OVERRIDE {
        return QSize(codeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE {
        codeEditor->lineNumberAreaPaintEvent(event);
    }

private:
    TextEditor *codeEditor;
};

#endif // TEXTEDITOR_H
