#ifndef REQUESTRESPONSEQUEUE_H
#define REQUESTRESPONSEQUEUE_H
#include <ctime>
#include <thread>
#include <list>
#include <atomic>
//http://codingways.blogspot.com/2012/08/implementing-thread-safe-queue-in-c11.html
#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <stdexcept>
#include <QString>
#include "mtypes.h"

class ServerRequestResponse{
public :
    RequestCtx reqData;
    QString rawResponse;
    QString rawRequest;
    QString errorData;
};
class RequestResponseQueue
{
public:
    RequestResponseQueue();
    ~RequestResponseQueue();
    void Push(ServerRequestResponse pValue);
    ServerRequestResponse Pop();
    ServerRequestResponse TryPop();
	bool Empty() const;
	bool is_stopped() const;
	void stop();
	void start();
    size_t size();
private:
    std::queue<ServerRequestResponse > mData;
	// check empty() method for description of "mutable"
	mutable std::mutex mMut;
	std::condition_variable mEmptyCondition;
	//void wait_for_debugging();
	bool m_isProgramStopped = false;
};

#endif
