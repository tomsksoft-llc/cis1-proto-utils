#pragma once

#include <boost/system/error_code.hpp>

namespace cis1
{

namespace proto_utils
{

namespace detail
{
    void set_cloexec_impl(int fd, boost::system::error_code& ec);
} // namespace detail

template <class T>
void set_cloexec(T& handle, boost::system::error_code& ec)
{
#ifdef __linux__
    int native_handle = handle.native_handle();
    detail::set_cloexec_impl(native_handle, ec);
#endif
}

} // namespace proto_utils

} // namespace cis1
