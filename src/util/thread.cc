#include "thread.h"

using namespace hrpc::util;

/////////////////////////// 线程方法实现 ///////////////////////////
int Thread::generateId_ = 0;

Thread::Thread(ThreadFunc func)
    : func_(func),
      threadId_(generateId_++)
{}

Thread::~Thread()
{}

int Thread::getId() const {
    return threadId_;
}

void Thread::start() {
    std::thread t(func_, threadId_);
    t.detach();
}