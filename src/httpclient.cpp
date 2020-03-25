#include "httpclient.h"
#include <openssl/ssl.h>
#include <openssl/err.h>
HttpClient::HttpClient() : queryString_(""),
    scheme_(""),
    proxyHost_(""),
    proxyPort_(0),
    host_(""),
    port_(0),
    path_("")
{

}
HttpClient::HttpClient(std::string scheme,std::string host,int port, std::string path, std::string queryString ):
    scheme_(scheme),
    host_(host),
    port_(port),
    path_(path),
    queryString_(queryString),
    proxyHost_(""),
    proxyPort_(0)
{

}

HttpClient::HttpClient(std::string scheme,std::string host,int port, std::string path, std::string queryString , std::string proxyHost, int proxyPort):
    scheme_(scheme),
    host_(host),
    port_(port),
    path_(path),
    queryString_(queryString),
    proxyHost_(proxyHost),
    proxyPort_(proxyPort)
{

}
#ifdef __linux
HttpClient::HttpClient(std::string url)
{
    std::string encodedInstance;// = encode_space(url);
    encodedInstance = boost::algorithm::replace_all_copy(url, " ", "%20");
//    network::uri::encode_path(std::begin(url), std::end(url),
//                    std::back_inserter(encodedInstance));
//    network::detail::encode_char(std::begin(url), std::end(url),
//                                 std::back_inserter(encodedInstance));
    network::uri uri(encodedInstance);

    if(uri.has_scheme())
        scheme_ = uri.scheme().to_string();
    host_ = uri.host().to_string();

    path_ = uri.path().to_string();
    if(uri.has_port()){
        port_ = uri.port<int>();
    }
    else{
        if(scheme_ == "https"){
            port_ = 443;
        }else if (scheme_ == "http"){
            port_ = 80;
        }else{
            throw "Uknown scheme";
        }
    }
    queryString_ = uri.query().to_string();
}
#endif
void HttpClient::setHost(std::string host)
{
    host_ = host;
}

std::string HttpClient::getHost()
{
    return host_;
}

void HttpClient::setPath(std::string path)
{
    path_ = path;
}

std::string HttpClient::getPath()
{
    return path_;
}

void HttpClient::setPort(int port)
{
    port_ = port;
}

int HttpClient::getPort()
{
    return port_;
}

void HttpClient::setScheme(std::string scheme)
{
    scheme_ = scheme;
}

std::string HttpClient::getScheme()
{
    return scheme_;
}

void HttpClient::setHttpVersion(int version)
{
    req.version = version;
}

int HttpClient::getHttpVersion()
{
    return req.version;
}

void HttpClient::setMethod(std::string method)
{
    req.method = method;
}

std::string HttpClient::getMethod()
{
    return req.method;
}

void HttpClient::setQuery(std::string queryString)
{
    queryString_ = queryString;
}

std::string HttpClient::getQuery()
{
    return queryString_;
}

void HttpClient::setRequestBody(std::string body)
{
    req.body = body;
}

std::string HttpClient::getRequestBody()
{
    return req.body;
}

void HttpClient::insertHeader(std::string header, std::string value)
{
    req.fields.insert(header,value);
}

void HttpClient::replaceHeader(std::string header, std::string value)
{
    req.fields.replace(header,value);
}

void HttpClient::clearHeaders()
{
    req.fields.clear();
}

void HttpClient::eraseHeader(std::string header)
{
    req.fields.erase(header);
}

std::string HttpClient::getHeader(const std::string header)
{
    return std::string(req.fields[header]);
}

bool HttpClient::existsHeader(std::string header)
{
    return req.fields.exists(header);
}

std::string HttpClient::getRawRequest()
{
    return boost::lexical_cast<std::string>(req);
}

void HttpClient::setProxyHost(std::string proxyHost)
{
    proxyHost_ = proxyHost;
}

std::string HttpClient::getProxyHost()
{
    return proxyHost_;
}

