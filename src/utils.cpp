#include "utils.h"

#include <sstream>
#include <stdexcept>
#include <iomanip>
#include <locale>
#include <codecvt>
#ifdef _WIN32
#include <Shlobj.h>
#include <Objbase.h>
#endif
#include <QDebug>
#include <QDir>

namespace Utils {

    std::string getDataDirectory(){
        std::string dirStr;
#ifdef _WIN32
        wchar_t *dirPath;
        HRESULT res = SHGetKnownFolderPath(FOLDERID_ProgramData,KF_FLAG_DEFAULT,NULL,&dirPath);
        qDebug() << dirPath;
        if(res == S_OK){
            dirPath[259] = 0;
            std::wstring ws(dirPath);
            //dirStr.insert(ws.begin(), ws.end());

            using convert_type = std::codecvt_utf8<wchar_t>;
            std::wstring_convert<convert_type, wchar_t> converter;
            dirStr = converter.to_bytes( ws );
        }else{
            dirStr = "/home/";
        }
        if(dirPath){
            CoTaskMemFree(dirPath);
        }
#else
        dirStr = QDir::homePath().toStdString();
#endif

        return dirStr;
    }

    std::string urlEncode(const std::string &toEncode) {
        std::ostringstream out;

        for(std::string::size_type i=0; i < toEncode.length(); ++i) {
            short t = toEncode.at(i);

            if(
                t == 45 ||          // hyphen
                (t >= 48 && t <= 57) ||       // 0-9
                (t >= 65 && t <= 90) ||       // A-Z
                t == 95 ||          // underscore
                (t >= 97 && t <= 122) ||  // a-z
                t == 126              ||  // tilde
                t == 46               ||    // .
                t == 47                     // /
            ) {
                out << toEncode.at(i);
            } else {
                out << charToHex(toEncode.at(i));
            }
        }

        return out.str();
    }

    std::string urlDecode(const std::string &toDecode) {
        std::ostringstream out;

        for(std::string::size_type i=0; i < toDecode.length(); ++i) {
            if(toDecode.at(i) == '%') {
                std::string str(toDecode.substr(i+1, 2));
                out << hexToChar(str);
                i += 2;
            } else {
                out << toDecode.at(i);
            }
        }

        return out.str();
    }

    std::string charToHex(unsigned char c) {
        short i = c;

        std::stringstream s;

        s << "%" << std::setw(2) << std::setfill('0') << std::hex << i;

        return s.str();
    }

    unsigned char hexToChar(const std::string &str) {
        short c = 0;

        if(!str.empty()) {
            std::istringstream in(str);

            in >> std::hex >> c;

            if(in.fail()) {
                throw std::runtime_error("stream decode failure");
            }
        }

        return static_cast<unsigned char>(c);
    }

    std::string urlQueryStringEncode(const std::string &toEncode)
    {
        std::ostringstream out;

        for(std::string::size_type i=0; i < toEncode.length(); ++i) {
            short t = toEncode.at(i);

            if(
                t == 45 ||          // hyphen
                (t >= 48 && t <= 57) ||       // 0-9
                (t >= 65 && t <= 90) ||       // A-Z
                t == 95 ||          // underscore
                (t >= 97 && t <= 122) ||  // a-z
                t == 126              ||  // tilde
                t == 46               ||    // .
                t == 47               ||      // /
                t == 61               ||
                t == 38
            ) {
                out << toEncode.at(i);
            } else {
                out << charToHex(toEncode.at(i));
            }
        }

        return out.str();
    }

}
