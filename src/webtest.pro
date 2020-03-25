#-------------------------------------------------
#
# Project created by QtCreator 2016-09-25T07:27:59
#
#-------------------------------------------------

QT       += core gui network qml quick quickwidgets widgets svg quickcontrols2 webenginewidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = webtest
TEMPLATE = app
CONFIG += c++14

SOURCES += main.cpp \
        mainwindow.cpp \
    utilwindow.cpp \
    requestheaderswidget.cpp \
    responsewidget.cpp \
    httpsniffer.cpp \
    httpserverwidget.cpp \
    httpserverrequestwidget.cpp \
    httpserverresponsewidget.cpp \
    clickablelabel.cpp \
    helpwidget.cpp \
    httpsniffwidget.cpp \
    httpserver.cpp \
    httpclientwidgetv2.cpp \
    requestresponsequeue.cpp \
    httpsserver.cpp \
    toolbar.cpp \
    utils.cpp \
    httpclient.cpp \
    configurationwidget.cpp \
    configurations.cpp \
    texteditor.cpp \
    notepad.cpp \
    searchwidget.cpp \
    highlighter.cpp \
    requestheadermodel.cpp \
    headerlistmodel.cpp \
    documenthandler.cpp \
    urlencodedbodymodel.cpp \
    querystringmodel.cpp \
    httpclientwidget.cpp \
    treemodel.cpp \
    treeitem.cpp \
    sortfilterproxymodel.cpp

HEADERS  += mainwindow.h \
    utilwindow.h \
    requestheaderswidget.h \
    responsewidget.h \
    sniffeddata.h \
    httpsniffer.h \
    tcp.h \
    httpserverwidget.h \
    httpserverrequestwidget.h \
    httpserverresponsewidget.h \
    interfaceinfo.h \
    clickablelabel.h \
    helpwidget.h \
    httpsniffwidget.h \
    httpserver.h \
    httpclientwidgetv2.h \
    requestresponsequeue.h \
    shared.h \
    mtypes.h \
    httpsserver.h \
    toolbar.h \
    utils.h \
    httpclient.h \
    configurationwidget.h \
    configurations.h \
    texteditor.h \
    notepad.h \
    searchwidget.h \
    lib/json.hpp \
    highlighter.h \
    requestheadermodel.h \
    headerlistmodel.h \
    documenthandler.h \
    urlencodedbodymodel.h \
    defines.h \
    querystringmodel.h \
    httpclientwidget.h \
    treemodel.h \
    treeitem.h \
    sortfilterproxymodel.h


win32 {
    message("* Using settings for Windows.")
    DEFINES += "WIN32_LEAN_AND_MEAN"

    INCLUDEPATH += $$PWD/lib/WpdPack/Include

    INCLUDEPATH += $$PWD/lib/beast/include
    DEPENDPATH += $$PWD/lib/beast/include

    INCLUDEPATH += D:/libs/boost_1_63_0_X64_VS2015
    DEPENDPATH += D:/libs/boost_1_63_0_X64_VS2015

    INCLUDEPATH +=D:/libs/OpenSSL-Win64_1.0.2k/include
    DEPENDPATH += D:/libs/OpenSSL-Win64_1.0.2k/include

    LIBS += -L$$PWD/lib/WpdPack/Lib/x64 -lPacket -lwpcap -lWs2_32 -lws2_32 -lOle32

    INCLUDEPATH += $$PWD/lib/zlib-1.2.8
    DEPENDPATH += $$PWD/lib/zlib-1.2.8
    DEPENDPATH += $$PWD/lib/zlib_64
    INCLUDEPATH += $$PWD/lib/zlib_64

    INCLUDEPATH += "C:/Program Files (x86)/Windows Kits/10/Include/10.0.10150.0/ucrt"
    LIBS += -L"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.10150.0/ucrt/x64"

    CONFIG(debug, debug|release) {
        message("*Debug build*")

        LIBS += -LD:/libs/OpenSSL-Win64_1.0.2k/lib/VC/ -lssleay32MTd
        LIBS += -LD:/libs/OpenSSL-Win64_1.0.2k/lib/VC/ -llibeay32Md
        LIBS += -LD:/libs/boost_1_63_0_X64_VS2015/lib64-msvc-14.0 -llibboost_system-vc140-mt-gd-1_63 -llibboost_program_options-vc140-mt-gd-1_63
        LIBS += -L$$PWD/lib/zlib_64/debug/ -lzlibstaticd
    }
    CONFIG(release, debug|release) {
        message("*Release build*")
        DEFINES += "_WIN32_WINNT_WIN7"
        DEFINES += "NTDDI_WIN7"
        DEFINES += "_WIN32_WINNT_WIN7=0x0601"
        LIBS += -LD:/libs/OpenSSL-Win64_1.0.2k/lib/VC/ -lssleay32MT
        LIBS += -LD:/libs/OpenSSL-Win64_1.0.2k/lib/VC/ -llibeay32MT

        LIBS += -LD:/libs/boost_1_63_0_X64_VS2015/lib64-msvc-14.0/ -llibboost_system-vc140-mt-1_63 -llibboost_program_options-vc140-mt-1_63

        LIBS += -L$$PWD/lib/zlib_64/debug/ -lzlibstaticd
    }
}