void HttpClient::setProxyPort(int proxyPort)
{
    proxyPort_ = proxyPort;
}

int HttpClient::getProxyPort()
{
    return proxyPort_;
}

std::map<std::string, std::string> HttpClient::getResponseHeaders()
{
    std::map<std::string, std::string>  headers;
    for(auto header : resp.fields){
        headers[std::string(header.name())] = std::string(header.value());
    }
    return headers;
}

size_t HttpClient::getResponseBodySize()
{
    return resp.body.size();
}

int HttpClient::getResponseStatus()
{
    return resp.status;
}

std::string HttpClient::getResponseReason()
{
    return resp.reason;
}

std::string HttpClient::getRawResponse()
{
    return boost::lexical_cast<std::string>(resp);
}

std::string HttpClient::getResponseBody()
{
    if(std::string(resp.fields["Content-Encoding"]) == std::string("gzip")){
        std::string uncompressedBody;
        uncompressedBody.reserve(500);
        gzipDecompress((unsigned char*) resp.body.data(),resp.body.size(), uncompressedBody);
        uncompressedBody.shrink_to_fit();
        return uncompressedBody;
    }else{
        return resp.body;
        //boost::lexical_cast<std::string>(resp.body);
    }
}

void hex_encode(unsigned char* readbuf, void *writebuf, size_t len)
{
    for(size_t i=0; i < len; i++) {
        char *l = (char*) (2*i + ((intptr_t) writebuf));
        sprintf(l, "%02x", readbuf[i]);
    }
}
#define DATE_LEN 128

int convert_ASN1TIME(ASN1_TIME *t, char* buf, size_t len)
{
    int rc;
    BIO *b = BIO_new(BIO_s_mem());
    rc = ASN1_TIME_print(b, t);
    if (rc <= 0) {
        //log_error("fetchdaemon", "ASN1_TIME_print failed or wrote no data.\n");
        BIO_free(b);
        return -1;
    }
    rc = BIO_gets(b, buf, len);
    if (rc <= 0) {
        //log_error("fetchdaemon", "BIO_gets call failed to transfer contents to buf");
        BIO_free(b);
        return -1;
    }
    BIO_free(b);
    return EXIT_SUCCESS;
}

