#ifndef HRPC_UTIL_ANY_H
#define HRPC_UTIL_ANY_H

#include <memory>

namespace hrpc
{
namespace util
{

// Any类型，可以接受任意数据类型
class Any {
public:
    Any() = default;
    ~Any() = default;

    Any(const Any&) = delete;
    Any& operator=(const Any&) = delete;

    Any(Any&&) = default;
    Any& operator=(Any&&) = default;

    template<typename T>
    Any(T data) : base_(std::make_unique<Derive<T>>(data))
    {}

    template<typename T>
    T cast_() {
        Derive<T>* pd = dynamic_cast<Derive<T>*>(base_.get());
        if (pd == nullptr) {
            throw "type is unmatch!";
        }
        return pd->data_;
    }

private:
    // 基类
    class Base {
    public:
        virtual ~Base() = default;
    };

    // 派生类
    template<typename T>
    class Derive : public Base {
    public:
        Derive(T data) : data_(data)
        {}
         
        T data_;    // 保存了任意的其他类型
    };

private:
    std::unique_ptr<Base> base_;
};


} // namespace util
} // namespace hrpc

#endif // HRPC_UTIL_ANY_H