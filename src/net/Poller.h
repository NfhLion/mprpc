#ifndef HRPC_NET_POLLER_H
#define HRPC_NET_POLLER_H

#include <unordered_map>
#include <vector>

#include "EventLoop.h"

struct epoll_event;

namespace hrpc
{
namespace net
{

class Channel;

class EPollPoller {
public:
    typedef std::vector<Channel*> ChannelList;
    typedef std::unordered_map<int, Channel*> ChannelMap;

    EPollPoller(EventLoop* loop);
    ~EPollPoller();

    void poll(int timeoutMs, ChannelList* activeChannels);
    void updateChannel(Channel* channel);
    void removeChannel(Channel* channel);
private:

    static const int kInitEventListSize = 16;

    void fillActiveChannels(int numEvents,
                            ChannelList* activeChannels) const;
    void update(int operation, Channel* channel);

    typedef std::vector<struct epoll_event> EventList;

    int epollfd_;
    EventList events_;
    ChannelMap channels_;
    
    EventLoop* loop_;
};

}   // namespace net
};  // namespace hrpc

#endif // HRPC_NET_POLLER_H