/*---------------------------------------------------------------------*/
/*--- ShowCerts - print out the certificates.                       ---*/
/*---https://zakird.com/2013/10/13/certificate-parsing-with-openssl ---*/
/*---------------------------------------------------------------------*/
void ShowCerts(SSL* ssl, std::vector<std::string> &certinfo)
{   X509 *cert;

    char line[1024];
    cert = SSL_get_peer_certificate(ssl);	/* get the server's certificate */
    if ( cert != NULL )
    {
        certinfo.push_back("Server Certificate Details");
        certinfo.push_back(" ");

        SSL_SESSION        *session;
        session = SSL_get1_session(ssl);
        if (session != NULL){
            if (session->ssl_version == SSL2_VERSION)
                certinfo.push_back("Protocol Version: " + std::string("SSLv2"));
            else if (session->ssl_version == SSL3_VERSION)
                certinfo.push_back("Protocol Version: " + std::string("SSLv3"));
            else if (session->ssl_version == TLS1_VERSION)
                certinfo.push_back("Protocol Version: " + std::string("TLSv1"));

            else if (session->ssl_version == TLS1_1_VERSION)
                certinfo.push_back("Protocol Version: " + std::string("TLSv1.1"));
            else if (session->ssl_version == TLS1_2_VERSION)
                certinfo.push_back("Protocol Version: " + std::string("TLSv1.2"));

            else if (session->ssl_version == DTLS1_VERSION)
                certinfo.push_back("Protocol Version: " + std::string("DTLSv1"));
            else if (session->ssl_version == DTLS1_BAD_VER)
                certinfo.push_back("Protocol Version: " + std::string("DTLSv1"));
            else
                certinfo.push_back("Protocol Version: " + std::string("unknown"));
        }
        if (session != NULL)
            SSL_SESSION_free(session);

        {
            X509_NAME_oneline(X509_get_subject_name(cert), line, 1024);
            line[1023] = 0;
            certinfo.push_back("Subject Name: " + std::string(line));

            memset(line,0,1024);
            X509_NAME_oneline(X509_get_issuer_name(cert), line, 1024);
            line[1023] = 0;
            certinfo.push_back("Issuer Name: " + std::string(line));

            const SSL_CIPHER * cipher = SSL_get_current_cipher(ssl);
            const char *cipherName = SSL_CIPHER_get_name(cipher);
            certinfo.push_back("Cipher Name: " + std::string(cipherName));
        }
        {
            //Cryptographic (e.g. SHA-1) Fingerprint
#define SHA1LEN 20
            unsigned char buf[SHA1LEN];

            const EVP_MD *digest = EVP_sha1();
            unsigned len;

            int rc = X509_digest(cert, digest, buf, &len);
            if (rc == 0 || len != SHA1LEN) {
                return;
            }
            char strbuf[2*SHA1LEN+1] = {0};
            hex_encode(buf, strbuf, SHA1LEN);
            certinfo.push_back(std::string("SHA1: ") + std::string(strbuf));
        }
        /////////////
        {
            int version = ((int) X509_get_version(cert)) + 1;
            certinfo.push_back(std::string("Cert Version: ") + std::to_string(version));
        }
        {
            ////////
            ///Serial number
            ///
#define SERIAL_NUM_LEN 1000
            //unsigned char buf[SHA1LEN];
            char serial_number[SERIAL_NUM_LEN+1];

            ASN1_INTEGER *serial = X509_get_serialNumber(cert);

            BIGNUM *bn = ASN1_INTEGER_to_BN(serial, NULL);
            if (!bn) {
                //fprintf(stderr, "unable to convert ASN1INTEGER to BN\n");
                return ;
            }

            char *tmp = BN_bn2dec(bn);
            if (!tmp) {
                //fprintf(stderr, "unable to convert BN to decimal string.\n");
                BN_free(bn);
                return ;
            }

            if (strlen(tmp) >= SERIAL_NUM_LEN) {
                //fprintf(stderr, "buffer length shorter than serial number\n");
                BN_free(bn);
                OPENSSL_free(tmp);
                return ;
            }

            strncpy(serial_number, tmp, SERIAL_NUM_LEN);
            BN_free(bn);
            OPENSSL_free(tmp);
            certinfo.push_back("Serial Number: "+ std::string(serial_number));
        }

        {
            //////////
            ///
            /// Signature algorithm
            int pkey_nid = OBJ_obj2nid(cert->cert_info->key->algor->algorithm);

            if (pkey_nid == NID_undef) {
                //fprintf(stderr, "unable to find specified signature algorithm name.\n");
                return ;
            }
#define SIG_ALGO_LEN 64
            char sigalgo_name[SIG_ALGO_LEN+1];
            const char* sslbuf = OBJ_nid2ln(pkey_nid);

            if (strlen(sslbuf) > SIG_ALGO_LEN) {
                //fprintf(stderr, "public key algorithm name longer than allocated buffer.\n");
                return ;
            }

            strncpy(sigalgo_name, sslbuf, SIG_ALGO_LEN);
            certinfo.push_back("Signature Algorithm: " + std::string(sigalgo_name));
        }
        {
            ///////////
            ASN1_TIME *not_before = X509_get_notBefore(cert);
            char not_before_str[DATE_LEN];
            convert_ASN1TIME(not_before, not_before_str, DATE_LEN);
            certinfo.push_back("Certificate Date not before: " + std::string(not_before_str));

            ASN1_TIME *not_after = X509_get_notAfter(cert);
            char not_after_str[DATE_LEN];
            convert_ASN1TIME(not_after, not_after_str, DATE_LEN);
            certinfo.push_back("Certificate Date not after: " + std::string(not_after_str));
        }

        X509_free(cert);					/* free the malloc'ed certificate copy */
    }
    else
        certinfo.push_back("No certificates.");
}

