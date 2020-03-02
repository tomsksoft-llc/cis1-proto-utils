#include <gtest/gtest.h>
#include <cis1_proto_utils/param_codec.h>

TEST(param_codec, encode_param)
{
    ASSERT_EQ(R"(test\=param\nvalue\\)",
              cis1::proto_utils::encode_param("test=param\nvalue\\"));
    ASSERT_EQ(R"(test\\\\param\=\nnumber)",
              cis1::proto_utils::encode_param("test\\\\param=\nnumber"));
    ASSERT_EQ("test param value",
              cis1::proto_utils::encode_param("test param value"));
    ASSERT_EQ("",
              cis1::proto_utils::encode_param(""));
}

TEST(param_codec, decode_param_correct)
{
    std::string dst;
    ASSERT_TRUE(cis1::proto_utils::decode_param(
            R"(test\=param\nvalue\\)",
            dst));
    ASSERT_EQ("test=param\nvalue\\", dst);

    //////////

    ASSERT_TRUE(cis1::proto_utils::decode_param(
            R"(test\\\\param\=\nnumber)",
            dst));
    ASSERT_EQ("test\\\\param=\nnumber", dst);

    //////////

    ASSERT_TRUE(cis1::proto_utils::decode_param(
            "test param value",
            dst));
    ASSERT_EQ("test param value", dst);

    //////////

    ASSERT_TRUE(cis1::proto_utils::decode_param(
            "",
            dst));
    ASSERT_EQ("", dst);
}

TEST(param_codec, decode_param_fail)
{
    std::string dst;
    ASSERT_FALSE(cis1::proto_utils::decode_param(
            // '\', '=' or 'n' should followed after the '\'
            R"(test\param)",
            dst));
    ASSERT_FALSE(cis1::proto_utils::decode_param(
            // '\', '=' or 'n' should followed after the '\'
            R"(test param\)",
            dst));
}
