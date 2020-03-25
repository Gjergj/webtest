#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QWidget>
#include <QStackedWidget>
#include <QToolBar>
#include <QAction>
#include <QDebug>
#include <QApplication>
#include <QMouseEvent>
#include <QAction>
#include <QToolButton>
#include "utilwindow.h"
#include "helpwidget.h"
#include "httpsniffwidget.h"
#include "httpserverwidget.h"
#include "httpclientwidgetv2.h"
#include "configurationwidget.h"
#include "httpclientwidget.h"
#include "toolbar.h"
#include "notepad.h"
class MainWindow : public QMainWindow
{
    Q_OBJECT

private slots:
    void openHttpClient2();
    void openHttpSniff();
    void openHttpServer();
    void openUtilities();
    void openNotepad();
    void openConfigurations();
    void openHelp();

    void toolBarPressed(QMouseEvent *event);
    void toolBarMoved(QMouseEvent *event);
    void toggleMaximize();
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void forceResize();
    //QTabWidget *mainWidget;
    QStackedWidget *stackedWidget;
    UtilWindow *utilWindow;
    HelpWindow *helpWindow;
    HttpSniffWindow *httpSniffWindow;
    HttpServerWidget *httpServerWidget;
    HttpClientWidget *httpClientWindowv2;
    ConfigurationWidget *configurationWidget;
    Notepad *notepad;
    ToolBar *toolBar;
    QToolButton *closeApplicationAction;
    QToolButton *maximizeApplicationAction;
    QToolButton *minimizeApplicationAction;

    QToolButton *httpClientAction;
    QToolButton *httpClient2Action;
    QToolButton *httpSniffAction;
    QToolButton *httpServerAction;
    QToolButton *utilAction;
    QToolButton *notepadAction;
    QToolButton *configurationAction;
    QToolButton *helpAction;

    QPoint mpos;
    QFont serifFont;
};

#endif // MAINWINDOW_H
