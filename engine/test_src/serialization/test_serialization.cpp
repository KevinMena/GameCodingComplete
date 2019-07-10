
#include <memory>
#include <string>

#include "gtest/gtest.h"

#include "serialization/json_serializer.hpp"
#include "serialization/serializer.hpp"


template <typename Serializer> class SerializerTest : public ::testing::Test {
protected:
  typename Serializer::ISerializer *GetSerializer() {
    return dynamic_cast<typename Serializer::ISerializer *>(new Serializer());
  }
};

using serializermethods = ::testing::Types<Serializer::JsonSerializer>;

TYPED_TEST_SUITE(SerializerTest, serializermethods);

TYPED_TEST(SerializerTest, intType) {
  std::unique_ptr<Serializer::ISerializer> serializer(this->GetSerializer());

  const int hola = 42;
  const std::string hola_name = "hola";
  int hola_res;
  std::string test_start_text = "Expected \"" + hola_name + "\" ";

  EXPECT_TRUE(serializer->SetInt(hola_name.c_str(), hola_name.length(), hola))
      << test_start_text << "to be inserted";
  EXPECT_TRUE(serializer->IsInt(hola_name.c_str()))
      << test_start_text << "to be an int";
  EXPECT_TRUE(serializer->GetInt(hola_name.c_str(), &hola_res))
      << test_start_text << "to be an int";
  EXPECT_EQ(hola_res, hola)
      << test_start_text << "to be " << hola << " but rather got " << hola_res;

  // Pretty Compile
  EXPECT_TRUE(serializer->CompilePretty()) << "Failed to Pretty Compile";

  Serializer::s_size size;

  EXPECT_TRUE(serializer->GetSize(&size))
      << "Failed to Get Size of Pretty Compile";

  std::unique_ptr<char[]> resp(new char[size]);

  EXPECT_TRUE(serializer->GetText(resp.get()))
      << "Failed to Get Pretty Compile";

  std::unique_ptr<Serializer::ISerializer> serializerPretty(
      this->GetSerializer());

  std::string prettyCompile(resp.get());

  hola_res = -1;

  EXPECT_TRUE(serializerPretty->ParseText(resp.get()))
      << "Failed to Parse Pretty Compile\n"
      << prettyCompile;
  EXPECT_TRUE(serializerPretty->GetInt(hola_name.c_str(), &hola_res))
      << test_start_text << "to be an int in Pretty Compile";
  EXPECT_EQ(hola_res, hola) << test_start_text << "in Pretty Compile to be "
                            << hola << " but rather got " << hola_res;

  // Compile
  EXPECT_TRUE(serializer->Compile()) << "Failed to Compile";

  EXPECT_TRUE(serializer->GetSize(&size)) << "Failed to Get Size of Compile";

  std::unique_ptr<char[]> resp2(new char[size]);

  EXPECT_TRUE(serializer->GetText(resp2.get())) << "Failed to Get Compile";

  std::unique_ptr<Serializer::ISerializer> serializerNormal(
      this->GetSerializer());

  std::string compile(resp2.get());

  hola_res = -1;

  EXPECT_TRUE(serializerNormal->ParseText(resp2.get()))
      << "Failed to Parse Compile" << compile;
  EXPECT_TRUE(serializerNormal->GetInt(hola_name.c_str(), &hola_res))
      << test_start_text << "to be an int in Compile";
  EXPECT_EQ(hola_res, hola) << test_start_text << "in Compile to be " << hola
                            << " but rather got " << hola_res;
}