#ifndef HTTPSSERVER_H
#define HTTPSSERVER_H

#include <QObject>
#include <beast/http.hpp>
#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/filesystem.hpp>
#include <beast/core/streambuf.hpp>
#include <boost/asio.hpp>
#include <beast/core/placeholders.hpp>
#include <beast/core/streambuf.hpp>
#include <beast/http.hpp>
#include <beast/core/to_string.hpp>
#include <cstdio>
#include <cstdint>
#include <cstdint>
#include <cstdio>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <utility>
#include <thread>
#include <QDebug>
#include <iostream>
#include <boost/thread.hpp>
#include "boost/lexical_cast.hpp"
#include "shared.h"
#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

typedef boost::asio::ssl::stream<boost::asio::ip::tcp::socket> ssl_socket;

using boost::asio::ip::tcp;
class session_ssl;
class HttpServerProject;


class https_sync_server : public QObject
{
    using endpoint_type = boost::asio::ip::tcp::endpoint;
    using address_type = boost::asio::ip::address;
    using socket_type = boost::asio::ip::tcp::socket;
    using error_code = boost::system::error_code;
    using req_type = beast::http::request<beast::http::string_body>;
    using resp_type = beast::http::response<beast::http::string_body>;

    Q_OBJECT
public:
    explicit https_sync_server(boost::asio::io_service &io_service, QObject *parent = 0);
    ~https_sync_server();
    bool startServer(HttpServerProjectCtx &serverProject);
    void  stopServer();
    void handleStop();
    bool isOpen();
    template<class... Args>
    void log(Args const&... args)
    {
        if(log_)
        {
            std::lock_guard<std::mutex> lock(m_);
            log_args(args...);
        }
    }
signals:
    void requestReceived(QString requestMessage);
    void error(QString errorMessage);
public slots:

private:
    void
    log_args()
    {
    }

    template<class Arg, class... Args>
    void
    log_args(Arg const& arg, Args const&... args)
    {
        std::cerr << arg;
        log_args(args...);
    }

    void
    fail(error_code ec, std::string what)
    {
        log(what, ": ", ec.message(), "\n");
        emit error(QString::fromStdString(what + ": " + ec.message() + "\n") );
    }

    void
    fail(int id, error_code const& ec)
    {
        if(ec != boost::asio::error::operation_aborted &&
                ec != boost::asio::error::eof)
            log("#", id, " ", ec.message(), "\n");
    }

    void on_accept(session_ssl *new_session, error_code ec);
    std::string get_password() const;

    bool log_ = true;
    std::mutex m_;
    boost::asio::io_service& ios_;
    //socket_type sock_;
    boost::asio::ip::tcp::acceptor acceptor_;
    boost::asio::ssl::context context_;

    HttpServerProjectCtx currentProject;

};

#endif // HTTPSERVER_H