unix:!macx{
    message("* Using settings for Unix/Linux.")

    INCLUDEPATH += $$PWD/lib/beast/include
    DEPENDPATH += $$PWD/lib/beast/include

    INCLUDEPATH += $$/home/gjergji/Downloads/boost_1_63_0/boost_lib/boost_bin/include
    DEPENDPATH += $$/home/gjergji/Downloads/boost_1_63_0/boost_lib/boost_bin/include

    unix:!macx: LIBS += -L/home/gjergji/Downloads/boost_1_63_0/boost_lib/boost_bin/lib -lboost_system -lboost_program_options
    #LIBS += -lssl -lcrypto
    INCLUDEPATH += $$PWD/../../Downloads/boost_1_63_0/bin.v2/libs/system/build/gcc-5.3.1/release/link-static/threading-multi
    DEPENDPATH += $$PWD/../../Downloads/boost_1_63_0/bin.v2/libs/system/build/gcc-5.3.1/release/link-static/threading-multi

    #unix:!macx: PRE_TARGETDEPS += $$PWD/../../Downloads/boost_1_61_0/bin.v2/libs/system/build/gcc-5.3.1/release/link-static/threading-multi/libboost_system.a

    INCLUDEPATH += $$PWD/lib/zlib_linux/include
    DEPENDPATH += $$PWD/lib/zlib_linux/include
    unix:!macx: PRE_TARGETDEPS += $$PWD/lib/zlib_linux/lib/libz.a
    LIBS += -L$$PWD/lib/zlib_linux/lib/ -lz

    INCLUDEPATH += $$PWD/lib/openssl_bin/include
    DEPENDPATH += $$PWD/lib/openssl_bin/include
    unix:!macx: PRE_TARGETDEPS += $$PWD/lib/openssl_bin/lib/libcrypto.a
    unix:!macx: PRE_TARGETDEPS += $$PWD/lib/openssl_bin/lib/libssl.a
    LIBS += -L$$PWD/lib/openssl_bin/lib -lcrypto -lssl

#    INCLUDEPATH += $$PWD/lib/libtins-3.5_linux/include
#    DEPENDPATH += $$PWD/lib/libtins-3.5_linux/include
#    LIBS += -L$$PWD/lib/libtins-3.5_linux/lib -ltins

    unix:!macx: LIBS += -L$$PWD/lib/uri_linux/bin/lib/ -lnetwork-uri
    INCLUDEPATH += $$PWD/lib/uri_linux/bin/include/include
    DEPENDPATH += $$PWD/lib/uri_linux/bin/include/include
    unix:!macx: PRE_TARGETDEPS += $$PWD/lib/uri_linux/bin/lib/libnetwork-uri.a


    LIBS        += -lpcap -lm -pthread -ldl# -lWs2_32
}

#ifdef _WIN64
#ifdef _WIN32
#ifdef __linux

DISTFILES += \
    todo

RESOURCES += \
    res.qrc

