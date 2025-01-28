#include "../include/requesthandler.h"
namespace thu
{

void RequestHandler::receivedRequest()
{
    m_queue.push(m_taskId);
    m_taskId++;
}

int RequestHandler::respondRequest()
{
    return m_queue.pop();
}

}