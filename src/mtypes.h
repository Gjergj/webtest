#ifndef TYPES_H
#define TYPES_H

#include <unordered_map>
#include "lib/json.hpp"
#include <list>

struct RequestCtx
{
    std::list<std::pair<std::string,std::string>> headers;
    std::string url;
    int version = 11;
    std::string body;
    std::string host;
    std::string protocolType;
    size_t bodyLength = 0;
    std::string requestType; //PUT,GET,POST
    uint64_t requestTime = 0; //time request is issued
    std::string path;
    int responseStatusCode = 0;
    std::string responseStatusDescription;
    void clear(){
        headers.clear();
        url.clear();
        protocolType.clear();
        body.clear();
        bodyLength = 0;
        host.clear();
        version = 11;
        requestType.clear();
        requestTime = 0;
        path.clear();
        responseStatusCode = 0;
        responseStatusDescription.clear();
    }
    bool serialise(nlohmann::json &reqData){
        reqData["Url"] = url;
        reqData["Host"] = host;
        reqData["ProtocolType"] = protocolType;
        reqData["Version"] = version;
        reqData["Body"] = body;
        reqData["BodyLength"] = bodyLength;
        reqData["RequestType"] = requestType;
        reqData["RequestTime"] = requestTime;
        reqData["Path"] = path;
        reqData["ResponseStatusCode"] = responseStatusCode;
        reqData["ResponseStatusDescription"] = responseStatusDescription;
        for(auto it : headers){
            reqData["RequestHeaders"].push_back(nlohmann::json::object_t::value_type(it.first,it.second));
        }
        return true;
    }
    bool deSerialize(const nlohmann::json &json){
        url = json["Url"];
        host = json.value("Host","");
        protocolType = json.value("ProtocolType","");
        version = json["Version"];
        body = json["Body"];
        bodyLength = json["BodyLength"];
        requestType = json["RequestType"];
        requestTime = json["RequestTime"];
        path = json["Path"];
        responseStatusCode = json["ResponseStatusCode"];
        responseStatusDescription = json["ResponseStatusDescription"];

        for (auto it = json["RequestHeaders"].begin(); it != json["RequestHeaders"].end(); ++it)
        {
            headers.push_back(std::make_pair(it.key() , it.value().get<nlohmann::json::string_t>()));
        }
        return true;
    }
};

struct ResponseDataCtx
{
    std::unordered_map<std::string,std::string> headers;
    std::string responseBody;
    uint64_t responseTime;
    int responseBodyLength = 0;
    int responseStatusCode = 0;
    std::string responseStatusDescription;
    void clear(){
        headers.clear();
        responseBody.clear();
        responseTime = 0;
        responseBodyLength = 0;
        responseStatusCode = 0;
        responseStatusDescription.clear();
    }
    bool serialise(nlohmann::json &resData){
        resData["Body"] = responseBody;
        resData["ResponseTime"] = responseTime;
        resData["BodyLength"] = responseBodyLength;
        resData["ResponseStatusCode"] = responseStatusCode;
        resData["ResponseStatusDescription"] = responseStatusDescription;
        resData["ResponseHeaders"] = headers;
        return true;
    }
    bool deSerialize(nlohmann::json &json){
        responseBody = json["Body"];
        responseTime = json["ResponseTime"];
        responseBodyLength = json["BodyLength"];
        responseStatusCode = json["ResponseStatusCode"];
        responseStatusDescription = json["ResponseStatusDescription"];

        for (auto it = json["ResponseHeaders"].begin(); it != json["ResponseHeaders"].end(); ++it)
        {
            headers.insert(std::make_pair(it.key() , it.value().get<nlohmann::json::string_t>()));
        }
        return true;
    }
};

struct HttpServerProjectCtx {
    std::string domain;
    std::string IPListen;
    bool http = true;
    bool https = true;
    int httpPort = 8081;
    int httpsPort = 443;
    std::list<RequestCtx> routes;
    void clear(){
        routes.clear();
        domain.clear();
        IPListen.clear();
        http = true;
        https = true;
        httpPort = 8081;
        httpsPort = 443;
    }
    bool serialise(nlohmann::json &serverProject){
        serverProject["Domain"] = domain;
        serverProject["IpListen"] = IPListen;
        serverProject["Http"] = http;
        serverProject["HttpPort"] = httpPort;
        serverProject["Https"] = https;
        serverProject["HttpsPort"] = httpsPort;
        nlohmann::json allProject = nlohmann::json::array();
        for(auto route : routes){
            nlohmann::json requestsVal = nlohmann::json::object();
            route.serialise(requestsVal);
            allProject.push_back(requestsVal);
        }
        serverProject["Routes"] = allProject;
        return true;
    }
    bool deSerialize(nlohmann::json &json){
        domain = json["Domain"];
        IPListen = json["IpListen"];
        http = json["Http"];
        httpPort = json["HttpPort"];
        https = json["Https"];
        httpsPort = json["HttpsPort"];

        for (auto it = json["Routes"].begin(); it != json["Routes"].end(); ++it)
        {
            RequestCtx route;
            route.deSerialize(*it);
            routes.push_back(route);
        }
        return true;
    }
};

struct HttpSession
{
    ResponseDataCtx responseData;
    RequestCtx requestData;

    void clear(){
        responseData.clear();
        requestData.clear();
    }
    bool serialise(nlohmann::json &sessionJson){
        nlohmann::json responseJson;
        responseData.serialise(responseJson);
        nlohmann::json requestJson;
        requestData.serialise(requestJson);
        sessionJson["Request"] = requestJson;
        sessionJson["Response"] = responseJson;
        return true;
    }
    bool deSerialize(nlohmann::json &json){
        nlohmann::json requestJson = json["Request"];
        requestData.deSerialize(requestJson);
        nlohmann::json responseJson = json["Response"];
        responseData.deSerialize(responseJson);
        return true;
    }
};

#endif // TYPES_H
