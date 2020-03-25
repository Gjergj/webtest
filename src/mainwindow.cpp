#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      serifFont("Times", 12, QFont::Bold)
{

    notepad = new Notepad(this);
    httpClientWindowv2 = new HttpClientWidget(notepad, this);
    httpSniffWindow = new HttpSniffWindow(this);
    httpServerWidget = new HttpServerWidget(this);
    utilWindow = new UtilWindow(this);

    configurationWidget = new ConfigurationWidget(this);
    helpWindow = new HelpWindow(this);

    stackedWidget = new QStackedWidget;
    stackedWidget->addWidget(httpClientWindowv2);
    stackedWidget->addWidget(httpSniffWindow);
    stackedWidget->addWidget(httpServerWidget);
    stackedWidget->addWidget(utilWindow);
    stackedWidget->addWidget(notepad);
    stackedWidget->addWidget(configurationWidget);
    stackedWidget->addWidget(helpWindow);

    toolBar = new ToolBar(this);
    toolBar->setAllowedAreas(Qt::TopToolBarArea);
    toolBar->setFloatable(false);
    toolBar->setContextMenuPolicy(Qt::PreventContextMenu);
    toolBar->setMovable(false);
    connect(toolBar, &ToolBar::toolBarPressed, this, &MainWindow::toolBarPressed);
    connect(toolBar, &ToolBar::toolBarMoved, this, &MainWindow::toolBarMoved);
    connect(toolBar, &ToolBar::toolBarDoubleClicked,this,&MainWindow::toggleMaximize);

    httpClient2Action = new QToolButton(this);
    httpClient2Action->setText("HTTP/S CLIENT v2");
    httpClient2Action->setStatusTip(tr("Http and Https client v2"));
    httpClient2Action->setToolTip(tr("Http and Https client v2"));
    httpClient2Action->setObjectName("httpclient2action");
    httpClient2Action->installEventFilter(toolBar);
    connect(httpClient2Action, &QToolButton::clicked, this, &MainWindow::openHttpClient2);
    toolBar->addWidget(httpClient2Action);

    httpSniffAction  = new QToolButton(this);
    httpSniffAction->setText("HTTP SNIFF");
    httpSniffAction->setStatusTip(tr("Http sniffer"));
    httpSniffAction->setToolTip(tr("Http sniffer"));
    httpSniffAction->setObjectName("httpsniffaction");
    httpSniffAction->installEventFilter(toolBar);
    //httpSniffAction->setFont(serifFont);
    connect(httpSniffAction, &QToolButton::clicked, this, &MainWindow::openHttpSniff);
    toolBar->addWidget(httpSniffAction);

    utilAction = new QToolButton(this);
    utilAction->setText("UTILS");
    utilAction->setStatusTip(tr("Utilities"));
    utilAction->setToolTip(tr("Utilities"));
    utilAction->setObjectName("utilaction");
    utilAction->installEventFilter(toolBar);
    connect(utilAction, &QToolButton::clicked, this, &MainWindow::openUtilities);
    toolBar->addWidget(utilAction);

    notepadAction = new QToolButton(this);;
    notepadAction->setText("NOTEPAD#");
    notepadAction->setStatusTip(tr("Notepad"));
    notepadAction->setToolTip(tr("Notepad"));
    notepadAction->setObjectName("notepadaction");
    notepadAction->installEventFilter(toolBar);
    connect(notepadAction, &QToolButton::clicked, this, &MainWindow::openNotepad);
    toolBar->addWidget(notepadAction);

    helpAction = new QToolButton(this);
    helpAction->setText("?");
    helpAction->setStatusTip(tr("Help"));
    helpAction->setToolTip(tr("Help"));
    helpAction->setObjectName("helpaction");
    helpAction->installEventFilter(toolBar);
    //helpAction->setFont(serifFont);
    connect(helpAction, &QToolButton::clicked, this, &MainWindow::openHelp);
    toolBar->addWidget(helpAction);


    QWidget* spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    toolBar->addWidget(spacer);

    configurationAction = new QToolButton(this);
    configurationAction->setIcon(QIcon(":/img/configuration-icon.png"));
    //configurationAction->setText("Configurations");
    configurationAction->setStatusTip(tr("Configurations"));
    configurationAction->setToolTip(tr("Configurations"));
    configurationAction->setObjectName("configurationsaction");
    configurationAction->installEventFilter(toolBar);
    connect(configurationAction, &QToolButton::clicked, this, &MainWindow::openConfigurations);
    toolBar->addWidget(configurationAction);

    minimizeApplicationAction = new QToolButton(this);
    //maximizeApplicationAction->setIcon(QIcon(":/img/maximize.png"));
    minimizeApplicationAction->setText("-");
    minimizeApplicationAction->setObjectName("minimizeapplicationaction");
    minimizeApplicationAction->installEventFilter(toolBar);
    connect(minimizeApplicationAction, &QToolButton::clicked, [=](bool trig) {
        Q_UNUSED(trig);
        this->setWindowState(Qt::WindowMinimized);
    });
    toolBar->addWidget(minimizeApplicationAction);

    maximizeApplicationAction = new QToolButton(this);
    //maximizeApplicationAction->setIcon(QIcon(":/img/maximize.png"));
    maximizeApplicationAction->setText("+");
    maximizeApplicationAction->setObjectName("maximizeapplicationaction");
    maximizeApplicationAction->installEventFilter(toolBar);
    connect(maximizeApplicationAction, &QToolButton::clicked, this,&MainWindow::toggleMaximize);
    toolBar->addWidget(maximizeApplicationAction);

    closeApplicationAction = new QToolButton(this);
    //closeApplicationAction->setIcon(QIcon(":/img/close.png"));
    closeApplicationAction->setText("X");
    closeApplicationAction->setObjectName("closeapplicationaction");
    closeApplicationAction->installEventFilter(toolBar);
    connect(closeApplicationAction, &QToolButton::clicked, [=](bool trig) {
        Q_UNUSED(trig);
        QApplication::quit();
    });
    toolBar->addWidget(closeApplicationAction);

    addToolBar(Qt::TopToolBarArea,toolBar);
    setCentralWidget(stackedWidget);
    this->setWindowFlags(Qt::FramelessWindowHint);
}

