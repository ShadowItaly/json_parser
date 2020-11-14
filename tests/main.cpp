#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "../json_parser.hpp"

TEST_CASE("Checking basic json parsing","[json_parse]")
{
  auto js = Json::parse("{\"key\":\"hallo\"}",[](Json::JsonParser&){});

  REQUIRE(js.type() == JsonType::object);
  REQUIRE(js.get("key").type() == JsonType::string);

  std::string val;
  js.get("key").map_string([&val](std::string &v){val = v;});
  REQUIRE(val == "hallo");
}

TEST_CASE("Checking integer json parsing","[json_parse]")
{
  auto js = Json::parse("{\"key\":100}",[](Json::JsonParser&){});

  REQUIRE(js.type() == JsonType::object);
  REQUIRE(js.get("key").type() == JsonType::integer);

  int x;
  js.get("key").map_int([&x](int y){x = y;});
  REQUIRE(x == 100);
}


TEST_CASE("Checking parse error","[json_parse]")
{
  bool set_err = false;
  auto js = Json::parse("{\"key\":100,,}",[&set_err](Json::JsonParser&){set_err = true;});

  REQUIRE(set_err == true);
}


TEST_CASE("Checking multi level parsing","[json_parse]")
{
  bool set_err = false;
  auto js = Json::parse("{\"key\": {\"tor\":\"hallo\"}}",[&set_err](Json::JsonParser&){set_err = true;});

  REQUIRE(js.type() == JsonType::object);
  REQUIRE(js.get("key").type() == JsonType::object);
  REQUIRE(js.get("key").get("tor").type() == JsonType::string);

  std::string val;
  js.get("key").get("tor").map_string([&val](std::string &v){val = v;});
  REQUIRE(val == "hallo");
}


TEST_CASE("Checking int and float mixing","[json_parse]")
{
  bool set_err = false;
  auto js = Json::parse("{\"key\": 10, \"loko\": 2.5}",[&set_err](Json::JsonParser&){set_err = true;});

  REQUIRE(js.type() == JsonType::object);
  REQUIRE(js.get("key").type() == JsonType::integer);
  REQUIRE(js.get("loko").type() == JsonType::floating_point);
}


TEST_CASE("Checking illegal function","[json_parse]")
{
  bool set_err = false;
  auto js = Json::parse("[10,21]",[&set_err](Json::JsonParser&){set_err = true;});

  REQUIRE(js.type() == JsonType::array);
  REQUIRE(js.size() == 2);
  js.set("illegal","fuchs")
    .set("thor",false)
    .set("ok",true);

  REQUIRE(js.has_error() == true);
}


TEST_CASE("Checking illegal function size changes","[json_parse]")
{
  bool set_err = false;
  auto js = Json::parse("[10,21]",[&set_err](Json::JsonParser&){set_err = true;});

  REQUIRE(js.type() == JsonType::array);
  REQUIRE(js.size() == 2);
  js.set("illegal","fuchs")
    .set("thor",false)
    .set("ok",true);

  REQUIRE(js.has_error() == true);
  REQUIRE(js.size() == 2);
}



TEST_CASE("Nested size","[json_parse]")
{
  bool set_err = false;
  auto js = Json::parse("[10,21,{\"nice\":true}]",[&set_err](Json::JsonParser&){set_err = true;});

  REQUIRE(set_err == false);
  REQUIRE(js.type() == JsonType::array);
  REQUIRE(js.size() == 3);
  REQUIRE(js.get(2).type() == JsonType::object);
  js.set("illegal","fuchs")
    .set("thor",false)
    .set("ok",true);

  REQUIRE(js.has_error() == true);
  REQUIRE(js.size() == 3);
}


TEST_CASE("Empty array","[json_parse]")
{
  bool set_err = false;
  auto js = Json::parse("[]",[&set_err](Json::JsonParser&){set_err = true;});

  REQUIRE(set_err == false);
  REQUIRE(js.type() == JsonType::array);
  REQUIRE(js.size() == 0);
}



TEST_CASE("Empty object","[json_parse]")
{
  bool set_err = false;
  auto js = Json::parse("{}",[&set_err](Json::JsonParser&){set_err = true;});

  REQUIRE(set_err == false);
  REQUIRE(js.type() == JsonType::object);
  REQUIRE(js.size() == 0);
}


TEST_CASE("Empty object add attributes","[json_parse]")
{
  bool set_err = false;
  auto js = Json::parse("{}",[&set_err](Json::JsonParser&){set_err = true;});

  REQUIRE(set_err == false);
  js.set("Hello","World");
  REQUIRE(js.type() == JsonType::object);
  REQUIRE(js.size() == 1);
  REQUIRE(js.has_error() == false);
}


TEST_CASE("Empty object add multiple attributes","[json_parse]")
{
  bool set_err = false;
  auto js = Json::parse("{}",[&set_err](Json::JsonParser&){set_err = true;});

  REQUIRE(set_err == false);
  js.set("Hello","World")
    .set("is_true",true);
  REQUIRE(js.type() == JsonType::object);
  REQUIRE(js.size() == 2);
  REQUIRE(js.has_error() == false);
}


TEST_CASE("Deep parenthesis","[json_parse]")
{
  bool set_err = false;
  auto js = Json::parse("[[[[]]]]",[&set_err](Json::JsonParser&){set_err = true;});

  REQUIRE(set_err == false);
  REQUIRE(js.type() == JsonType::array);
  REQUIRE(js.size() == 1);
  REQUIRE(js.has_error() == false);

  REQUIRE(js.get(0).size() == 1);
  REQUIRE(js.get(0).type() == JsonType::array);
  REQUIRE(js.get(0).get(0).type() == JsonType::array);
  REQUIRE(js.get(0).get(0).size() == 1);
}


TEST_CASE("Deep parenthesis access","[json_parse]")
{
  bool set_err = false;
  auto js = Json::parse("[[[[]]]]",[&set_err](Json::JsonParser&){set_err = true;});

  REQUIRE(set_err == false);
  REQUIRE(js.type() == JsonType::array);
  REQUIRE(js.size() == 1);
  REQUIRE(js.has_error() == false);

  REQUIRE(js.get(0).size() == 1);
  REQUIRE(js.get(0).type() == JsonType::array);
  REQUIRE(  js.get(0)
              .get(0)
              .get("thor")
              .get("thor")
              .get("thor")
              .get("thor").has_error() == true);
}

TEST_CASE("Deep parenthesis access missing one quote","[json_parse]")
{
  bool set_err = false;
  auto js = Json::parse("[ [ [ [ ,] ] ]",[&set_err](Json::JsonParser&){set_err = true;});

  REQUIRE(set_err == true);
}

TEST_CASE("Stringify jsons","[json_parse]")
{
  bool set_err = false;
  std::string js_str = "[[[[]]]]";
  auto js = Json::parse(js_str,[&set_err](Json::JsonParser&){set_err = true;});

  REQUIRE(set_err == false);
  REQUIRE(js.dump() == js_str);
}

TEST_CASE("Stringify jsons and serialize","[json_parse]")
{
  bool set_err = false;
  std::string js_str = "[10,20]";
  auto js = Json::parse(js_str,[&set_err](Json::JsonParser&){set_err = true;});
  auto js2 = Json::parse(js.dump(),[&set_err](Json::JsonParser&){set_err = true;});

  REQUIRE(set_err == false);
  REQUIRE(js.dump() == js2.dump());
  REQUIRE(js.size() == js2.size());
  REQUIRE(js.type() == js2.type());
}
