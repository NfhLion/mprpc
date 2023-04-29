#include "EventLoop.h"
#include "Channel.h"
#include "Poller.h"

#include <assert.h>
#include <algorithm>

using namespace hrpc;
using namespace hrpc::net;

const int kPollTimeMs = 10000;

EventLoop::EventLoop()
  : looping_(false),
    quit_(false),
    eventHandling_(false),
    poller_(new EPollPoller(this)),
    currentActiveChannel_(NULL)
{
    // LOG_DEBUG << "EventLoop created " << this << " in thread " << threadId_;

}

EventLoop::~EventLoop()
{
}

void EventLoop::loop() {
    assert(!looping_);
    looping_ = true;
    quit_ = false;  // FIXME: what if someone calls quit() before loop() ?
    // LOG_TRACE << "EventLoop " << this << " start looping";

    while (!quit_)
    {
        activeChannels_.clear();
        poller_->poll(kPollTimeMs, &activeChannels_);
        // TODO sort channel by priority
        eventHandling_ = true;
        for (Channel* channel : activeChannels_)
        {
            currentActiveChannel_ = channel;
            currentActiveChannel_->handleEvent();
        }
        currentActiveChannel_ = NULL;
        eventHandling_ = false;
    }

  // LOG_TRACE << "EventLoop " << this << " stop looping";
  looping_ = false;
}

void EventLoop::quit()
{
  quit_ = true;
}



void EventLoop::updateChannel(Channel* channel) {
    assert(channel->ownerLoop() == this);
    poller_->updateChannel(channel);
}

void EventLoop::removeChannel(Channel* channel) {
    assert(channel->ownerLoop() == this);
    if (eventHandling_) {
        assert(currentActiveChannel_ == channel ||
            std::find(activeChannels_.begin(), activeChannels_.end(), channel) == activeChannels_.end());
    }
    poller_->removeChannel(channel);
}
