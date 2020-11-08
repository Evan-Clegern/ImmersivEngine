/*
TESTING FILE: November 2020
FILE: error-finding.cpp
PURPOSE: Find errors with the functions of base_entity.h
*/
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <fstream>
#include "dist/jsoncpp.cpp"
#include "dist/json/json-forwards.h"
#include "base_entity.h"
using namespace std;
using namespace Json;
using namespace entbaseD;
using namespace entbaseFIN;
using namespace entbaseFOUT;
//Date : 8 November 2020
point test_pnt() {
  point d = p(185.30, -184.13, 10.2);
  point e = p(100.34, -10.66, 0.27);
  float goop = d >> e;
  point added = d + e;
  point g = added;
  return g;
}

int main() {
  point bruh = test_pnt();
  return 0;
}
