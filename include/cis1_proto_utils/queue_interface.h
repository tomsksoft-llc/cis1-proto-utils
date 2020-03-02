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

    virtual uint32_t add_close_handler(std::function<void()> on_close) = 0;

    virtual void remove_close_handler(uint32_t id) = 0;

    virtual void close() = 0;
};

} // namespace proto_utils

} // namespace cis1
