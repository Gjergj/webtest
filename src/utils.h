#ifndef UTILITY_H
#define UTILITY_H
#include <string>
namespace Utils {
    std::string urlEncode(const std::string &url);
    std::string urlDecode(const std::string &encoded);

    std::string urlQueryStringEncode(const std::string &toEncode);

    std::string charToHex(unsigned char c);
    unsigned char hexToChar(const std::string &str);
    std::string getDataDirectory();
    enum UrlEncodingType{
        None = 0,
        QueryString = 1
    };
}

#endif // UTILITY_H
