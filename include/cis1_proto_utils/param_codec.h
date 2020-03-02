#pragma once

#include <string>

namespace cis1::proto_utils
{

/**
 * Converts the given input string to an URL encoded string.
 * @param src - input string.
 * @return - URL encoded string.
 */
std::string encode_param(std::string_view src);

/**
 * Decode the given input string from an URL encoded string.
 * @param src - input string.
 * @param src - output string.
 * @return - true if the given input string has been parsed correct
 *           else false.
 */
bool decode_param(std::string_view src, std::string& dst);

} // namespace cis1::proto_utils
