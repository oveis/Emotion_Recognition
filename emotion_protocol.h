#ifndef emotion_protocol_h
#define emotion_protocol_h
#include <map>
#include <string>
using namespace std;

struct WordInfo{
  int count;        // How many times it is appeared
  int emotion_num;  // the emotion number of this word
  WordInfo(int emotion_num_)
    :count(1), emotion_num(emotion_num_){ }
};

struct Emotion{
  string name;      // Emotion name ; happy, sad, so on
  int sentence_count;
  map<string, WordInfo*> words_map;   // the word which was appead in this sentence emotion
  Emotion(string name_)
    :name(name_), sentence_count(1){ }
};

#endif
