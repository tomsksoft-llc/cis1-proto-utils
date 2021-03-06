#include <cis1_proto_utils/cloexec.h>

#ifdef __linux__
#include <fcntl.h>
#endif

namespace cis1
{

namespace proto_utils
{

namespace detail
{
#ifdef __linux__
void set_cloexec_impl(int fd, boost::system::error_code& ec)
{
    int result = ::fcntl(fd, F_GETFD);
    if(result == -1)
    {
        ec = boost::system::error_code(errno, boost::system::system_category());
        return;
    }
    result = ::fcntl(fd, F_SETFD, result | FD_CLOEXEC);
    if(result == -1)
    {
        ec = boost::system::error_code(errno, boost::system::system_category());
        return;
    }
}
#endif
} // namespace detail

} // namespace proto_utils

} // namespace net
