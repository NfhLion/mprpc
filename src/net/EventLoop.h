#ifndef HRPC_NET_EVENTLOOP_H
#define HRPC_NET_EVENTLOOP_H

#include <memory>
#include <atomic>
#include <functional>
#include <vector>

namespace hrpc
{
namespace net
{

class EPollPoller;
class Channel;

class EventLoop {
public:
    EventLoop();
    ~EventLoop();

    void loop();
    void quit();

    void updateChannel(Channel* channel);
    void removeChannel(Channel* channel);
    bool hasChannel(Channel* channel);

private:
    typedef std::vector<Channel*> ChannelList;

    bool looping_;
    std::atomic<bool> quit_;
    bool eventHandling_; 

    std::unique_ptr<EPollPoller> poller_;
    ChannelList activeChannels_;
    Channel* currentActiveChannel_;
};

}   // namespace net
};  // namespace hrpc

#endif // HRPC_NET_EVENTLOOP_H