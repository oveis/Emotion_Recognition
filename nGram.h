#ifndef learn_h_
#define learn_h_
#include <string>
#include <map>
#include <vector>
//#include "database.h"
using namespace std;
class Learn {
 private:
  vector<int> eachEmcount;
  vector<int> dbSeq;
  vector<int> histSeq;
  vector< vector<int> > prob;
  int N;
  int curState;
  void loadHistSeq();
  void countIng();
  int max;
 public:
  Learn(int n);
  void feedBack(int em);
  int preDict();
  void test();
};

#endif
