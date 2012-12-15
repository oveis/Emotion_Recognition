#ifndef Database_h
#define Database_h
#include <fstream>
#include <vector>
#include <map>
#include <stdlib.h>
#include <sstream>
#include <iostream>
#include "emotion_protocol.h"
using namespace std;

class Database{
 private:
  ifstream emotion_class_file;
  ifstream learning_data_file;
  ifstream not_important_words_file;
  ifstream transfer_file;
  map<int, Emotion*> learning_map;
  map<string, int>  not_important_map;
  map<string, string> transfer_map;
  map<int, double> initial_emotion_prob; // robability of the sentence emotion 
  double state_transition_prob[7][7];
  unsigned int train_total_sentence;
  unsigned int train_total_word;

 public:
  Database();
  ~Database();
  void getClassTable();
  void getLearningData();
  void getNotImportantData();
  void getTransferData();
  void calInitialEmotionProb();
  map<int, Emotion*> getLearningMap(){ return learning_map; }
  map<string, int> getNotImportantMap(){ return not_important_map; }
  string getEmotionName(int id){ return learning_map[id]->name; }
  double getStateTransitionProb(int i, int j){ return state_transition_prob[i][j]; }
  map<int, double> getInitialEmotionProb(){ return initial_emotion_prob; }
  double getInitialEmotionProb(int id){ return initial_emotion_prob[id]; }
  unsigned int getTrainTotalSentence(){ return train_total_sentence; }
  unsigned int getTrainTotalWord(){ return train_total_word; }
};

#endif
