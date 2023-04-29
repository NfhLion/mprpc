#ifndef HRPC_UTIL_SEMAPHORE_H
#define HRPC_UTIL_SEMAPHORE_H

#include <mutex>
#include <condition_variable>
#include <atomic>

namespace hrpc
{
namespace util
{

// 信号量
class Semaphore {
public:
    Semaphore(int resLimit = 0)
        : resLimit_(resLimit),
          isExit_(false)
    {}
    ~Semaphore() {
        isExit_ = true;
    }

    // 获取一个信号量资源
    void wait() {
        if (isExit_) {
            return;
        }
        std::unique_lock<std::mutex> lock(mtx_);
        cond_.wait(lock, [&]()->bool{
            return resLimit_ > 0;
        });
        resLimit_--;
    }

    // 增加一个信号量资源
    void post() {
        if (isExit_) {
            return;
        }
        std::unique_lock<std::mutex> lock(mtx_);
        resLimit_++;
        cond_.notify_all();
    }
private:
    int resLimit_;                      // 资源计数
    std::mutex mtx_;
    std::condition_variable cond_;
    std::atomic_bool isExit_;
};

} // namespace util
} // namespace hrpc

#endif // HRPC_UTIL_SEMAPHORE_H