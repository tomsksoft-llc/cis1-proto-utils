#include <cis1_proto_utils/transaction.h>

#include <rapidjson/writer.h>

#include <boost/asio/post.hpp>

#include <cis1_proto_utils/protocol_message.h>

namespace cis1
{

namespace proto_utils
{

const std::string transaction::default_error_ = "system.error";

transaction::transaction(
        const std::weak_ptr<queue_interface>& queue,
        uint64_t transaction_id)
    : transaction_id_(transaction_id)
    , queue_(queue)
{}

transaction::transaction(const transaction& tr)
    : transaction_id_(tr.transaction_id_)
    , queue_(tr.queue_)
{}

void transaction::send_error(const std::string& err) const
{
    if(auto queue = queue_.lock(); queue)
    {
        rapidjson::Document d;

        prepare_response(d, default_error_);

        d.AddMember(
                rapidjson::Value().SetString("errorMessage"),
                rapidjson::Value().SetString(
                        err.c_str(),
                        err.length(),
                        d.GetAllocator()),
                d.GetAllocator());

        auto buffer = std::make_shared<rapidjson::StringBuffer>();
        rapidjson::Writer<rapidjson::StringBuffer> writer(*buffer);
        d.Accept(writer);

        boost::asio::post(
                queue->get_executor(),
                [queue, buffer = std::move(buffer)]()
                {
                    queue->send_text(
                    boost::asio::const_buffer(
                            buffer->GetString(),
                            buffer->GetSize()),
                    [buffer](){});
                });
    }
}

std::optional<boost::asio::executor> transaction::get_executor() const
{
    if(auto queue = queue_.lock(); queue)
    {
        return queue->get_executor();
    }

    return std::nullopt;
}

std::weak_ptr<queue_interface> transaction::get_queue() const
{
    return queue_;
}

void transaction::prepare_response(rapidjson::Document& doc, std::string event) const
{
    doc.SetObject();

    protocol_message msg;
    msg.event = event;
    msg.transaction_id = transaction_id_;

    msg.get_converter().set<json::engine>(doc, msg, doc.GetAllocator());
}

void transaction::send(
        const std::shared_ptr<queue_interface>& queue,
        const rapidjson::Value& v) const
{
    auto buffer = std::make_shared<rapidjson::StringBuffer>();
    rapidjson::Writer<rapidjson::StringBuffer> writer(*buffer);
    v.Accept(writer);

    boost::asio::post(
            queue->get_executor(),
            [queue, buffer = std::move(buffer)]()
            {
                queue->send_text(
                boost::asio::const_buffer(
                        buffer->GetString(),
                        buffer->GetSize()),
                [buffer](){});
            });
}

}

}
