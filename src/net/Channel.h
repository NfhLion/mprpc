#ifndef HRPC_NET_CHANNEL_H
#define HRPC_NET_CHANNEL_H

#include "noncopyable.h"

#include <functional>
#include <memory>

namespace hrpc
{
namespace net
{

class EventLoop;

class Channel {
public:
    typedef std::function<void()> EventCallback;

    Channel(EventLoop* loop, int fd);
    ~Channel();

    void handleEvent();
    void setReadCallback(EventCallback cb)
    { readCallback_ = std::move(cb); }
    void setWriteCallback(EventCallback cb)
    { writeCallback_ = std::move(cb); }
    void setCloseCallback(EventCallback cb)
    { closeCallback_ = std::move(cb); }
    void setErrorCallback(EventCallback cb)
    { errorCallback_ = std::move(cb); }

    int fd() const { return fd_; }
    int events() const { return events_; }
    int index() const { return index_; }
    void set_index(int index) { index_ = index;}
    void set_revents(int revt) { revents_ = revt; } // used by pollers
    // int revents() const { return revents_; }
    bool isNoneEvent() const { return events_ == kNoneEvent; }

    void enableReading() { events_ |= kReadEvent; update(); }
    void disableReading() { events_ &= ~kReadEvent; update(); }
    void enableWriting() { events_ |= kWriteEvent; update(); }
    void disableWriting() { events_ &= ~kWriteEvent; update(); }
    void disableAll() { events_ = kNoneEvent; update(); }
    bool isWriting() const { return events_ & kWriteEvent; }
    bool isReading() const { return events_ & kReadEvent; }

    EventLoop* ownerLoop() { return loop_; }
    void remove();

    void tie(const std::shared_ptr<void>&);

private:
    void update();

    int fd_;
    EventLoop* loop_;

    int index_;

    int events_;
    int revents_;

    std::weak_ptr<void> tie_;
    bool tied_;

    EventCallback readCallback_;
    EventCallback writeCallback_;
    EventCallback closeCallback_;
    EventCallback errorCallback_;

    static const int kNoneEvent;
    static const int kReadEvent;
    static const int kWriteEvent;

    bool eventHandling_;
};

}   // namespace net
};  // namespace hrpc

#endif // HRPC_NET_CHANNEL_H