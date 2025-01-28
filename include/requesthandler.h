#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H
#include "messagequeue.h"
namespace thu
{
class RequestHandler
{
public:
    void receivedRequest();
    int respondRequest();
private:
    AtomicMessageQueue<int> m_queue;
    int m_taskId{};
};

} // namespace thuclass RequestHandler

#endif