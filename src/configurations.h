#ifndef CONFIGURATIONS_H
#define CONFIGURATIONS_H
#include <fstream>
#include <map>
#include <string>
#include <thread>
#include <mutex>
#include <QString>
class Configurations
{
public:
    static Configurations& get()
    {
        static Configurations instance;
        return instance;
    }
    int loadConfigurations();
    int saveConfigurations();
    std::string getValue(std::string key);
    void setValue(std::string key, std::string value);
    void setProxyHost(std::string value);
    void setProxyPort(std::string value);
    std::string getProxyHost();
    int getProxyPort();
    void setProxySetting(int value);
    int getProxySetting();
    void setHttpClientV2MainSplitterSizes(QList<int> sizes);
    QList<int> getHttpClientV2MainSplitterSizes();
    void setHttpClientV2HistorySplitterSizes(QList<int> sizes);
    QList<int> getHttpClientV2HistorySplitterSizes();
    void setHttpClientV2RequestSplitterSizes(int sizes);
    int getHttpClientV2RequestSplitterSizes();
    void setHttpClientV2ResponseSplitterSizes(QList<int> sizes);
    QList<int> getHttpClientV2ResponseSplitterSizes();
    void setHistoryLastSavedDir(std::string lastSavedDir);
    std::string getHistoryLastSavedDir();
private:

    Configurations();
    Configurations(const Configurations&);
    Configurations& operator=(const Configurations&);
    std::map<std::string,std::string> storedConfig;
    QString configurationFileName;
    std::mutex _mutex;
};


#endif // CONFIGURATIONS_H
