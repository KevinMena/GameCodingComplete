
#include <memory>
#include <string>

#include "gtest/gtest.h"

#include "serialization/serializer.hpp"
#include "serialization/json_serializer.hpp"

template <typename Serializer>
class SerializerTest : public ::testing::Test
{
protected: 
    typename Serializer::ISerializer* GetSerializer() {
        return dynamic_cast<typename Serializer::ISerializer *>(new Serializer());
    }

    
};

using serializermethods = ::testing::Types<Serializer::JsonSerializer>;

TYPED_TEST_SUITE(SerializerTest, serializermethods);


TYPED_TEST(SerializerTest, basictypes)
{
    std::unique_ptr<Serializer::ISerializer> serializer ( this->GetSerializer() );

    const int hola = 42;
    const std::string hola_name = "hola";
    int hola_res;
    std::string test_start_text = "Expected \"" + hola_name + "\" ";

    EXPECT_TRUE(serializer->SetInt(hola_name.c_str(), hola_name.length(), hola)) << test_start_text << "to be inserted"; 
    EXPECT_TRUE(serializer->IsInt(hola_name.c_str())) << test_start_text << "to be an int";
    EXPECT_TRUE(serializer->GetInt(hola_name.c_str(), &hola_res)) <<  test_start_text << "to be an int";
    EXPECT_EQ(hola_res, hola) <<  test_start_text << "to be " << hola << " but rather got " << hola_res;
}

