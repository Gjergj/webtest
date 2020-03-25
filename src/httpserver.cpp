#include "httpserver.h"
RequestResponseQueue httpServerRequestResponseQueue;
class session
{
    using endpoint_type = boost::asio::ip::tcp::endpoint;
    using address_type = boost::asio::ip::address;
    using socket_type = boost::asio::ip::tcp::socket;
    using error_code = boost::system::error_code;
    using req_type = beast::http::request<beast::http::string_body>;
    using resp_type = beast::http::response<beast::http::string_body>;
public:
  session(boost::asio::io_service& io_service)
    : sock_(io_service)
  {
  }

  tcp::socket& socket()
  {
    return sock_;
  }

  void start(HttpServerProjectCtx &serverProject)
  {
      ServerRequestResponse serverRequestResponse;

      beast::streambuf sb;
      error_code ec;

      req_type req;
      beast::http::read(sock_, sb, req, ec);
      if(ec){
          goto exit;
      }
      serverRequestResponse.reqData.body = boost::lexical_cast<std::string>(req.body);
      serverRequestResponse.reqData.version = req.version;
      serverRequestResponse.reqData.url = req.url;
      serverRequestResponse.reqData.requestType = req.method;
      for(auto header : req.fields)
         serverRequestResponse.reqData.headers.push_back(std::make_pair(std::string(header.name()), std::string(header.value())));

      serverRequestResponse.rawRequest = QString::fromStdString(boost::lexical_cast<std::string>(req));

      try
      {
          bool routeExists = false;
          resp_type res;
          for(auto route : serverProject.routes){
              std::string url = "";
              std::size_t found = req.url.find("?");
              if (found!=std::string::npos){
                    url.append(req.url,0,found);
              }else{
                  url = req.url;
              }

              if( (route.path == url) && (route.requestType == req.method )){
                  for(auto header : route.headers){
                      res.fields.insert(header.first, header.second);
                  }
                  res.status = route.responseStatusCode;
                  res.reason = route.responseStatusDescription;
                  res.version = req.version;
                  res.body = route.body;
                  routeExists = true;
              }
          }
          if(!routeExists){
              res.status = 404;
              res.reason = "Not found";
              res.version = req.version;
              //prepare(res);
          }
          serverRequestResponse.rawResponse = QString::fromStdString(boost::lexical_cast<std::string>(res));
          write(sock_, res, ec);
          if(ec){
              serverRequestResponse.errorData += QString::fromStdString(ec.message());
              goto exitError;
          }
      }
      catch(std::exception const& e)
      {
          beast::http::response<beast::http::string_body> res;
          res.status = 500;
          res.reason = "Internal Error";
          res.version = req.version;
          res.fields.insert("Server", "http_sync_server");
          res.fields.insert("Content-Type", "text/html");
          res.body =
                  std::string{"An internal error occurred: "} + e.what();
          prepare(res);
          serverRequestResponse.rawResponse = QString::fromStdString(boost::lexical_cast<std::string>(res));
          write(sock_, res, ec);
          if(ec){
              serverRequestResponse.errorData += QString::fromStdString(ec.message());
              goto exitError;
          }
      }
exitError: httpServerRequestResponseQueue.Push(serverRequestResponse);
exit: sock_.shutdown(boost::asio::ip::tcp::socket::shutdown_send,ec);
      sock_.close();
      delete this;
  }
private:
  tcp::socket sock_;
};


http_sync_server::http_sync_server(boost::asio::io_service &io_service, QObject *parent)
    : QObject(parent)
    ,ios_(io_service)
    ,acceptor_(io_service)
{
}
http_sync_server::~http_sync_server()
{
}

bool http_sync_server::startServer(HttpServerProjectCtx &serverProject)
{
    try{
        currentProject = serverProject;
        endpoint_type ep(address_type::from_string(serverProject.IPListen),serverProject.httpPort );
        boost::system::error_code ec;
        acceptor_.open(ep.protocol(),ec);
        if(ec){
            emit error(QString::fromStdString(ec.message()));
            return false;
        }
        acceptor_.bind(ep,ec);
        if(ec){
            emit error(QString::fromStdString(ec.message()));
            return false;
        }
        if(!ec){
            acceptor_.listen( boost::asio::socket_base::max_connections,ec);
            if(ec){
                emit error(QString::fromStdString(ec.message()));
                return false;
            }
            session* new_session = new session(ios_);
            acceptor_.async_accept(new_session->socket(),
                                   boost::bind(&http_sync_server::on_accept, this, new_session,
                                               boost::asio::placeholders::error));

        }else{
            emit error(QString::fromStdString(ec.message()));
            return false;
        }
    }catch (std::exception ex){
        emit error(QString::fromStdString(ex.what()));
        if(acceptor_.is_open())
            acceptor_.close();
        return false;
    }
    return true;
}

void http_sync_server::on_accept(session* new_session, error_code ec)
{
    if (!ec)
    {
        new_session->start(currentProject);
        new_session = new session(ios_);
        acceptor_.async_accept(new_session->socket(),
                               boost::bind(&http_sync_server::on_accept, this, new_session,
                                           boost::asio::placeholders::error));
    }
    else
    {
        delete new_session;
    }
}

bool http_sync_server::isOpen()
{
    return acceptor_.is_open();
}
void http_sync_server::stopServer()
{
    error_code ec;
    try{
        if(acceptor_.is_open())
            acceptor_.close(ec);
        if(ec){
            emit error(QString::fromStdString(ec.message()));
        }
    }
    catch (std::exception ex){
        qDebug() << QString::fromStdString(ex.what());
        emit error(QString::fromStdString(ex.what()));
    }
    return;
}

