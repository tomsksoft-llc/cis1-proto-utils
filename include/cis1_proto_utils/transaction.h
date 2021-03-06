#pragma once

#include <memory>
#include <string>
#include <optional>

#include <boost/asio/executor.hpp>

#include <tpl_reflect/meta_converter.h>
#include <tpl_reflect/json_engine.h>

#include <cis1_proto_utils/queue_interface.h>

namespace cis1
{

namespace proto_utils
{

class transaction
{
public:
    transaction(
            const std::weak_ptr<queue_interface>& queue,
            uint64_t transaction_id);

    transaction(const transaction& tr);

    template <class Payload>
    void send(const Payload& p) const
    {
        if(auto queue = queue_.lock(); queue)
        {
            rapidjson::Document d;

            const auto& conv = Payload::get_converter();

            auto event = conv.template name<json::engine>();

            prepare_response(d, event);

            d.AddMember(
                    rapidjson::Value().SetString("errorMessage"),
                    rapidjson::Value().SetString(""),
                    d.GetAllocator());

            conv.template set<json::engine>(
                    d["data"],
                    p,
                    d.GetAllocator());

            send(queue, d);
        }
    }

    template <class Payload>
    void send_error(const Payload& p, const std::string& err) const
    {
        if(auto queue = queue_.lock(); queue)
        {
            rapidjson::Document d;

            const auto& conv = Payload::get_converter();

            auto event = conv.template name<json::engine>();

            prepare_response(d, event);

            d.AddMember(
                    rapidjson::Value().SetString("errorMessage"),
                    rapidjson::Value().SetString(
                            err.c_str(),
                            err.length(),
                            d.GetAllocator()),
                    d.GetAllocator());

            conv.template set<json::engine>(
                    d["data"],
                    p,
                    d.GetAllocator());

            send(queue, d);
        }
    }

    void send_error(const std::string& err) const;

    std::optional<boost::asio::executor> get_executor() const;

    std::weak_ptr<queue_interface> get_queue() const;

private:
    const uint64_t transaction_id_;
    const std::weak_ptr<queue_interface> queue_;
    static const std::string default_error_;

    void prepare_response(rapidjson::Document& doc, std::string event) const;

    void send(
            const std::shared_ptr<queue_interface>& queue,
            const rapidjson::Value& v) const;
};

} // namespace proto_utils

} // namespace cis1
