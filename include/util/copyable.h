#ifndef HRPC_BASE_COPYABLE_H
#define HRPC_BASE_COPYABLE_H

namespace hrpc
{

/// A tag class emphasises the objects are copyable.
/// The empty base class optimization applies.
/// Any derived class of copyable should be a value type.
class copyable
{
 protected:
  copyable() = default;
  ~copyable() = default;
};

}  // namespace hrpc

#endif  // HRPC_BASE_COPYABLE_H
