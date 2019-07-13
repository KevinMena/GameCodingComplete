
#include <algorithm>
#include <cmath>
#include <limits>
#include <memory>
#include <random>
#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "serialization/json_serializer.hpp"
#include "serialization/serializer.hpp"

// https://stackoverflow.com/questions/55892577/how-to-test-the-same-behaviour-for-multiple-templated-classes-with-different-tem
template <typename Serializer> class SerializerTest : public ::testing::Test {
protected:
  typename Serializer::ISerializer *GetSerializer() {
    return dynamic_cast<typename Serializer::ISerializer *>(new Serializer());
  }
};

// Register here other serializers
using serializermethods = ::testing::Types<Serializer::JsonSerializer>;

TYPED_TEST_SUITE(SerializerTest, serializermethods);

TYPED_TEST(SerializerTest, intType) {
  std::unique_ptr<Serializer::ISerializer> serializer(this->GetSerializer());

  const int hola = -42;
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

  EXPECT_TRUE(
      serializerPretty->ParseText(resp.get(), static_cast<size_t>(size)))
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

TYPED_TEST(SerializerTest, uintType) {
  std::unique_ptr<Serializer::ISerializer> serializer(this->GetSerializer());

  const unsigned int hola = 42;
  const std::string hola_name = "hola";
  unsigned int hola_res;
  std::string test_start_text = "Expected \"" + hola_name + "\" ";

  EXPECT_TRUE(serializer->SetUint(hola_name.c_str(), hola_name.length(), hola))
      << test_start_text << "to be inserted";
  EXPECT_TRUE(serializer->IsUint(hola_name.c_str()))
      << test_start_text << "to be an Uint";
  EXPECT_TRUE(serializer->GetUint(hola_name.c_str(), &hola_res))
      << test_start_text << "to be an Uint";
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

  hola_res = 0;

  EXPECT_TRUE(serializerPretty->ParseText(resp.get()))
      << "Failed to Parse Pretty Compile\n"
      << prettyCompile;
  EXPECT_TRUE(serializerPretty->GetUint(hola_name.c_str(), &hola_res))
      << test_start_text << "to be an Uint in Pretty Compile";
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

  hola_res = 0;

  EXPECT_TRUE(serializerNormal->ParseText(resp2.get()))
      << "Failed to Parse Compile" << compile;
  EXPECT_TRUE(serializerNormal->GetUint(hola_name.c_str(), &hola_res))
      << test_start_text << "to be an Uint in Compile";
  EXPECT_EQ(hola_res, hola) << test_start_text << "in Compile to be " << hola
                            << " but rather got " << hola_res;
}

TYPED_TEST(SerializerTest, boolType) {
  std::unique_ptr<Serializer::ISerializer> serializer(this->GetSerializer());

  const bool hola = true;
  const std::string hola_name = "hola";
  bool hola_res;
  std::string test_start_text = "Expected \"" + hola_name + "\" ";

  EXPECT_TRUE(serializer->SetBool(hola_name.c_str(), hola_name.length(), hola))
      << test_start_text << "to be inserted";
  EXPECT_TRUE(serializer->IsBool(hola_name.c_str()))
      << test_start_text << "to be an Bool";
  EXPECT_TRUE(serializer->GetBool(hola_name.c_str(), &hola_res))
      << test_start_text << "to be an Bool";
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

  hola_res = !hola;

  EXPECT_TRUE(serializerPretty->ParseText(resp.get()))
      << "Failed to Parse Pretty Compile\n"
      << prettyCompile;
  EXPECT_TRUE(serializerPretty->GetBool(hola_name.c_str(), &hola_res))
      << test_start_text << "to be an Bool in Pretty Compile";
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

  hola_res = !hola;

  EXPECT_TRUE(serializerNormal->ParseText(resp2.get()))
      << "Failed to Parse Compile" << compile;
  EXPECT_TRUE(serializerNormal->GetBool(hola_name.c_str(), &hola_res))
      << test_start_text << "to be an Bool in Compile";
  EXPECT_EQ(hola_res, hola) << test_start_text << "in Compile to be " << hola
                            << " but rather got " << hola_res;
}

TYPED_TEST(SerializerTest, uint64Type) {
  std::unique_ptr<Serializer::ISerializer> serializer(this->GetSerializer());

  const uint64_t hola = 1ull << 50;
  const std::string hola_name = "hola";
  uint64_t hola_res;
  std::string test_start_text = "Expected \"" + hola_name + "\" ";

  EXPECT_TRUE(
      serializer->SetUint64(hola_name.c_str(), hola_name.length(), hola))
      << test_start_text << "to be inserted";
  EXPECT_TRUE(serializer->IsUint64(hola_name.c_str()))
      << test_start_text << "to be an Uint64";
  EXPECT_TRUE(serializer->GetUint64(hola_name.c_str(), &hola_res))
      << test_start_text << "to be an Uint64";
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

  hola_res = 0;

  EXPECT_TRUE(serializerPretty->ParseText(resp.get()))
      << "Failed to Parse Pretty Compile\n"
      << prettyCompile;
  EXPECT_TRUE(serializerPretty->GetUint64(hola_name.c_str(), &hola_res))
      << test_start_text << "to be an Uint64 in Pretty Compile";
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

  hola_res = 0;

  EXPECT_TRUE(serializerNormal->ParseText(resp2.get()))
      << "Failed to Parse Compile" << compile;
  EXPECT_TRUE(serializerNormal->GetUint64(hola_name.c_str(), &hola_res))
      << test_start_text << "to be an Uint64 in Compile";
  EXPECT_EQ(hola_res, hola) << test_start_text << "in Compile to be " << hola
                            << " but rather got " << hola_res;
}

TYPED_TEST(SerializerTest, int64Type) {
  std::unique_ptr<Serializer::ISerializer> serializer(this->GetSerializer());

  const int64_t hola = (1ll << 50) * -1ll;
  const std::string hola_name = "hola";
  int64_t hola_res;
  std::string test_start_text = "Expected \"" + hola_name + "\" ";

  EXPECT_TRUE(serializer->SetInt64(hola_name.c_str(), hola_name.length(), hola))
      << test_start_text << "to be inserted";
  EXPECT_TRUE(serializer->IsInt64(hola_name.c_str()))
      << test_start_text << "to be an Int64";
  EXPECT_TRUE(serializer->GetInt64(hola_name.c_str(), &hola_res))
      << test_start_text << "to be an Int64";
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

  hola_res = 0;

  EXPECT_TRUE(serializerPretty->ParseText(resp.get()))
      << "Failed to Parse Pretty Compile\n"
      << prettyCompile;
  EXPECT_TRUE(serializerPretty->GetInt64(hola_name.c_str(), &hola_res))
      << test_start_text << "to be an Int64 in Pretty Compile";
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

  hola_res = 0;

  EXPECT_TRUE(serializerNormal->ParseText(resp2.get()))
      << "Failed to Parse Compile" << compile;
  EXPECT_TRUE(serializerNormal->GetInt64(hola_name.c_str(), &hola_res))
      << test_start_text << "to be an Int64 in Compile";
  EXPECT_EQ(hola_res, hola) << test_start_text << "in Compile to be " << hola
                            << " but rather got " << hola_res;
}

TYPED_TEST(SerializerTest, doubleType) {
  std::unique_ptr<Serializer::ISerializer> serializer(this->GetSerializer());

  const double hola = 3.14159265358979323846;
  const std::string hola_name = "hola";
  double hola_res;
  std::string test_start_text = "Expected \"" + hola_name + "\" ";

  EXPECT_TRUE(
      serializer->SetDouble(hola_name.c_str(), hola_name.length(), hola))
      << test_start_text << "to be inserted";
  EXPECT_TRUE(serializer->IsDouble(hola_name.c_str()))
      << test_start_text << "to be an Double";
  EXPECT_TRUE(serializer->GetDouble(hola_name.c_str(), &hola_res))
      << test_start_text << "to be an Double";
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

  hola_res = 0;

  EXPECT_TRUE(serializerPretty->ParseText(resp.get()))
      << "Failed to Parse Pretty Compile\n"
      << prettyCompile;
  EXPECT_TRUE(serializerPretty->GetDouble(hola_name.c_str(), &hola_res))
      << test_start_text << "to be an Double in Pretty Compile";
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

  hola_res = 0;

  EXPECT_TRUE(serializerNormal->ParseText(resp2.get()))
      << "Failed to Parse Compile" << compile;
  EXPECT_TRUE(serializerNormal->GetDouble(hola_name.c_str(), &hola_res))
      << test_start_text << "to be an Double in Compile";
  EXPECT_EQ(hola_res, hola) << test_start_text << "in Compile to be " << hola
                            << " but rather got " << hola_res;
}

TYPED_TEST(SerializerTest, stringType) {
  std::unique_ptr<Serializer::ISerializer> serializer(this->GetSerializer());

  const std::string hola = "sdfsd fsdfsdf sdfsssss  ssdd sdfssSSS SdfSDFf "
                           "FFFSF sdSSSSSS,,,,;;;\"sdfsdf\",s,s ,s,s";
  const std::string hola_name = "hola";
  std::string test_start_text = "Expected \"" + hola_name + "\" ";

  EXPECT_TRUE(serializer->SetString(hola_name.c_str(), hola_name.length(),
                                    hola.c_str(), hola.length()))
      << test_start_text << "to be inserted";
  EXPECT_TRUE(serializer->IsString(hola_name.c_str()))
      << test_start_text << "to be an String";

  Serializer::s_size size;

  EXPECT_TRUE(serializer->GetStringLength(hola_name.c_str(), &size))
      << test_start_text << "String length";

  std::unique_ptr<char[]> hola_res(new char[size]);
  EXPECT_TRUE(serializer->GetString(hola_name.c_str(), hola_res.get()))
      << test_start_text << "to be an String";
  EXPECT_STREQ(hola_res.get(), hola.c_str())
      << test_start_text << "to be " << hola << " but rather got "
      << std::string(hola_res.get());

  // Pretty Compile
  EXPECT_TRUE(serializer->CompilePretty()) << "Failed to Pretty Compile";

  EXPECT_TRUE(serializer->GetSize(&size))
      << "Failed to Get Size of Pretty Compile";

  std::unique_ptr<char[]> resp(new char[size]);

  EXPECT_TRUE(serializer->GetText(resp.get()))
      << "Failed to Get Pretty Compile";

  std::unique_ptr<Serializer::ISerializer> serializerPretty(
      this->GetSerializer());

  std::string prettyCompile(resp.get());

  EXPECT_TRUE(serializerPretty->ParseText(resp.get()))
      << "Failed to Parse Pretty Compile\n"
      << prettyCompile;

  EXPECT_TRUE(serializerPretty->GetStringLength(hola_name.c_str(), &size))
      << test_start_text << "String length";

  std::unique_ptr<char[]> hola_res2(new char[size]);
  EXPECT_TRUE(serializerPretty->GetString(hola_name.c_str(), hola_res2.get()))
      << test_start_text << "to be an String in Pretty Compile";
  EXPECT_STREQ(hola_res2.get(), hola.c_str())
      << test_start_text << "in Pretty Compile to be " << hola
      << " but rather got " << std::string(hola_res2.get());

  // Compile
  EXPECT_TRUE(serializer->Compile()) << "Failed to Compile";

  EXPECT_TRUE(serializer->GetSize(&size)) << "Failed to Get Size of Compile";

  std::unique_ptr<char[]> resp2(new char[size]);

  EXPECT_TRUE(serializer->GetText(resp2.get())) << "Failed to Get Compile";

  std::unique_ptr<Serializer::ISerializer> serializerNormal(
      this->GetSerializer());

  std::string compile(resp2.get());

  EXPECT_TRUE(serializerNormal->ParseText(resp2.get()))
      << "Failed to Parse Compile" << compile;
  EXPECT_TRUE(serializerNormal->GetStringLength(hola_name.c_str(), &size))
      << test_start_text << "String length";

  std::unique_ptr<char[]> hola_res3(new char[size]);
  EXPECT_TRUE(serializerNormal->GetString(hola_name.c_str(), hola_res3.get()))
      << test_start_text << "to be an String in Compile";
  EXPECT_STREQ(hola_res3.get(), hola.c_str())
      << test_start_text << "in Compile to be " << hola << " but rather got "
      << std::string(hola_res3.get());
}

TYPED_TEST(SerializerTest, nullType) {
  std::unique_ptr<Serializer::ISerializer> serializer(this->GetSerializer());

  const std::string hola_name = "hola";
  std::string test_start_text = "Expected \"" + hola_name + "\" ";

  EXPECT_TRUE(serializer->SetNull(hola_name.c_str(), hola_name.length()))
      << test_start_text << "to be inserted";
  EXPECT_TRUE(serializer->IsNull(hola_name.c_str()))
      << test_start_text << "to be a nul";

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

  EXPECT_TRUE(
      serializerPretty->ParseText(resp.get(), static_cast<size_t>(size)))
      << "Failed to Parse Pretty Compile\n"
      << prettyCompile;
  EXPECT_TRUE(serializerPretty->IsNull(hola_name.c_str()))
      << test_start_text << "to be a null in Pretty Compile";

  // Compile
  EXPECT_TRUE(serializer->Compile()) << "Failed to Compile";

  EXPECT_TRUE(serializer->GetSize(&size)) << "Failed to Get Size of Compile";

  std::unique_ptr<char[]> resp2(new char[size]);

  EXPECT_TRUE(serializer->GetText(resp2.get())) << "Failed to Get Compile";

  std::unique_ptr<Serializer::ISerializer> serializerNormal(
      this->GetSerializer());

  std::string compile(resp2.get());

  EXPECT_TRUE(serializerNormal->ParseText(resp2.get()))
      << "Failed to Parse Compile" << compile;
  EXPECT_TRUE(serializerNormal->IsNull(hola_name.c_str()))
      << test_start_text << "to be a null in Compile";
}

TYPED_TEST(SerializerTest, intArrayType) {
  std::unique_ptr<Serializer::ISerializer> serializer(this->GetSerializer());

  const std::vector<int> hola{42, -42, 0, 1 << 30, -1 * (1 << 30)};
  std::vector<int> hola_resp;
  hola_resp.reserve(hola.size());
  const std::string hola_name = "hola";
  int hola_res;
  std::string test_start_text = "Expected \"" + hola_name + "\" ";

  EXPECT_TRUE(serializer->SetArray(hola_name.c_str(), hola_name.length()));

  EXPECT_TRUE(serializer->SetInt(nullptr, 0, hola[0]));
  EXPECT_TRUE(serializer->IsArray(hola_name.c_str()));

  for (size_t i = 1; i < hola.size(); i++) {
    EXPECT_TRUE(serializer->SetInt(nullptr, 0, hola[i]));
  }

  EXPECT_TRUE(serializer->CloseArray());

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

  EXPECT_TRUE(serializerPretty->ParseText(resp.get()))
      << "Failed to Parse Pretty Compile\n"
      << prettyCompile;

  Serializer::s_size array_size;

  EXPECT_TRUE(serializerPretty->IsArray(hola_name.c_str())) << prettyCompile;
  EXPECT_TRUE(
      serializerPretty->GetArrayCapacity(hola_name.c_str(), &array_size));
  ASSERT_EQ(array_size, hola.size()) << "Arrays of Different Size";
  EXPECT_TRUE(serializerPretty->OpenArray(hola_name.c_str()));

  for (size_t i = 0; i < array_size; i++) {
    EXPECT_TRUE(serializerPretty->GetInt(hola_name.c_str(), &hola_res))
        << test_start_text << "to be an int in Pretty Compile";

    hola_resp.push_back(hola_res);
    EXPECT_TRUE(serializerPretty->MoveArray() || i == array_size - 1);
  }
  EXPECT_TRUE(hola == hola_resp) << " Different Arrays in Pretty Compile\n"
                                 << prettyCompile;

  EXPECT_TRUE(serializerPretty->CloseArray());

  // Compile
  EXPECT_TRUE(serializer->Compile()) << "Failed to Compile";

  EXPECT_TRUE(serializer->GetSize(&size)) << "Failed to Get Size of Compile";

  std::unique_ptr<char[]> resp2(new char[size]);

  EXPECT_TRUE(serializer->GetText(resp2.get())) << "Failed to Get Compile";

  std::unique_ptr<Serializer::ISerializer> serializerNormal(
      this->GetSerializer());

  std::string compile(resp2.get());

  EXPECT_TRUE(serializerNormal->ParseText(resp2.get()))
      << "Failed to Parse Compile\n"
      << compile;

  EXPECT_TRUE(
      serializerNormal->GetArrayCapacity(hola_name.c_str(), &array_size));
  ASSERT_EQ(array_size, hola.size()) << "Arrays of Different Size";
  EXPECT_TRUE(serializerNormal->OpenArray(hola_name.c_str()));
  hola_resp.clear();
  for (size_t i = 0; i < array_size; i++) {
    EXPECT_TRUE(serializerNormal->GetInt(hola_name.c_str(), &hola_res))
        << test_start_text << "to be an int in Pretty Compile";
    hola_resp.push_back(hola_res);
    EXPECT_TRUE(serializerNormal->MoveArray() || i == array_size - 1);
  }
  EXPECT_TRUE(hola == hola_resp) << " Different Arrays in Compile\n"
                                 << prettyCompile;

  EXPECT_TRUE(serializerNormal->CloseArray());
}

TYPED_TEST(SerializerTest, uintArrayType) {
  std::unique_ptr<Serializer::ISerializer> serializer(this->GetSerializer());

  const std::vector<unsigned int> hola{42, 7, 0, 1 << 30, (1u << 31)};
  std::vector<unsigned int> hola_resp;
  hola_resp.reserve(hola.size());
  const std::string hola_name = "hola";
  unsigned int hola_res;
  std::string test_start_text = "Expected \"" + hola_name + "\" ";

  EXPECT_TRUE(serializer->SetArray(hola_name.c_str(), hola_name.length()));

  EXPECT_TRUE(serializer->SetUint(nullptr, 0, hola[0]));
  EXPECT_TRUE(serializer->IsArray(hola_name.c_str()));

  for (size_t i = 1; i < hola.size(); i++) {
    EXPECT_TRUE(serializer->SetUint(nullptr, 0, hola[i]));
  }

  EXPECT_TRUE(serializer->CloseArray());

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

  EXPECT_TRUE(serializerPretty->ParseText(resp.get()))
      << "Failed to Parse Pretty Compile\n"
      << prettyCompile;

  Serializer::s_size array_size;

  EXPECT_TRUE(serializerPretty->IsArray(hola_name.c_str())) << prettyCompile;
  EXPECT_TRUE(
      serializerPretty->GetArrayCapacity(hola_name.c_str(), &array_size));
  ASSERT_EQ(array_size, hola.size()) << "Arrays of Different Size";
  EXPECT_TRUE(serializerPretty->OpenArray(hola_name.c_str()));

  for (size_t i = 0; i < array_size; i++) {
    EXPECT_TRUE(serializerPretty->GetUint(hola_name.c_str(), &hola_res))
        << test_start_text << "to be an Uint in Pretty Compile";

    hola_resp.push_back(hola_res);
    EXPECT_TRUE(serializerPretty->MoveArray() || i == array_size - 1);
  }
  EXPECT_TRUE(hola == hola_resp) << " Different Arrays in Pretty Compile\n"
                                 << prettyCompile;

  EXPECT_TRUE(serializerPretty->CloseArray());

  // Compile
  EXPECT_TRUE(serializer->Compile()) << "Failed to Compile";

  EXPECT_TRUE(serializer->GetSize(&size)) << "Failed to Get Size of Compile";

  std::unique_ptr<char[]> resp2(new char[size]);

  EXPECT_TRUE(serializer->GetText(resp2.get())) << "Failed to Get Compile";

  std::unique_ptr<Serializer::ISerializer> serializerNormal(
      this->GetSerializer());

  std::string compile(resp2.get());

  EXPECT_TRUE(serializerNormal->ParseText(resp2.get()))
      << "Failed to Parse Compile\n"
      << compile;

  EXPECT_TRUE(
      serializerNormal->GetArrayCapacity(hola_name.c_str(), &array_size));
  ASSERT_EQ(array_size, hola.size()) << "Arrays of Different Size";
  EXPECT_TRUE(serializerNormal->OpenArray(hola_name.c_str()));
  hola_resp.clear();
  for (size_t i = 0; i < array_size; i++) {
    EXPECT_TRUE(serializerNormal->GetUint(hola_name.c_str(), &hola_res))
        << test_start_text << "to be an int in Pretty Compile";
    hola_resp.push_back(hola_res);
    EXPECT_TRUE(serializerNormal->MoveArray() || i == array_size - 1);
  }
  EXPECT_TRUE(hola == hola_resp) << " Different Arrays in Compile\n"
                                 << prettyCompile;

  EXPECT_TRUE(serializerNormal->CloseArray());
}

TYPED_TEST(SerializerTest, int64ArrayType) {
  std::unique_ptr<Serializer::ISerializer> serializer(this->GetSerializer());

  const std::vector<int64_t> hola{42,      7,           0,
                                  1 << 30, (1ll << 50), -1 * (1ll << 50)};
  std::vector<int64_t> hola_resp;
  hola_resp.reserve(hola.size());
  const std::string hola_name = "hola";
  int64_t hola_res;
  std::string test_start_text = "Expected \"" + hola_name + "\" ";

  EXPECT_TRUE(serializer->SetArray(hola_name.c_str(), hola_name.length()));

  EXPECT_TRUE(serializer->SetInt64(nullptr, 0, hola[0]));
  EXPECT_TRUE(serializer->IsArray(hola_name.c_str()));

  for (size_t i = 1; i < hola.size(); i++) {
    EXPECT_TRUE(serializer->SetInt64(nullptr, 0, hola[i]));
  }

  EXPECT_TRUE(serializer->CloseArray());

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

  EXPECT_TRUE(serializerPretty->ParseText(resp.get()))
      << "Failed to Parse Pretty Compile\n"
      << prettyCompile;

  Serializer::s_size array_size;

  EXPECT_TRUE(serializerPretty->IsArray(hola_name.c_str())) << prettyCompile;
  EXPECT_TRUE(
      serializerPretty->GetArrayCapacity(hola_name.c_str(), &array_size));
  ASSERT_EQ(array_size, hola.size()) << "Arrays of Different Size";
  EXPECT_TRUE(serializerPretty->OpenArray(hola_name.c_str()));

  for (size_t i = 0; i < array_size; i++) {
    EXPECT_TRUE(serializerPretty->GetInt64(hola_name.c_str(), &hola_res))
        << test_start_text << "to be an Int64 in Pretty Compile";

    hola_resp.push_back(hola_res);
    EXPECT_TRUE(serializerPretty->MoveArray() || i == array_size - 1);
  }
  EXPECT_TRUE(hola == hola_resp) << " Different Arrays in Pretty Compile\n"
                                 << prettyCompile;

  EXPECT_TRUE(serializerPretty->CloseArray());

  // Compile
  EXPECT_TRUE(serializer->Compile()) << "Failed to Compile";

  EXPECT_TRUE(serializer->GetSize(&size)) << "Failed to Get Size of Compile";

  std::unique_ptr<char[]> resp2(new char[size]);

  EXPECT_TRUE(serializer->GetText(resp2.get())) << "Failed to Get Compile";

  std::unique_ptr<Serializer::ISerializer> serializerNormal(
      this->GetSerializer());

  std::string compile(resp2.get());

  EXPECT_TRUE(serializerNormal->ParseText(resp2.get()))
      << "Failed to Parse Compile\n"
      << compile;

  EXPECT_TRUE(
      serializerNormal->GetArrayCapacity(hola_name.c_str(), &array_size));
  ASSERT_EQ(array_size, hola.size()) << "Arrays of Different Size";
  EXPECT_TRUE(serializerNormal->OpenArray(hola_name.c_str()));
  hola_resp.clear();
  for (size_t i = 0; i < array_size; i++) {
    EXPECT_TRUE(serializerNormal->GetInt64(hola_name.c_str(), &hola_res))
        << test_start_text << "to be an int in Pretty Compile";
    hola_resp.push_back(hola_res);
    EXPECT_TRUE(serializerNormal->MoveArray() || i == array_size - 1);
  }
  EXPECT_TRUE(hola == hola_resp) << " Different Arrays in Compile\n"
                                 << prettyCompile;

  EXPECT_TRUE(serializerNormal->CloseArray());
}

TYPED_TEST(SerializerTest, uint64ArrayType) {
  std::unique_ptr<Serializer::ISerializer> serializer(this->GetSerializer());

  const std::vector<uint64_t> hola{42, 7, 0, 1 << 30, (1ll << 50), (1ll << 51)};
  std::vector<uint64_t> hola_resp;
  hola_resp.reserve(hola.size());
  const std::string hola_name = "hola";
  uint64_t hola_res;
  std::string test_start_text = "Expected \"" + hola_name + "\" ";

  EXPECT_TRUE(serializer->SetArray(hola_name.c_str(), hola_name.length()));

  EXPECT_TRUE(serializer->SetUint64(nullptr, 0, hola[0]));
  EXPECT_TRUE(serializer->IsArray(hola_name.c_str()));

  for (size_t i = 1; i < hola.size(); i++) {
    EXPECT_TRUE(serializer->SetUint64(nullptr, 0, hola[i]));
  }

  EXPECT_TRUE(serializer->CloseArray());

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

  EXPECT_TRUE(serializerPretty->ParseText(resp.get()))
      << "Failed to Parse Pretty Compile\n"
      << prettyCompile;

  Serializer::s_size array_size;

  EXPECT_TRUE(serializerPretty->IsArray(hola_name.c_str())) << prettyCompile;
  EXPECT_TRUE(
      serializerPretty->GetArrayCapacity(hola_name.c_str(), &array_size));
  ASSERT_EQ(array_size, hola.size()) << "Arrays of Different Size";
  EXPECT_TRUE(serializerPretty->OpenArray(hola_name.c_str()));

  for (size_t i = 0; i < array_size; i++) {
    EXPECT_TRUE(serializerPretty->GetUint64(hola_name.c_str(), &hola_res))
        << test_start_text << "to be an Uint64 in Pretty Compile";

    hola_resp.push_back(hola_res);
    EXPECT_TRUE(serializerPretty->MoveArray() || i == array_size - 1);
  }
  EXPECT_TRUE(hola == hola_resp) << " Different Arrays in Pretty Compile\n"
                                 << prettyCompile;

  EXPECT_TRUE(serializerPretty->CloseArray());

  // Compile
  EXPECT_TRUE(serializer->Compile()) << "Failed to Compile";

  EXPECT_TRUE(serializer->GetSize(&size)) << "Failed to Get Size of Compile";

  std::unique_ptr<char[]> resp2(new char[size]);

  EXPECT_TRUE(serializer->GetText(resp2.get())) << "Failed to Get Compile";

  std::unique_ptr<Serializer::ISerializer> serializerNormal(
      this->GetSerializer());

  std::string compile(resp2.get());

  EXPECT_TRUE(serializerNormal->ParseText(resp2.get()))
      << "Failed to Parse Compile\n"
      << compile;

  EXPECT_TRUE(
      serializerNormal->GetArrayCapacity(hola_name.c_str(), &array_size));
  ASSERT_EQ(array_size, hola.size()) << "Arrays of Different Size";
  EXPECT_TRUE(serializerNormal->OpenArray(hola_name.c_str()));
  hola_resp.clear();
  for (size_t i = 0; i < array_size; i++) {
    EXPECT_TRUE(serializerNormal->GetUint64(hola_name.c_str(), &hola_res))
        << test_start_text << "to be an int in Pretty Compile";
    hola_resp.push_back(hola_res);
    EXPECT_TRUE(serializerNormal->MoveArray() || i == array_size - 1);
  }
  EXPECT_TRUE(hola == hola_resp) << " Different Arrays in Compile\n"
                                 << prettyCompile;

  EXPECT_TRUE(serializerNormal->CloseArray());
}

TYPED_TEST(SerializerTest, doubleArrayType) {
  std::unique_ptr<Serializer::ISerializer> serializer(this->GetSerializer());

  const std::vector<double> hola{42,
                                 7.5,
                                 0,
                                 3.14159265358979323846,
                                 std::numeric_limits<double>::quiet_NaN(),
                                 std::numeric_limits<double>::infinity(),
                                 std::numeric_limits<double>::epsilon(),
                                 -0.1};
  std::vector<double> hola_resp;
  hola_resp.reserve(hola.size());
  const std::string hola_name = "hola";
  double hola_res;
  std::string test_start_text = "Expected \"" + hola_name + "\" ";

  EXPECT_TRUE(serializer->SetArray(hola_name.c_str(), hola_name.length()));

  EXPECT_TRUE(serializer->SetDouble(nullptr, 0, hola[0]));
  EXPECT_TRUE(serializer->IsArray(hola_name.c_str()));

  for (size_t i = 1; i < hola.size(); i++) {
    EXPECT_TRUE(serializer->SetDouble(nullptr, 0, hola[i]));
  }

  EXPECT_TRUE(serializer->CloseArray());

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

  EXPECT_TRUE(serializerPretty->ParseText(resp.get()))
      << "Failed to Parse Pretty Compile\n"
      << prettyCompile;

  Serializer::s_size array_size;

  EXPECT_TRUE(serializerPretty->IsArray(hola_name.c_str())) << prettyCompile;
  EXPECT_TRUE(
      serializerPretty->GetArrayCapacity(hola_name.c_str(), &array_size));
  ASSERT_EQ(array_size, hola.size()) << "Arrays of Different Size";
  EXPECT_TRUE(serializerPretty->OpenArray(hola_name.c_str()));

  for (size_t i = 0; i < array_size; i++) {
    EXPECT_TRUE(serializerPretty->GetDouble(hola_name.c_str(), &hola_res))
        << test_start_text << "to be an Double in Pretty Compile";

    hola_resp.push_back(hola_res);
    EXPECT_TRUE(serializerPretty->MoveArray() || i == array_size - 1);
  }

  for (size_t i = 0; i < hola.size(); i++) {
    if (std::isnan(hola[i])) {
      EXPECT_TRUE(std::isnan(hola[i]) && std::isnan(hola_resp[i]))
          << " Different Arrays in Pretty Compile\n"
          << prettyCompile;
    } else {
      EXPECT_DOUBLE_EQ(hola[i], hola_resp[i])
          << " Different Arrays in Pretty Compile\n"
          << prettyCompile;
    }
  }

  EXPECT_TRUE(serializerPretty->CloseArray());

  // Compile
  EXPECT_TRUE(serializer->Compile()) << "Failed to Compile";

  EXPECT_TRUE(serializer->GetSize(&size)) << "Failed to Get Size of Compile";

  std::unique_ptr<char[]> resp2(new char[size]);

  EXPECT_TRUE(serializer->GetText(resp2.get())) << "Failed to Get Compile";

  std::unique_ptr<Serializer::ISerializer> serializerNormal(
      this->GetSerializer());

  std::string compile(resp2.get());

  EXPECT_TRUE(serializerNormal->ParseText(resp2.get()))
      << "Failed to Parse Compile\n"
      << compile;

  EXPECT_TRUE(
      serializerNormal->GetArrayCapacity(hola_name.c_str(), &array_size));
  ASSERT_EQ(array_size, hola.size()) << "Arrays of Different Size";
  EXPECT_TRUE(serializerNormal->OpenArray(hola_name.c_str()));
  hola_resp.clear();
  for (size_t i = 0; i < array_size; i++) {
    EXPECT_TRUE(serializerNormal->GetDouble(hola_name.c_str(), &hola_res))
        << test_start_text << "to be an int in Compile";
    hola_resp.push_back(hola_res);
    EXPECT_TRUE(serializerNormal->MoveArray() || i == array_size - 1);
  }

  for (size_t i = 0; i < hola.size(); i++) {
    if (std::isnan(hola[i])) {
      EXPECT_TRUE(std::isnan(hola[i]) && std::isnan(hola_resp[i]))
          << " Different Arrays in Pretty Compile\n"
          << prettyCompile;
    } else {
      EXPECT_DOUBLE_EQ(hola[i], hola_resp[i])
          << " Different Arrays in Pretty Compile\n"
          << prettyCompile;
    }
  }

  EXPECT_TRUE(serializerNormal->CloseArray());
}

TYPED_TEST(SerializerTest, stringArrayType) {
  std::unique_ptr<Serializer::ISerializer> serializer(this->GetSerializer());

  const std::vector<std::string> hola{
      "sdfsd fsdfsdf sdfsssss  ssdd sdfssSSS SdfSDFf ",
      "FFFSF sdSSSSSS,,,,;;;\"sdfsdf\",s,s ,s,s",
      "AAAAAAAaaaAaa\t\t\t\taaaadsasdacc;;;;;;\"\"\"\\",
      "234o233o2o2.2.2.3.42.3423f", "{\"hola\":\"hola\"}"};
  const std::string hola_name = "hola";
  std::string test_start_text = "Expected \"" + hola_name + "\" ";

  EXPECT_TRUE(serializer->SetArray(hola_name.c_str(), hola_name.length()));

  EXPECT_TRUE(
      serializer->SetString(nullptr, 0, hola[0].c_str(), hola[0].size()));
  EXPECT_TRUE(serializer->IsArray(hola_name.c_str()));

  for (size_t i = 1; i < hola.size(); i++) {
    EXPECT_TRUE(
        serializer->SetString(nullptr, 0, hola[i].c_str(), hola[i].size()));
  }

  EXPECT_TRUE(serializer->CloseArray());

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

  EXPECT_TRUE(serializerPretty->ParseText(resp.get()))
      << "Failed to Parse Pretty Compile\n"
      << prettyCompile;

  Serializer::s_size array_size;

  EXPECT_TRUE(serializerPretty->IsArray(hola_name.c_str())) << prettyCompile;
  EXPECT_TRUE(
      serializerPretty->GetArrayCapacity(hola_name.c_str(), &array_size));
  ASSERT_EQ(array_size, hola.size()) << "Arrays of Different Size";
  EXPECT_TRUE(serializerPretty->OpenArray(hola_name.c_str()));

  for (size_t i = 0; i < array_size; i++) {
    Serializer::s_size str_size;

    EXPECT_TRUE(serializerPretty->GetStringSize(nullptr, &str_size))
        << "Failed to Get String Size in Pretty Compile";

    std::unique_ptr<char[]> str_resp(new char[str_size]);

    EXPECT_TRUE(serializerPretty->GetString(nullptr, str_resp.get()))
        << "Failed to Get String in Pretty Compile";

    EXPECT_STREQ(str_resp.get(), hola[i].c_str());

    EXPECT_TRUE(serializerPretty->MoveArray() || i == array_size - 1);
  }

  EXPECT_TRUE(serializerPretty->CloseArray());

  // Compile
  EXPECT_TRUE(serializer->Compile()) << "Failed to Compile";

  EXPECT_TRUE(serializer->GetSize(&size)) << "Failed to Get Size of Compile";

  std::unique_ptr<char[]> resp2(new char[size]);

  EXPECT_TRUE(serializer->GetText(resp2.get())) << "Failed to Get Compile";

  std::unique_ptr<Serializer::ISerializer> serializerNormal(
      this->GetSerializer());

  std::string compile(resp2.get());

  EXPECT_TRUE(serializerNormal->ParseText(resp2.get()))
      << "Failed to Parse Compile\n"
      << compile;

  EXPECT_TRUE(
      serializerNormal->GetArrayCapacity(hola_name.c_str(), &array_size));
  ASSERT_EQ(array_size, hola.size()) << "Arrays of Different Size";
  EXPECT_TRUE(serializerNormal->OpenArray(hola_name.c_str()));
  for (size_t i = 0; i < array_size; i++) {
    Serializer::s_size str_size;

    EXPECT_TRUE(serializerNormal->GetStringSize(nullptr, &str_size))
        << "Failed to Get String Size in Compile";

    std::unique_ptr<char[]> str_resp(new char[str_size]);

    EXPECT_TRUE(serializerNormal->GetString(nullptr, str_resp.get()))
        << "Failed to Get String in Compile";

    EXPECT_STREQ(str_resp.get(), hola[i].c_str());
    EXPECT_TRUE(serializerNormal->MoveArray() || i == array_size - 1);
  }
  EXPECT_TRUE(serializerNormal->CloseArray());
}

TYPED_TEST(SerializerTest, nullArrayType) {
  std::unique_ptr<Serializer::ISerializer> serializer(this->GetSerializer());

  const std::vector<int> hola{0, 0, 0, 0, 0};
  const std::string hola_name = "hola";
  std::string test_start_text = "Expected \"" + hola_name + "\" ";

  EXPECT_TRUE(serializer->SetArray(hola_name.c_str(), hola_name.length()));

  EXPECT_TRUE(serializer->SetNull(nullptr, 0));
  EXPECT_TRUE(serializer->IsArray(hola_name.c_str()));

  for (size_t i = 1; i < hola.size(); i++) {
    EXPECT_TRUE(serializer->SetNull(nullptr, 0));
  }

  EXPECT_TRUE(serializer->CloseArray());

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

  EXPECT_TRUE(serializerPretty->ParseText(resp.get()))
      << "Failed to Parse Pretty Compile\n"
      << prettyCompile;

  Serializer::s_size array_size;

  EXPECT_TRUE(serializerPretty->IsArray(hola_name.c_str())) << prettyCompile;
  EXPECT_TRUE(
      serializerPretty->GetArrayCapacity(hola_name.c_str(), &array_size));
  ASSERT_EQ(array_size, hola.size()) << "Arrays of Different Size";
  EXPECT_TRUE(serializerPretty->OpenArray(hola_name.c_str()));

  for (size_t i = 0; i < array_size; i++) {
    EXPECT_TRUE(serializerPretty->IsNull(nullptr))
        << test_start_text << "to be a null in Pretty Compile";

    EXPECT_TRUE(
        (serializerPretty->CanMoveArray() && serializerPretty->MoveArray()) ||
        (i == array_size - 1 && (!serializerPretty->CanMoveArray() &&
                                 !serializerPretty->MoveArray())));
  }

  EXPECT_TRUE(serializerPretty->CloseArray());

  // Compile
  EXPECT_TRUE(serializer->Compile()) << "Failed to Compile";

  EXPECT_TRUE(serializer->GetSize(&size)) << "Failed to Get Size of Compile";

  std::unique_ptr<char[]> resp2(new char[size]);

  EXPECT_TRUE(serializer->GetText(resp2.get())) << "Failed to Get Compile";

  std::unique_ptr<Serializer::ISerializer> serializerNormal(
      this->GetSerializer());

  std::string compile(resp2.get());

  EXPECT_TRUE(serializerNormal->ParseText(resp2.get()))
      << "Failed to Parse Compile\n"
      << compile;

  EXPECT_TRUE(
      serializerNormal->GetArrayCapacity(hola_name.c_str(), &array_size));
  ASSERT_EQ(array_size, hola.size()) << "Arrays of Different Size";
  EXPECT_TRUE(serializerNormal->OpenArray(hola_name.c_str()));
  for (size_t i = 0; i < array_size; i++) {
    EXPECT_TRUE(serializerNormal->IsNull(nullptr))
        << test_start_text << "to be a null in Compile";
    EXPECT_TRUE(serializerNormal->MoveArray() || i == array_size - 1);
  }
  EXPECT_TRUE(serializerNormal->CloseArray());
}

using RecursiveStruct = struct RecursiveStruct {
  std::string m_someText;
  int m_anInt;
  double m_aDouble;
  bool m_aBool;
  std::vector<uint64_t> m_someUints;

  std::unique_ptr<RecursiveStruct> m_aPtr;

  RecursiveStruct() = default;

  RecursiveStruct(RecursiveStruct *ptr) {
    m_aPtr = std::unique_ptr<RecursiveStruct>(ptr);
  }

  ~RecursiveStruct() = default;

  void SetRandom(const char *text) {
    m_someText.clear();
    m_someText.assign(text);

    auto intGenerator = std::bind(kIntDistribution, kGenerator);
    m_anInt = intGenerator();
    m_aBool = intGenerator() & 1;

    int size = intGenerator() % kMaxVectorSize;
    m_someUints.clear();
    m_someUints.reserve(size);

    auto doubleGenerator = std::bind(kDoubleDistribution, kGenerator);
    m_aDouble = doubleGenerator();

    auto unitGenerator = std::bind(kUintDistribution, kGenerator);
    for (int i = 0; i < size; i++) {
      m_someUints.push_back(unitGenerator());
    }
  }

  bool operator==(const RecursiveStruct &other) {
    if (m_someText != other.m_someText) {
      return false;
    }
    if (m_anInt != other.m_anInt) {
      return false;
    }
    if (m_aDouble != other.m_aDouble) {
      return false;
    }
    if (m_aBool != other.m_aBool) {
      return false;
    }
    if (m_someUints != other.m_someUints) {
      return false;
    }
    if (m_aPtr && other.m_aPtr) {
      return *m_aPtr == *other.m_aPtr;
    }
    return !m_aPtr && !other.m_aPtr;
  }

  bool operator!=(const RecursiveStruct &other) { return !(*this == other); }

  bool Serialize(Serializer::ISerializer *serializer, const char *name,
                 Serializer::s_size name_length) {
    if (!serializer->SetEntry(name, name_length, kSerializerVersion)) {
      return false;
    }

    if (!serializer->SetString(SET_NAME(m_someText), m_someText.c_str(),
                               m_someText.length())) {
      return false;
    }

    if (!serializer->SetInt(SET_NAME(m_anInt), m_anInt)) {
      return false;
    }

    if (!serializer->SetDouble(SET_NAME(m_aDouble), m_aDouble)) {
      return false;
    }

    if (!serializer->SetBool(SET_NAME(m_aBool), m_aBool)) {
      return false;
    }

    if (!serializer->SetArray(SET_NAME(m_someUints))) {
      return false;
    }

    for (auto anUint : m_someUints) {
      if (!serializer->SetUint64(nullptr, 0, anUint)) {
        return false;
      }
    }

    if (!serializer->CloseArray()) {
      return false;
    }

    if (m_aPtr) {
      if (!m_aPtr->Serialize(serializer, SET_NAME(m_aPtr))) {
        return false;
      }

    } else {
      if (!serializer->SetNull(SET_NAME(m_aPtr))) {
        return false;
      }
    }

    if (!serializer->CloseEntry()) {
      return false;
    }

    return true;
  }

  static std::unique_ptr<RecursiveStruct>
  Deserialize(const Serializer::ISerializer *serializer, const char *name) {

    Serializer::s_size version;

    if (!serializer->OpenEntry(name, &version)) {
      return nullptr;
    }

    if (version != kSerializerVersion) {
      return nullptr;
    }

    size_t size;

    if (!serializer->GetStringLength(GET_NAME(m_someText), &size)) {
      return nullptr;
    }

    std::unique_ptr<char[]> someText(new char[size]);

    if (!serializer->GetString(GET_NAME(m_someText), someText.get())) {
      return nullptr;
    }

    std::unique_ptr<RecursiveStruct> object =
        std::make_unique<RecursiveStruct>();

    object->m_someText.clear();

    object->m_someText.assign(someText.get());

    if (!serializer->GetInt(GET_NAME(m_anInt), &(object->m_anInt))) {
      return nullptr;
    }

    if (!serializer->GetDouble(GET_NAME(m_aDouble), &(object->m_aDouble))) {
      return nullptr;
    }

    if (!serializer->GetBool(GET_NAME(m_aBool), &(object->m_aBool))) {
      return nullptr;
    }

    Serializer::s_size m_someUints_size;

    if (!serializer->GetArrayCapacity(GET_NAME(m_someUints),
                                      &m_someUints_size)) {
      return nullptr;
    }

    if (!serializer->OpenArray(GET_NAME(m_someUints))) {
      return nullptr;
    }

    object->m_someUints.clear();
    object->m_someUints.reserve(static_cast<size_t>(m_someUints_size));

    {
      uint64_t tmp;

      for (Serializer::s_size i = 0; i < m_someUints_size; i++) {
        if (!serializer->GetUint64(nullptr, &tmp)) {
          return nullptr;
        }
        object->m_someUints.push_back(tmp);
        serializer->MoveArray();
      }
    }

    if (!serializer->CloseArray()) {
      return nullptr;
    }

    object->m_aPtr.reset();

    if (!serializer->IsNull(GET_NAME(m_aPtr))) {

      object->m_aPtr = std::move(Deserialize(serializer, GET_NAME(m_aPtr)));

      if (!object->m_aPtr) {
        return nullptr;
      }
    }

    if (!serializer->CloseEntry()) {
      return nullptr;
    }

    return object;
  }

private:
  static std::default_random_engine kGenerator;
  static std::uniform_int_distribution<int> kIntDistribution;
  static std::uniform_int_distribution<uint64_t> kUintDistribution;
  static std::uniform_real_distribution<double> kDoubleDistribution;
  static constexpr int kMaxVectorSize = 4;
  static constexpr Serializer::s_size kSerializerVersion = 0;
};

std::default_random_engine RecursiveStruct::kGenerator(42);
std::uniform_int_distribution<int> RecursiveStruct::kIntDistribution;
std::uniform_int_distribution<uint64_t> RecursiveStruct::kUintDistribution;
std::uniform_real_distribution<double> RecursiveStruct::kDoubleDistribution;

TYPED_TEST(SerializerTest, recursiveObject) {
  RecursiveStruct *recTmp = new RecursiveStruct();

  recTmp->SetRandom("LOWESTaljsdlakjsldasd;;;;;;;;\"\n\t");

  recTmp = new RecursiveStruct(recTmp);

  recTmp->SetRandom("\"MIDDLE\"");

  std::unique_ptr<RecursiveStruct> rec =
      std::make_unique<RecursiveStruct>(recTmp);
  recTmp = nullptr;
  rec->SetRandom("\"HIGHEST\"");

  const std::string hola_name = "hola";
  std::string test_start_text = "Expected \"" + hola_name + "\" ";

  std::unique_ptr<Serializer::ISerializer> serializer(this->GetSerializer());

  EXPECT_TRUE(
      rec->Serialize(serializer.get(), hola_name.c_str(), hola_name.length()))
      << "failed to serialize";

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

  EXPECT_TRUE(
      serializerPretty->ParseText(resp.get(), static_cast<size_t>(size)))
      << "Failed to Parse Pretty Compile\n"
      << prettyCompile;

  std::unique_ptr<RecursiveStruct> rec_resp =
      RecursiveStruct::Deserialize(serializerPretty.get(), hola_name.c_str());

  EXPECT_TRUE(rec_resp) << "Failed to Deserialize in Pretty Compile\n"
                        << prettyCompile;
  EXPECT_TRUE(*rec == *rec_resp)
      << "Failed to Deserialize Correctly in Pretty Compile\n"
      << prettyCompile;

  // Compile
  EXPECT_TRUE(serializer->Compile()) << "Failed to Compile";

  EXPECT_TRUE(serializer->GetSize(&size)) << "Failed to Get Size of Compile";

  std::unique_ptr<char[]> resp2(new char[size]);

  EXPECT_TRUE(serializer->GetText(resp2.get())) << "Failed to Get Compile";

  std::unique_ptr<Serializer::ISerializer> serializerNormal(
      this->GetSerializer());

  std::string compile(resp2.get());

  EXPECT_TRUE(serializerNormal->ParseText(resp2.get()))
      << "Failed to Parse Compile" << compile;

  rec_resp.reset();
  rec_resp =
      RecursiveStruct::Deserialize(serializerNormal.get(), hola_name.c_str());

  EXPECT_TRUE(rec_resp) << "Failed to Deserialize in Compile\n" << compile;
  EXPECT_TRUE(*rec == *rec_resp)
      << "Failed to Deserialize Correctly in Compile\n"
      << compile;
}