#pragma once

#include <functional>
#include <map>
#include <memory>

#include <rapidjson/document.h>

#include <tpl_reflect/json_engine.h>
#include <tpl_helpers/overloaded.h>

#include <cis1_proto_utils/queue_interface.h>
#include <cis1_proto_utils/transaction.h>
#include <cis1_proto_utils/const_stream_adapter.h>
#include <cis1_proto_utils/protocol_message.h>

namespace cis1
{

namespace proto_utils
{

template <class... Args>
class event_dispatcher
{
private:
    enum class error
    {
        parse,
        invalid_json,
    };

public:
    template <class ReqType>
    using default_event_handler_t = void(
            Args...,
            const ReqType&,
            transaction);

    void dispatch(
            Args... ctx,
            bool text,
            boost::asio::const_buffer buffer,
            size_t bytes_transferred,
            const std::shared_ptr<queue_interface>& queue)
    {
        if(text)
        {
            auto msg = parse_protocol_message(buffer, bytes_transferred);

            std::visit(
                    meta::overloaded{
                    [&](const error&)
                    {
                        transaction(queue, 0).send_error("Invalid json.");
                    },
                    [&](const protocol_message& msg)
                    {
                        transaction tr(
                                queue,
                                msg.transaction_id);

                        if(auto it = event_handlers_.find(msg.event);
                                it != event_handlers_.end())
                        {
                            (it->second)(
                                    std::forward<Args>(ctx)...,
                                    msg.data,
                                    std::move(tr));
                        }
                        else
                        {
                            tr.send_error("Unknown eventId.");
                        }
                    }},
                    msg);
        }
    }

    template <class ReqType>
    void add_event_handler(
            const std::function<default_event_handler_t<ReqType>>& cb)
    {
        const auto& conv = ReqType::get_converter();

        event_handlers_.insert({
                conv.template name<json::engine>(),
                [cb](
                     Args... ctx,
                     const rapidjson::Value& json,
                     transaction tr)
                {
                    ReqType req;
                    const auto& conv = ReqType::get_converter();

                    if(    conv.template has<json::engine>(json)
                        && conv.template get<json::engine>(json, req))
                    {
                        if(!conv.validate(req))
                        {
                            return tr.send_error("Invalid field content.");
                        }

                        cb(
                                std::forward<Args>(ctx)...,
                                req,
                                std::move(tr));
                    }
                    else
                    {
                        tr.send_error("Invalid json.");
                    }
                }});
    }

private:
    using event_handler_t = void(
            Args... ctx,
            const rapidjson::Value& json,
            transaction tr);

    std::variant<protocol_message, error> parse_protocol_message(
            const boost::asio::const_buffer& buffer,
            size_t bytes_transferred)
    {
        const_stream_adapter bs(buffer, bytes_transferred);
        rapidjson::Document request;
        request.ParseStream(bs);

        if(request.HasParseError())
        {
            return error::parse;
        }

        if(!request.IsObject())
        {
            return error::invalid_json;
        }

        auto c = protocol_message::get_converter();
        protocol_message result;

        if(    c.has<json::engine>(request)
            && c.get<json::engine>(request, result))
        {
            result.data.CopyFrom(request["data"], result.data.GetAllocator());
            return result;
        }

        return error::invalid_json;
    }

    std::map<std::string, std::function<event_handler_t>> event_handlers_;
};

} // namespace proto_utils

} // namespace cis1
