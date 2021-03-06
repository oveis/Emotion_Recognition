#ifndef NaiveBayes_h
#define NaiveBayes_h
#include "emotion_protocol.h"
#include "database.h"
#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
using namespace std;

class NaiveBayes{
 public:
  NaiveBayes(Database *db);
  ~NaiveBayes();
  int getEmotion(string input, double &prob);
  int calNaiveBayes(vector<string> words, double &prob);
  double calNaiveBayesWithID(string text, int id);
  void setVerbos(int i){ verbos = i; }
  unsigned int total_word;
  map<int, double> emotion_prob;  // probability of the sentence emotion 
  
 private:
  map<int, Emotion*> learning_map;
  Database *db;
  int verbos;
};

#endif
