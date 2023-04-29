#include "Channel.h"
#include "EventLoop.h"

#include <poll.h>
#include <assert.h>


using namespace hrpc;
using namespace hrpc::net;

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = POLLIN | POLLPRI;
const int Channel::kWriteEvent = POLLOUT;

Channel::Channel(EventLoop* loop, int fd)
    : loop_(loop),
      fd_(fd),
      index_(-1),
      events_(0),
      revents_(0),
      tied_(false),
      eventHandling_(false)

{}

Channel::~Channel() {
    assert(!eventHandling_);
}

void Channel::remove() {
    assert(isNoneEvent());
    loop_->removeChannel(this);
}

void Channel::tie(const std::shared_ptr<void>& obj) {
    tie_ = obj;
    tied_ = true;
}

void Channel::handleEvent() {
    std::shared_ptr<void> guard;
    if (tied_) {
        guard = tie_.lock();
    }

    eventHandling_ = true;
    // LOG_TRACE << reventsToString();
    if ((revents_ & POLLHUP) && !(revents_ & POLLIN))
    {
        if (closeCallback_) closeCallback_();
    }

    if (revents_ & POLLNVAL)
    {
        // LOG_WARN << "fd = " << fd_ << " Channel::handle_event() POLLNVAL";
    }

    if (revents_ & (POLLERR | POLLNVAL))
    {
        if (errorCallback_) errorCallback_();
    }
    if (revents_ & (POLLIN | POLLPRI | POLLRDHUP))
    {
        if (readCallback_) readCallback_();
    }
    if (revents_ & POLLOUT)
    {
        if (writeCallback_) writeCallback_();
    }
    eventHandling_ = false;
}

void Channel::update() {
    loop_->updateChannel(this);
}