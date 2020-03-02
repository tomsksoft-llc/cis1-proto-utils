#include <cis1_proto_utils/param_codec.h>

#include <sstream>
#include <iomanip>

namespace cis1::proto_utils
{

std::string encode_param(std::string_view src)
{
    std::string result;
    result.reserve(src.size() * 3);
    for(const char ch : src)
    {
        if(ch == '\\') result.append(R"(\\)");
        else if(ch == '=') result.append(R"(\=)");
        else if(ch == '\n') result.append(R"(\n)");
        else result.push_back(ch);
    }

    return result;
}

bool decode_param(std::string_view src, std::string& dst)
{
    dst.clear();
    dst.reserve(src.length());

    std::size_t prev_pos = 0;
    std::size_t pos = 0;
    do
    {
        prev_pos = pos;

        pos = src.find('\\', pos);
        if(pos == std::string::npos)
        {
            break;
        }

        if(pos + 1 >= src.length())
        {
            return false;
        }

        dst.append(src, prev_pos, pos - prev_pos);

        const auto encrypted_ch = src[pos + 1];

        if(encrypted_ch == '\\') dst.push_back('\\');
        else if(encrypted_ch == '=') dst.push_back('=');
        else if(encrypted_ch == 'n') dst.push_back('\n');
        else
        {
            // '\', '=' or 'n' should followed after the '\'
            return false;
        }

        pos += 2;
    } while(pos != std::string::npos);

    dst.append(src, prev_pos);
    return true;
}

} // namespace cis1::proto_utils
