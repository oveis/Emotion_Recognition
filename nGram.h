#ifndef learn_h_
#define learn_h_
#include <string>
#include <map>
#include <vector>
#include "naiveBayes.h"
#include "database.h"
using namespace std;
class Learn {
 private:
  vector<int> eachEmcount;
  vector<int> dbSeq;
  vector<string> dbSen;
  vector<int> histSeq;
  vector< vector<int> > prob;
  int N;
  int curState;
  void loadHistSeq();
  void countIng();
  int max;
  double preDic_prob;
  Database *db;
 public:
  Learn(int n, Database *db_);
  void feedBack(int em);
  int preDict();
  int preDict(NaiveBayes *nb, string s);
  int preDict2(NaiveBayes *nb, string s);
  void test();
  void test2();
  void test3();
  int getEmotion(string text, double &prob);
};

#endif
