#ifndef HRPC_BASE_NONCOPYABLE_H
#define HRPC_BASE_NONCOPYABLE_H

namespace hrpc
{

class noncopyable
{
 public:
  noncopyable(const noncopyable&) = delete;
  void operator=(const noncopyable&) = delete;

 protected:
  noncopyable() = default;
  ~noncopyable() = default;
};

}  // namespace hrpc

#endif  // HRPC_BASE_NONCOPYABLE_H