bool HttpClient::request(std::vector<std::string> &stats,std::string &error)
{
    //todo nese perdoruesi vendos http://google.al atehere programi e nderton kerekesen pa path
    //ne kete rast programi duhet te shtoje vete root path "/" ose kjo gje te jete opsionale
    if( (scheme_ == "") || (host_ == "") || (port_ == 0) ){
        error = "scheme , host or port not set correctly.";
        return false;
    }
    prepareHttpRequest();
    boost::system::error_code ec;
    try{
        boost::asio::io_service ios;
        boost::asio::ip::tcp::resolver r{ios};

        stats.push_back("Connection Details");
        stats.push_back(" ");

        boost::asio::ip::tcp::socket sock{ios};
        boost::asio::ip::tcp::resolver::iterator iter;
        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
        if(proxyHost_ != ""){
            std::string servicePort  = std::to_string(proxyPort_);
            boost::asio::ip::tcp::resolver::query tcpQuery{proxyHost_, servicePort.c_str()};
            iter = r.resolve(tcpQuery,ec);
        }else{
            std::string servicePort  = std::to_string(port_);
            boost::asio::ip::tcp::resolver::query tcpQuery{host_, servicePort.c_str()};
            iter = r.resolve(tcpQuery,ec);
        }
        if(ec){
            error = ec.message();
            return false;
        }
        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
        stats.push_back("Domain resolve : " + boost::lexical_cast<std::string>(duration) + "us");

        t1 = std::chrono::high_resolution_clock::now();
        boost::asio::connect(sock, iter,ec);
        if(ec){
            error = ec.message();
            return false;
        }
        t2 = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
        stats.push_back("Connect : " + boost::lexical_cast<std::string>(duration) + "us");

        sock.set_option(boost::asio::ip::tcp::no_delay(true));

        if(scheme_ == "https"){
            if(proxyHost_ != ""){
                if(!proxySend(sock, error))
                    return false;
            }
            //ctx.set_default_verify_paths();
            boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23_client);
            boost::asio::ssl::stream<boost::asio::ip::tcp::socket&> stream{sock, ctx};
            stream.set_verify_mode(boost::asio::ssl::verify_none);
            SSL_set_tlsext_host_name(stream.native_handle(), host_.c_str());//tls v1.2 sni
            SSL *ssl = stream.native_handle();
            SSL_CTX *context = ctx.impl();
            t1 = std::chrono::high_resolution_clock::now();
            stream.handshake(boost::asio::ssl::stream_base::client,ec);
            if(ec){
                error = ec.message();
                return false;
            }
            t2 = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
            stats.push_back("SSL Handshake : " + boost::lexical_cast<std::string>(duration) + "us");

            stats.push_back("Handshake Read: " + std::to_string(BIO_number_read(SSL_get_rbio(ssl))) + " bytes");
            stats.push_back("Handshake Written: " + std::to_string(BIO_number_written(SSL_get_wbio(ssl))) + " bytes");

            t1 = std::chrono::high_resolution_clock::now();
            beast::http::write(stream, req,ec);
            if(ec){
                error = ec.message();
                return false;
            }
            t2 = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
            stats.push_back("Write : " + boost::lexical_cast<std::string>(duration) + "us");


            beast::streambuf sb;
            t1 = std::chrono::high_resolution_clock::now();
            beast::http::read(stream, sb, resp,ec);
            if(ec){
                error = ec.message();
                return false;
            }
            t2 = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
            stats.push_back("Read : " + boost::lexical_cast<std::string>(duration) + "us");

            ShowCerts(stream.impl()->ssl,stats);
        }else if(scheme_ == "http"){
            t1 = std::chrono::high_resolution_clock::now();
            beast::http::write(sock, req,ec);
            if(ec){
                error = ec.message();
                return false;
            }
            t2 = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
            stats.push_back("Write : " + boost::lexical_cast<std::string>(duration) + "us");

            beast::streambuf sb;
            t1 = std::chrono::high_resolution_clock::now();
            beast::http::read(sock, sb, resp,ec);
            if(ec){
                error = ec.message();
                return false;
            }
            t2 = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
            stats.push_back("Read : " + boost::lexical_cast<std::string>(duration) + "us");

        }else{
            error = std::string("Uknown scheme.Only HTTP or HTTPS supported.");
            return false;
        }
    }catch (std::exception ex){
        error = ex.what();
        return false;
    }
    return true;
}

