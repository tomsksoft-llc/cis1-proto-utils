#include "read_istream_kv_str.h"

#include <algorithm>
#include "param_codec.h"

template<typename T>
std::error_code make_error(T err)
{
    return {static_cast<int>(err), std::generic_category()};
}

std::optional<std::pair<std::string, std::string>> read_key_value(
        std::istream& is,
        std::error_code& ec,
        bool decode)
{
    std::string key;
    std::string val;

    std::string line;
    std::getline(is, line, '\n');

    if(line.empty())
    {
        // is not an error, just an empty line
        return std::nullopt;
    }

    std::size_t pos = 0;
    for(; pos < line.size(); ++pos)
    {
        if(line[pos] == '\\')
        {
            // escape the next character
            ++pos;
        }
        else if(line[pos] == '=')
        {
            break;
        }
    }

    if(pos == 0 || pos >= line.size())
    {
        // key is empty or couldn't find the '=' delimiter
        ec = make_error(std::errc::io_error);
        return std::nullopt;
    }

    key = line.substr(0, pos);
    val = line.substr(pos + 1);

    val.erase(
            std::find_if(
                    val.rbegin(),
                    val.rend(),
                    [](int ch)
                    {
                        return !std::isspace(ch);
                    }).base(),
            val.end());

    if(!decode)
    {
        return std::make_pair(key, val);
    }

    std::string decoded_key;
    std::string decoded_value;

    if(!cis1::proto_utils::decode_param(key, decoded_key)
       || !cis1::proto_utils::decode_param(val, decoded_value))
    {
        ec = make_error(std::errc::io_error);
        return std::nullopt;
    }

    return std::make_pair(decoded_key, decoded_value);
}

namespace cis1::proto_utils
{

void read_istream_kv_str(
        std::istream& is,
        std::map<std::string, std::string>& lines,
        std::error_code& ec,
        bool decode /*=false*/)
{
    while(!is.eof())
    {
        const auto key_val = read_key_value(is, ec, decode);
        if(ec)
        {
            return;
        }

        if(key_val)
        {
            const auto &[key, val] = key_val.value();
            lines[key] = val;
        }
    }
}

void read_istream_ordered_kv_str(
        std::istream& is,
        std::vector<std::pair<std::string, std::string>>& lines,
        std::error_code& ec,
        bool decode /*=false*/)
{
    while(!is.eof())
    {
        const auto key_val = read_key_value(is, ec, decode);
        if(ec)
        {
            return;
        }

        if(key_val)
        {
            lines.push_back(key_val.value());
        }
    }
}

} // namespace cis1::proto_utils
