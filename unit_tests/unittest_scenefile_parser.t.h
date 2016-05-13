// Unit tests for code in scenefile_parser.cpp

// older compilation notes:
// $ cxxtestgen.py --error-printer -o test_runner.cpp unit_tests/unittest_scenefile_parser.t.h
// $ clang++ -o test_runner test_runner.cpp utilities.cpp scenefile_parser.cpp -I/usr/local/include -lyaml-cpp

#include <cxxtest/TestSuite.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

#include "utilities_pub.h"
#include "scene_file_parser.h"

const string  TEST_SCENEFILE_GOOD("./simplescene.yml");
const string  TEST_SCENEFILE_BAD1("./badscene.yml");


class NewTestSuite : public CxxTest::TestSuite 
{
public:

  // Test good scene file
  void testVetSceneFile_Simple( void )
  {
    
    bool fileStatus = VetSceneFile(TEST_SCENEFILE_GOOD);
    TS_ASSERT( fileStatus == true );
  }


};