MainWindow::~MainWindow()
{

}
void MainWindow::forceResize()
{
    //show briefly to make them resize
    stackedWidget->setCurrentIndex(5);
    stackedWidget->setCurrentIndex(4);
    stackedWidget->setCurrentIndex(3);
    //stackedWidget->setCurrentIndex(2);
    stackedWidget->setCurrentIndex(1);
    stackedWidget->setCurrentIndex(0);
}

void MainWindow::openHttpClient2()
{
    stackedWidget->setCurrentIndex(0);
}

void MainWindow::openHttpSniff()
{
    stackedWidget->setCurrentIndex(1);
}

void MainWindow::openHttpServer()
{
    stackedWidget->setCurrentIndex(2);
}

void MainWindow::openUtilities()
{
    stackedWidget->setCurrentIndex(3);
}

void MainWindow::openNotepad()
{
    stackedWidget->setCurrentIndex(4);
}

void MainWindow::openConfigurations()
{
    stackedWidget->setCurrentIndex(5);
}

void MainWindow::openHelp()
{
    stackedWidget->setCurrentIndex(6);
}

void MainWindow::toolBarPressed(QMouseEvent *event)
{
    mpos = event->pos();
}

void MainWindow::toolBarMoved(QMouseEvent *event)
{
    if(this->windowState() == Qt::WindowMaximized){
        //this->setWindowState(Qt::WindowNoState);
        toggleMaximize();
        return;
    }
    QPoint diff = event->pos() - mpos;
    QPoint newpos = this->pos() + diff;
    this->move(newpos);
}

void MainWindow::toggleMaximize()
{
    this->setWindowState( (this->windowState() && Qt::WindowMaximized)
                          ? Qt::WindowNoState :  Qt::WindowMaximized );
}

