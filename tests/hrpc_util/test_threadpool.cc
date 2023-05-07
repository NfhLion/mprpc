#include "threadpool.h"
#include <iostream>
#include <chrono>
#include <any>

using namespace hrpc::util;

class MyTask : public Task {
public:
    MyTask(int begin, int end)
        : begin_(begin),
          end_(end)
    {}
    std::any run() {
        std::cout << "thread id = " << std::this_thread::get_id() << " begin" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(3));

        size_t sum = 0;
        for (int i = begin_; i <= end_; i++) {
            sum += i;
        }

        std::cout << "thread id = " << std::this_thread::get_id() << " endl" << std::endl;
        return sum;
    }
private:
    int begin_;
    int end_;
};

int main(int argc, char* agrv[]) {
    {
        ThreadPool pool;
        pool.setMode(PoolMode::MODE_CACHED);
        pool.start(2);

        
        std::shared_ptr<Result> res1 = pool.subMitTask(std::make_shared<MyTask>(1, 1000000));
        std::shared_ptr<Result> res2 = pool.subMitTask(std::make_shared<MyTask>(1000001, 2000000));
        // pool.subMitTask(std::make_shared<MyTask>(200001, 300000));
        // pool.subMitTask(std::make_shared<MyTask>(200001, 300000));
        // pool.subMitTask(std::make_shared<MyTask>(200001, 300000));

        // size_t r1 = res1->get().cast_<size_t>();
        // size_t r2 = res2->get().cast_<size_t>();
        size_t r1 = std::any_cast<size_t>(res1->get());
        size_t r2 = std::any_cast<size_t>(res2->get());

        // std::cout << "sum = " << r1 + r2 << std::endl;

        // size_t sum = 0;
        // for (int i = 1; i <= 300000; i ++) {
        //     sum += i;
        // }
        // std::cout << "sum = " << sum << std::endl;
    }
    std::cout << "main over" << std::endl;
    getchar();

    // ThreadPool pool;
    // pool.start(4);
    // Result res1 = pool.subMitTask(std::make_shared<MyTask>(1, 100000));
    // size_t r1 = res1.get().cast_<size_t>();

    // std::cout << r1 << std::endl;

    return 0;
}