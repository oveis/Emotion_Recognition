#ifndef Database_h
#define Database_h
#include <fstream>
#include <vector>
#include <map>
#include <stdlib.h>
#include <sstream>
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

 public:
  Database();
  ~Database();
  void getClassTable();
  void getLearningData();
  void getNotImportantData();
  void getTransferData();
  map<int, Emotion*> getLearningMap(){ return learning_map; }
  map<string, int> getNotImportantMap(){ return not_important_map; }
  string getEmotionName(int id){ return learning_map[id]->name; }
};

#endif
