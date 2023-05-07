#ifndef HRPC_UTIL_THREAD_H
#define HRPC_UTIL_THREAD_H

#include <thread>
#include <functional>

namespace hrpc
{
namespace util
{

class Thread {
public:
    // 线程函数对象类型
    using ThreadFunc = std::function<void(int)>;
    
    Thread(ThreadFunc func);
    ~Thread();
    
    // 启动线程
    void start();

    int getId() const;
private:
    ThreadFunc func_;
    int threadId_;      // 保存线程id

    static int generateId_;
};

} // namespace util
} // namespace hrpc

#endif // HRPC_UTIL_THREAD_H