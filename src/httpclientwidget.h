#ifndef HTTPCLIENTWIDGET_H
#define HTTPCLIENTWIDGET_H
#include <QTabWidget>
#include "httpclientwidgetv2.h"

class HttpClientWidget : public QWidget
{
public:
    HttpClientWidget(Notepad *notepadEditor, QWidget *parent = 0);
private slots:
    void closeTab(int tabIndex);
    void addWidget();
private:
    QTabWidget *tabs;
    Notepad *_notepadEditor;
    QVBoxLayout *mainLayout;
    QToolButton *tb;
};

#endif // HTTPCLIENTWIDGET_H