//todo ,mund te pershpejtohet, shih beast::to_string
//http://stackoverflow.com/questions/20734831/compress-string-with-gzip-using-qcompress
//http://stackoverflow.com/questions/2690328/qt-quncompress-gzip-data/24949005#24949005

int HttpClient::gzipDecompress(unsigned char *compressData, size_t length, std::string &body)// QByteArray compressData )
{
    //decompress GZIP data
    //strip header and trailer
    compressData += 10;
    length -= 12;

    const int buffersize = 1024*64;
    uint8_t buffer[buffersize];


    z_stream cmpr_stream;
    cmpr_stream.next_in = compressData;
    cmpr_stream.avail_in = length;
    cmpr_stream.total_in = 0;

    cmpr_stream.next_out = buffer;
    cmpr_stream.avail_out = buffersize;
    cmpr_stream.total_out = 0;

    cmpr_stream.zalloc = Z_NULL;
    cmpr_stream.zalloc = Z_NULL;

    if( inflateInit2(&cmpr_stream, -8 ) != Z_OK) {
        return -1;
    }

    //QByteArray uncompressed;
    do {
        int status = inflate( &cmpr_stream, Z_SYNC_FLUSH );

        if(status == Z_OK || status == Z_STREAM_END) {
            body.append((char *)buffer, buffersize - cmpr_stream.avail_out);
            cmpr_stream.next_out = buffer;
            cmpr_stream.avail_out = buffersize;
        } else {
            inflateEnd(&cmpr_stream);
        }

        if(status == Z_STREAM_END) {
            inflateEnd(&cmpr_stream);
            break;
        }

    }while(cmpr_stream.avail_out == 0);
    return 0;
}

void HttpClient::prepareHttpRequest()
{
    std::string hostHeaderValue = host_; //todo ndoshta duhet edhe :port
    // the port # should be included if its not the default port of 80.
    //https://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html#sec14.23
    if(proxyHost_ != ""){
        req.url = scheme_ + "://" + host_ + ":" + std::to_string(port_) + Utils::urlEncode(path_);
    }else{
        req.url = Utils::urlEncode(path_);

    }
    if(!queryString_.empty()){
         req.url += "?" + queryString_;
    }
    req.fields.replace("Host", hostHeaderValue/* + ":" + std::to_string(sock.remote_endpoint().port())*/);
}

bool HttpClient::proxySend(boost::asio::ip::tcp::socket &sock, std::string &error)
{
    boost::system::error_code ec;
    beast::http::request<beast::http::empty_body> proxyReq;
    proxyReq.url = host_ + ":" + std::to_string(port_);
    proxyReq.method = "CONNECT";
    proxyReq.version = 11;
    proxyReq.fields.insert("Proxy-Connection","keep-alive");
    proxyReq.fields.insert("Host",host_);
    beast::http::write(sock, proxyReq,ec);
    if(ec){
        error = ec.message();
        return false;
    }
    beast::http::parser_v1<false, beast::http::string_body,
            beast::http::basic_fields<std::allocator<char>>> p;
    p.set_option(beast::http::skip_body{true});

    beast::streambuf sb1;
    beast::http::parse(sock, sb1, p,ec);
    if(ec){
        error = ec.message();
        return false;
    }
    auto m = p.release();
    if(m.status != 200){
        error = "Proxy error";
        return false;
    }
    return true;
}
