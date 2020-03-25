#include "mainwindow.h"
#include <QApplication>
#include "configurations.h"
#include "lib/json.hpp"
#include "utils.h"
#include "defines.h"
#include <boost/program_options.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>

namespace po = boost::program_options;
bool debugToFile = false;
QString dataDirectory;
void myDebugLogFileMessageHandler(QtMsgType type, const QMessageLogContext &, const QString & msg)
{
    //#ifdef QT_NO_DEBUG
    //    return;
    //#endif
    QString txt;
    switch (type) {
    case QtDebugMsg:
        txt = QString("Debug: %1").arg(msg);
        break;
    case QtWarningMsg:
        txt = QString("Warning: %1").arg(msg);
        break;
    case QtCriticalMsg:
        txt = QString("Critical: %1").arg(msg);
        break;
    case QtFatalMsg:
        txt = QString("Fatal: %1").arg(msg);
    case QtInfoMsg:
        txt = QString("Info: %1").arg(msg);
        break;
    }
    QFile outFile(dataDirectory + "log.txt");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << txt << endl;
}
void myDebugMessageHandler(QtMsgType type, const QMessageLogContext &, const QString & msg)
{
#ifdef QT_NO_DEBUG
    return;
#endif
    switch (type) {
    case QtDebugMsg:
        qDebug() << msg;
        break;
    case QtWarningMsg:
        qWarning() << msg;
        break;
    case QtCriticalMsg:
        qCritical() << msg;
        break;
    case QtFatalMsg:
        qFatal(msg.toStdString().c_str());
    case QtInfoMsg:
        qInfo() << msg;
        break;
    }
}
void
show_help(const po::options_description& desc, const std::string& topic = " ")
{
    std::cout << desc << '\n';
    if (topic != "") {
        std::cout << "You asked for help on: " << topic << '\n';
    }
    exit( EXIT_SUCCESS );
}

void
process_program_options(const int argc, const char *const argv[])
{
    po::options_description desc("Usage");
    desc.add_options()
            (
                "help,h",
                po::value< std::string >()
                ->implicit_value(" ")
                ->notifier(
                    [&desc](const std::string& topic) {
        show_help(desc, topic);
    }
    ),
                "Show help. If given, show help on the specified topic."
                )
            (
                "debug,d",
                po::value< bool >()
                ->implicit_value(true)
                ->default_value(false)
                ->notifier(
                    [&desc](const bool& debugOption) {
        debugToFile = debugOption;
    }
    ),
                "Show help. If given, show help on the specified topic."
                )
            ;

    //    if (argc == 1) {
    //        show_help(desc); // does not return
    //    }

    po::variables_map args;

    try {
        po::store(
                    po::parse_command_line(argc, argv, desc),
                    args
                    );
    }
    catch (po::error const& e) {
        std::cerr << e.what() << '\n';
        exit( EXIT_FAILURE );
    }
    po::notify(args);
    return;
}
int main(int argc, char *argv[], char **env_var_ptr)
{
    QApplication a(argc, argv);
    dataDirectory = QString::fromStdString(Utils::getDataDirectory() + "/WebTest/");

    QDir dataDir(dataDirectory);
    if(!dataDir.exists(dataDirectory))
        dataDir.mkpath(dataDirectory);

    process_program_options(argc, argv);
    if(debugToFile){
        qInstallMessageHandler(myDebugLogFileMessageHandler);
    }
    else{
        qInstallMessageHandler(myDebugMessageHandler);
    }
    a.setStyleSheet(R"(
                    /*QWidget{ background-color: #E3F2FD}*/

                    QLineEdit { font-size:12px;
                    padding:5px 5px 1px 1px;
                    width:30px;
                    border:none;
                    border-bottom:1px solid #2196F3;
                    }

                    QPushButton { background-color: #64B5F6;
                    border: none;
                    border-radius: 0px;
                    height: 20px;
                    line-height: 20px;
                    outline: 0;
                    padding: 0 2rem;
                    text-transform: uppercase;
                    vertical-align: middle;
                    font: bold 10px;
                    min-width: 5em;
                    }
                    QPushButton:pressed {
                    background-color: #2196F3;
                    border-style: inset;
                    }
                    QLabel {
                    color:#999;
                    font-size:10px;
                    font-weight:normal;
                    position:absolute;
                    left:5px;
                    top:10px;
                    }
                    QToolBar {/*
                    background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 lightgray, stop:1 darkgray);*/
                    background-color: #E3F2FD;
                    border: none;
                    border-radius: 2px;
                    height: 36px;
                    line-height: 36px;
                    outline: 0;
                    padding: 0 2rem;
                    text-transform: uppercase;
                    vertical-align: middle;
                    font: bold 14px;
                    min-width: 10em;
                    }/*
                    QToolBar::handle {
                    image: url(handle.png);
                    }*/
                    QToolButton { /* all types of tool button */
                    border: 0px solid #8f8f91;
                    border-radius: 0px;
                    font: bold 12px;
                    /*background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f6f7fa, stop: 1 #dadbde);*/
                    }
                    QToolButton:pressed {
                    background-color: #2196F3;
                    border-style: inset;
                    }
                    QToolButton[popupMode="1"] { /* only for MenuButtonPopup */
                    padding-right: 20px; /* make way for the popup button */
                    }
                    QToolButton:pressed {
                    /*background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #dadbde, stop: 1 #f6f7fa);*/
                    }
                    QMenu {
                    background-color: #64B5F6; /* sets background of the menu */
                    border: 1px solid black;
                    }
                    QMenu::item {
                    /* sets background of menu item. set this to something non-transparent
                    if you want menu color and menu item color to be different */
                    background-color: transparent;
                    }

                    QMenu::item:selected { /* when user selects item using mouse or keyboard */
                    background-color: #2196F3;
                    }
                    QRadioButton::indicator {
                    width: 13px;
                    height: 13px;
                    color: #2196F3
                    }
                    QRadioButton::indicator::checked {

                    }
                    QSplitter::handle:horizontal {
                    background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                    stop:0 #eee, stop:1 #ccc);
                    border: 1px solid #777;
                    width: 5px;
                    margin-top: 2px;
                    margin-bottom: 2px;
                    /*margin-left: 2px;
                    margin-right: 2px;*/
                    border-radius: 4px;
                    }
                    QSplitter::handle:vertical {
                    background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                    stop:0 #eee, stop:1 #ccc);
                    border: 1px solid #777;
                    width: 5px;
                    /*margin-top: 2px;
                    margin-bottom: 2px;*/
                    margin-left: 2px;
                    margin-right: 2px;
                    border-radius: 4px;
                    }

                    )");

    Configurations::get().loadConfigurations();
    MainWindow w;
    w.showMaximized();
    w.forceResize();

    int retCode = a.exec();
    Configurations::get().saveConfigurations();
    return retCode;
}

