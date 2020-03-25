#include "configurations.h"
#include "lib/json.hpp"
#include <QFile>
#include <QMessageBox>
#include "utils.h"
#include <QDir>
Configurations::Configurations() :
    configurationFileName("configurations.cfg")
{

}

std::string Configurations::getValue(std::string key)
{
    std::lock_guard<std::mutex> lock(_mutex);
    try{
        return storedConfig.at(key);
    }catch(std::exception ex){
        return "";
    }
}

void Configurations::setValue(std::string key, std::string value)
{
    std::lock_guard<std::mutex> lock(_mutex);
    storedConfig[key] = value;
}

void Configurations::setProxyHost(std::string value)
{
    Configurations::get().setValue("ProxyHost",value);
}

void Configurations::setProxyPort(std::string value)
{
    Configurations::get().setValue("ProxyPort",value);
}

void Configurations::setHttpClientV2MainSplitterSizes(QList<int> sizes)
{
    std::string value;
    for(auto it : sizes){
        value += QString::number(it).toStdString() + ".";
    }
    Configurations::get().setValue("HttpClientV2MainSplitterSizes",value);
}

QList<int> Configurations::getHttpClientV2MainSplitterSizes()
{
    std::string value = Configurations::get().getValue("HttpClientV2MainSplitterSizes");
    QString tmpVal = QString::fromStdString(value);
    QStringList mm = tmpVal.split(".",QString::SkipEmptyParts);
    QList<int> retList;
    for(auto i : mm){
        retList.push_back(i.toInt());
    }
    return retList;
}

void Configurations::setHttpClientV2HistorySplitterSizes(QList<int> sizes)
{
    std::string value;
    for(auto it : sizes){
        value += QString::number(it).toStdString() + ".";
    }
    Configurations::get().setValue("HttpClientV2HistorySplitterSizes",value);
}

QList<int> Configurations::getHttpClientV2HistorySplitterSizes()
{
    std::string value = Configurations::get().getValue("HttpClientV2HistorySplitterSizes");
    QString tmpVal = QString::fromStdString(value);
    QStringList mm = tmpVal.split(".",QString::SkipEmptyParts);
    QList<int> retList;
    for(auto i : mm){
        retList.push_back(i.toInt());
    }
    return retList;
}

void Configurations::setHttpClientV2RequestSplitterSizes(int sizes)
{
    Configurations::get().setValue("HttpClientV2RequestSplitterSizes",QString::number(sizes).toStdString());
}

int Configurations::getHttpClientV2RequestSplitterSizes()
{
    std::string value = Configurations::get().getValue("HttpClientV2RequestSplitterSizes");
    QString tmpVal = QString::fromStdString(value);
    return tmpVal.toInt();
}

void Configurations::setHttpClientV2ResponseSplitterSizes(QList<int> sizes)
{
    std::string value;
    for(auto it : sizes){
        value += QString::number(it).toStdString() + ".";
    }
    Configurations::get().setValue("HttpClientV2ResponseSplitterSizes",value);
}

QList<int> Configurations::getHttpClientV2ResponseSplitterSizes()
{
    std::string value = Configurations::get().getValue("HttpClientV2ResponseSplitterSizes");
    QString tmpVal = QString::fromStdString(value);
    QStringList mm = tmpVal.split(".",QString::SkipEmptyParts);
    QList<int> retList;
    for(auto i : mm){
        retList.push_back(i.toInt());
    }
    return retList;
}

void Configurations::setHistoryLastSavedDir(std::string lastSavedDir)
{
    Configurations::get().setValue("HistoryLastSavedDir", lastSavedDir);
}

std::string Configurations::getHistoryLastSavedDir()
{
    return Configurations::get().getValue("HistoryLastSavedDir");
}

std::string Configurations::getProxyHost()
{
    return Configurations::get().getValue("ProxyHost");
}

int Configurations::getProxyPort()
{
    return std::atoi(Configurations::get().getValue("ProxyPort").c_str());
}

void Configurations::setProxySetting(int value)
{
    Configurations::get().setValue("HttpClientProxySettings",QString::number(value).toStdString());
}

int Configurations::getProxySetting()
{
    return std::atoi(Configurations::get().getValue("HttpClientProxySettings").c_str());
}

int Configurations::saveConfigurations()
{

    QString dataDirectory = QString::fromStdString(Utils::getDataDirectory() + "/WebTest/");

    nlohmann::json configurations;
    std::lock_guard<std::mutex> lock(_mutex);
    configurations = storedConfig;

    QFile file(dataDirectory + configurationFileName);
    if (!file.open(QIODevice::WriteOnly)) {
        return -1;
    }
    std::string jsonstring = configurations.dump(4);
    file.write(jsonstring.c_str(),jsonstring.length());
    file.close();
    return 0;
}

int Configurations::loadConfigurations()
{
    QString dataDirectory =  QString::fromStdString(Utils::getDataDirectory() + "/WebTest/");

    QFile configFile(dataDirectory + configurationFileName);
    if(configFile.exists()){
        if(!configFile.open(QIODevice::ReadOnly)){
            return -1;
        }

        QByteArray rawJson = configFile.readAll();
        if(rawJson.size() == 0){
            return -1;
        }
        try{
            nlohmann::json configJsonDoc = nlohmann::json::parse(rawJson.toStdString());
            auto vv = configJsonDoc.get<std::map <std::string, nlohmann::json>>();
            std::lock_guard<std::mutex> lock(_mutex);
            for (auto i : vv)
            {
                storedConfig.insert(std::make_pair(i.first,i.second.get<std::string>()));
            }
        }catch (std::exception ex){
            return -1;
        }
    }
    return 0;
}

