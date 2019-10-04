#pragma once

#include <functional>

#include <boost/asio/executor.hpp>
#include <boost/asio/buffer.hpp>

namespace cis1
{

namespace proto_utils
{

struct queue_interface
{
    virtual ~queue_interface() = default;
    virtual void send_text(
            boost::asio::const_buffer buffer,
            std::function<void()> on_write) = 0;
    virtual bool is_full() = 0;
    virtual boost::asio::executor get_executor() = 0;
};

} // namespace proto_utils

} // namespace cis1
