#include "threadpool.h"
#include <iostream>

using namespace hrpc;
using namespace hrpc::util;

const int TASK_MAX_THRESHHOLD = 1024;
const int THREAD_SIZE_THRESHHOLD = 10;
const int THREAD_MAX_IDLE_TIME = 10;    // s

ThreadPool::ThreadPool()
    : initThreadSize_(4),
      threadSizeThreshHold_(THREAD_SIZE_THRESHHOLD),
      curThreadSize_(0),
      idleThreadSize_(0),
      taskSize_(0),
      taskQueMaxThreshHold_(TASK_MAX_THRESHHOLD),
      poolMode_(PoolMode::MODE_FIXED),
      isPoolRunning_(false)
{}

ThreadPool::~ThreadPool() {
    isPoolRunning_ = false;

    std::unique_lock<std::mutex> lock(taskQueMtx_);
    notEmpty_.notify_all();
    exitCond_.wait(lock, [&]()->bool {
        return threads_.empty();
    });
}

// 设置线程池的工作模式
void ThreadPool::setMode(PoolMode mode) {
    if (checkRunningState()) {
        return;
    }
    poolMode_ = mode;
}

// 设置cached模式下线程数量的上限阈值
void ThreadPool::setThreadSizeThreshHold(int threshHold) {
    if (checkRunningState()) {
        return;
    }
    if (poolMode_ == PoolMode::MODE_CACHED) {
        threadSizeThreshHold_ = threshHold;
    }
}

// 设置task队列的上限阈值
void ThreadPool::setTaskQueMaxThreshHold(int threshHold) {
    if (checkRunningState()) {
        return;
    }
    taskQueMaxThreshHold_ = threshHold;
}

// 给线程池提交任务
std::shared_ptr<Result> ThreadPool::subMitTask(std::shared_ptr<Task> sp) {
    std::unique_lock<std::mutex> lock(taskQueMtx_);

    // while (taskQue_.size() == taskQueMaxThreshHold_) {
    //     notFull_.wait(lock);
    // }
    // 等同于以下操作
    // notFull_.wait(lock, [&]()->bool { return taskQue_.size() < taskQueMaxThreshHold_; });

    bool res = notFull_.wait_for(lock, std::chrono::seconds(1), [&]()->bool {
        return taskQue_.size() < (size_t)taskQueMaxThreshHold_;
    });

    if (!res) {
        // 表示notFull_等待1s，条件依然没有满足
        std::cerr << "task queue is full, submit task fail." << std::endl;
        std::shared_ptr<Result> resPtr(new Result(sp, false));
        return resPtr;
    }

    taskQue_.emplace(sp);
    taskSize_ ++;

    notEmpty_.notify_one();

    // cached模式，任务数量比较紧急。场景：小而快的任务
    // 需要根据任务数量和空闲线程数量，判断是否需要开辟新线程
    if (poolMode_ == PoolMode::MODE_CACHED
        && taskSize_ > idleThreadSize_
        && curThreadSize_ < threadSizeThreshHold_)
    {
        // 创建新线程
        std::unique_ptr<Thread> ptr = std::make_unique<Thread>(std::bind(&ThreadPool::threadHandler, this, std::placeholders::_1));
        int threadId = ptr->getId();
        threads_.emplace(threadId, std::move(ptr));
        threads_[threadId]->start();
        // 修改线程个数相关变量
        curThreadSize_++;
        idleThreadSize_++;
        std::cout << "create new thread, id = " << threadId << "   ---   " << std::this_thread::get_id() << std::endl;
    }

    std::shared_ptr<Result> resPtr(new Result(sp));
    return resPtr;
}

//  开启线程池
void ThreadPool::start(unsigned int initThreadSize) {
    isPoolRunning_ = true;
    initThreadSize_ = initThreadSize;
    curThreadSize_ = initThreadSize;

    // 集中创建线程对象
    for (int i = 0; i < initThreadSize_; i++) {
        std::unique_ptr<Thread> ptr = std::make_unique<Thread>(std::bind(&ThreadPool::threadHandler, this, std::placeholders::_1));
        int threadId = ptr->getId();
        threads_.emplace(threadId, std::move(ptr));
    }

    // 启动所有线程
    for (auto it = threads_.begin(); it != threads_.end(); it++) {
        it->second->start();
        idleThreadSize_++;
    }
    std::cout << "init create [ " << initThreadSize_ << " ] threads" << std::endl;
}

// 定义线程函数
void ThreadPool::threadHandler(int threadId) {
    auto lastTime = std::chrono::high_resolution_clock().now();

    // 所有任务必须执行完成，线程池才可以回收所有线程资源
    while (true) {
        std::shared_ptr<Task> task;
        {
            std::unique_lock<std::mutex> lock(taskQueMtx_);

            
            std::cout << "thread id = " << std::this_thread::get_id() << " 尝试获取任务" << std::endl;
            
            while (taskQue_.size() == 0) {
                if (!isPoolRunning_) {
                    // 开始回收当前线程
                    threads_.erase(threadId);
                    std::cout << "threadId = " << std::this_thread::get_id() << " >>> exit" << std::endl;
                    exitCond_.notify_one();
                    return;
                }
                // cached模式下，有可能创建了很多线程，但是空闲时间超过了60s
                // 应该把多余的线程给结束回收掉（超过initThreadSize_的线程回收掉
                // 当前时间 - 上一次线程执行的时间 > 60s
                if (poolMode_ == PoolMode::MODE_CACHED) {
                    if (std::cv_status::timeout == 
                            notEmpty_.wait_for(lock, std::chrono::seconds(1)))
                    {
                        auto now = std::chrono::high_resolution_clock().now();
                        auto dur = std::chrono::duration_cast<std::chrono::seconds>(now - lastTime);
                        if (dur.count() >= THREAD_MAX_IDLE_TIME
                            && curThreadSize_ > initThreadSize_) 
                        {
                            // 开始回收当前线程
                            threads_.erase(threadId);
                            curThreadSize_--;
                            idleThreadSize_--;
                            std::cout << "threadId = " << std::this_thread::get_id() << " >>> exit" << std::endl;
                            return;
                        }
                    }
                } else {
                    notEmpty_.wait(lock);            
                }
            }

            std::cout << "thread id = " << std::this_thread::get_id() << " 获取任务成功 ..." << std::endl;
            
            idleThreadSize_--;

            task = taskQue_.front();
            taskQue_.pop();
            taskSize_ --;

            // 如果依然有剩余任务，继续通知其他线程执行任务
            if (taskQue_.size() > 0) {
                notEmpty_.notify_all();
            }

            notFull_.notify_all();
        } // 把锁释放掉

        if (task != nullptr) {
            task->exec();
        }
        idleThreadSize_++;
        lastTime = std::chrono::high_resolution_clock().now();
    }
}

bool ThreadPool::checkRunningState() const {
    return isPoolRunning_;
}

/////////////////////////// Result方法实现 ///////////////////////////
Result::Result(std::shared_ptr<Task> task, bool isValid) 
    : task_(task),
      isValid_(isValid)
{
    task_->setResult(this);
}

void Result::setVal(std::any&& any) {
    any_ = std::move(any);
    sem_.post();
}

const std::any& Result::get() {
    if (!isValid_) {
            return "";
    }
    sem_.wait();
    return any_;
}