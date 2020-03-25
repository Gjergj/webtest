#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <beast/core/to_string.hpp>
#include <beast/http.hpp>
#include <beast/http/parse.hpp>
#include <beast/http/parser_v1.hpp>
#include <beast/websocket.hpp>
#include <beast/http/message.hpp>
#include <beast/http.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <beast/core/to_string.hpp>
#include <beast/http/string_body.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <zlib.h>

#include <chrono>
#ifdef __linux
#include <network/uri.hpp>
#endif
#include "utils.h"
class HttpClient
{
public:
    HttpClient();
    HttpClient(std::string scheme,std::string host,int port, std::string path, std::string queryString );
    HttpClient(std::string scheme,std::string host,int port, std::string path, std::string queryString , std::string proxyHost, int proxyPort);
#ifdef __linux
    HttpClient(std::string url);
#endif
    void setHost(std::string host);
    std::string getHost();
    void setPath(std::string path);
    std::string getPath();
    void setPort(int port);
    int getPort();
    void setScheme(std::string scheme);
    std::string getScheme();
    void setHttpVersion(int version);
    int getHttpVersion();
    void setMethod(std::string method);
    std::string getMethod();
    void setQuery(std::string queryString);
    std::string getQuery();
    void setRequestBody(std::string body);
    std::string getRequestBody();
    void insertHeader(std::string header, std::string value);
    void replaceHeader(std::string header, std::string value);
    void clearHeaders();
    void eraseHeader(std::string header);
    std::string getHeader(const std::string header);
    bool existsHeader(std::string header);
    std::string getRawRequest();
    void setProxyHost(std::string proxyHost);
    std::string getProxyHost();
    void setProxyPort(int proxyPort);
    int getProxyPort();


    std::map<std::string,std::string> getResponseHeaders();
    size_t getResponseBodySize();
    int getResponseStatus();
    std::string getResponseReason();
    std::string getRawResponse();
    std::string getResponseBody();

    bool request(std::vector<std::string> &stats, std::string &error);
private:
    int gzipDecompress(unsigned char *compressData , size_t length, std::string &body);
    void prepareHttpRequest();
    bool prepareProxy();
    bool proxySend(boost::asio::ip::tcp::socket &sock, std::string &error);
    beast::http::request<beast::http::string_body> req;
    beast::http::response<beast::http::string_body> resp;
    std::string host_;
    std::string path_;
    std::string queryString_;
    std::string scheme_;
    std::string proxyHost_;
    int proxyPort_;
    int port_;

    template <typename InputIterator, typename OutputIterator>
    OutputIterator encode_space(InputIterator first, InputIterator last,
                               OutputIterator out) {
      auto it = first;
      while (it != last) {
        network::detail::encode_char(*it, out, " ");
        ++it;
      }
      return out;
    }
    template <class String>
    String encode_space(const String &host) {
      String encoded;
      encode_space(std::begin(host), std::end(host), std::back_inserter(encoded));
      return encoded;
    }
};

#endif // HTTPCLIENT_H
