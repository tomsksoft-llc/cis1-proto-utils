#pragma once

#include <istream>
#include <map>
#include <vector>
#include <string>
#include <system_error>

namespace cis1::proto_utils
{

/**
 * \brief Function for reading 'key=value' from istream to map
 * @param[in] is
 * @param[out] lines
 * @param[out] ec
 * @param[in] decode
 */
void read_istream_kv_str(
        std::istream& is,
        std::map<std::string, std::string>& lines,
        std::error_code& ec,
        bool decode = false);

/**
 * \brief Function for reading 'key=value' from istream to ordered vector
 * @param[in] is
 * @param[out] lines
 * @param[out] ec
 */
void read_istream_ordered_kv_str(
        std::istream& is,
        std::vector<std::pair<std::string, std::string>>& lines,
        bool decode = false);

} // namespace cis1::proto_utils
