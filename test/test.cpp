#include <catch2/catch_test_macros.hpp>
#include <iostream>

// change if you choose to use a different header name
#include "CampusCompass.h"
#include "Input_Parser.h"
using namespace std;

// the syntax for defining a test is below. It is important for the name to be
// unique, but you can group multiple tests with [tags]. A test can have
// [multiple][tags] using that syntax.
TEST_CASE("Input Validation", "[flag]") {
  // instantiate any class members that you need to test here// initialize your main project object
  CampusCompass compass;

  // ingest CSV data
  compass.ParseCSV("data/edges.csv", "data/classes.csv");

  Input_Parser parser(&compass, 1);

  SECTION("Invalid Name") {
    std::string success = parser.selectCommand("insert \"A11yson\" 45679999 20 2 COP3530 MAC2311");
    REQUIRE(success == "unsuccessful");
  }
  SECTION("Short ID") {
    std::string success = parser.selectCommand("insert ""Allison"" 3238900 20 2 COP3530 MAC2311");
    REQUIRE(success == "unsuccessful");
  }
  SECTION("Long ID") {
    std::string success = parser.selectCommand("insert ""Allison"" 323890001 20 2 COP3530 MAC2311");
    REQUIRE(success == "unsuccessful");
  }
  SECTION("Not Enough Classes") {
    std::string success = parser.selectCommand("insert ""Allison"" 32389000 20 3 COP3530 MAC2311");
    REQUIRE(success == "unsuccessful");
  }
  SECTION("Misspelled Command") {
    std::string success = parser.selectCommand("inshirt ""Allison"" 32389000 20 2 COP3530 MAC2311");
    REQUIRE(success == "unsuccessful");
  }
  SECTION("Invalid Class Name") {
    std::string success = parser.selectCommand("insert ""Allison"" 32389000 20 2 CO330 MAC2311");
    REQUIRE(success == "unsuccessful");
  }
}

TEST_CASE("Edge Cases", "[tag]") {
  CampusCompass compass;

  // ingest CSV data
  compass.ParseCSV("data/edges.csv", "data/classes.csv");

  Input_Parser parser(&compass, 1);
  parser.selectCommand("insert \"Brandon\" 45679999 20 2 COP3530 MAC2311");
  SECTION("Removing non-existent student") {
    std::string success = parser.selectCommand("remove 32387048");
    REQUIRE(success == "unsuccessful");
  };


  SECTION("Class Code does not exist") {
    std::string success = parser.selectCommand("dropClass 45679999 MAP4001");
    REQUIRE(success == "unsuccessful");
  };

  SECTION("Student Already Has Class") {
    std::string success = parser.selectCommand("replaceClass 45679999 COP3530 MAC2311");
    REQUIRE(success == "unsuccessful");
  };

  // each section runs the setup code independently to ensure that they don't
  // affect each other
}


TEST_CASE("Class Commands", "[tag]") {
  CampusCompass compass;
  compass.ParseCSV("data/edges.csv", "data/classes.csv");
  Input_Parser parser(&compass, 1);

  parser.selectCommand("insert \"Brandon\" 45679999 20 2 COP3530 MAC2311");
  parser.selectCommand("insert \"Brian\" 35459999 21 3 COP3530 CDA3101 MAC2311");
  parser.selectCommand("insert \"Briana\" 87879999 22 3 CDA3101 MAC2311 EEL3701");

  SECTION("Remove Class") {
    std::string success = parser.selectCommand("removeClass COP3530");
    REQUIRE(success == "2");
  }
  SECTION("Replace Class") {
    std::string success = parser.selectCommand("replaceClass 45679999 COP3530 COT3100");
    REQUIRE(success == "successful");
    std::string success2 = parser.selectCommand("removeClass COP3530");
    REQUIRE(success2 == "1");

  }

  SECTION("Remove Student") {
    std::string success = parser.selectCommand("remove 45679999");
    REQUIRE(success == "successful");
    std::string success2 = parser.selectCommand("removeClass COP3530");
    REQUIRE(success2 == "1");
  }

  SECTION("Drop Class") {
    std::string success = parser.selectCommand("dropClass 45679999 COP3530");
    REQUIRE(success == "successful");
    std::string success2 = parser.selectCommand("removeClass COP3530");
    REQUIRE(success2 == "1");
  }

}

TEST_CASE("Shortest Path") {
  CampusCompass compass;
  compass.ParseCSV("data/edges.csv", "data/classes.csv");
  Input_Parser parser(&compass, 4);

  std::string success = parser.selectCommand("insert \"Brandon\" 45679999 3 1 CNT4007");
  REQUIRE(success == "successful");
  std::string time1 = parser.selectCommand("printShortestEdges 45679999");

  std::string expected = R"(Time For Shortest Edges: Brandon
CNT4007: 8)";
  REQUIRE(time1 == expected);
  std::string success2 = parser.selectCommand("toggleEdgesClosure 1 3 6");
  REQUIRE(success2 == "successful");
  std::string time2 = parser.selectCommand("printShortestEdges 45679999");

  std::string expected2 = R"(Time For Shortest Edges: Brandon
CNT4007: 16)";
  REQUIRE(time2 == expected2);

  std::string success3 = parser.selectCommand("toggleEdgesClosure 1 3 5");
  REQUIRE(success3== "successful");

  std::string time3 = parser.selectCommand("printShortestEdges 45679999");

  std::string expected3 = R"(Time For Shortest Edges: Brandon
CNT4007: 17)";
  REQUIRE(time3 == expected3);

  std::string success4 = parser.selectCommand("toggleEdgesClosure 1 5 7");
  REQUIRE(success4== "successful");

  std::string time4 = parser.selectCommand("printShortestEdges 45679999");

  std::string expected4 = R"(Time For Shortest Edges: Brandon
CNT4007: 18)";
  REQUIRE(time4 == expected4);

  std::string success5 = parser.selectCommand("toggleEdgesClosure 1 3 4");
  REQUIRE(success5== "successful");

  std::string time5 = parser.selectCommand("printShortestEdges 45679999");

  std::string expected5 = R"(Time For Shortest Edges: Brandon
CNT4007: -1)";
  REQUIRE(time4 == expected4);

}

