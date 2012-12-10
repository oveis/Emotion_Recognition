#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <stdlib.h>
#include <sstream>
#include "naiveBayes.h"
#include "emotion_protocol.h"
using namespace std;

ifstream emotion_class_file;
ifstream learning_data_file;
map<int, Emotion*> learning_map;

// Get the emotion_class_file data
void getClassTable(){
  string buff;
  while(getline(emotion_class_file, buff)){
    stringstream ss;
    string emotion_name;
    int emotion_num;
    ss << buff;
    ss >> emotion_name >> emotion_num;
    learning_map[emotion_num] = new Emotion(emotion_name);
  }
}

// Get the learning_data_file
void getLearningData(){
  string buff;
  int sentence_emotion = 0;
  while(getline(learning_data_file, buff)){
    int pos;
    if((pos = buff.find("@")) != string::npos){         // sentence
      if(buff.substr(pos+1) == " ")
	sentence_emotion = 0;
      else
	sentence_emotion = atoi( buff.substr(pos+1).c_str() );
      // Increate count
      learning_map[sentence_emotion]->sentence_count++;
    }else if((pos = buff.find(":")) != string::npos){   // word
      stringstream ss;
      string word;
      ss << buff;
      ss >> word;
      int word_emotion;
      if(buff.substr(pos+1) == " ")
	word_emotion = 0;
      else
	word_emotion = atoi( buff.substr(pos+1).c_str() );
      Emotion *emotion = learning_map[sentence_emotion];
      if(emotion->words_map.find(word) == emotion->words_map.end()){
	//	WordInfo wordInfo(word_emotion);
	emotion->words_map[word] = new WordInfo(word_emotion);
      }else{
	emotion->words_map[word]->count += 1;	
      }
      learning_map[sentence_emotion] = emotion;
    }else{                                               // blank
      sentence_emotion = 0;
    }
  }
}

int main(int argc, char* argv[]){
  emotion_class_file.open("./data/class_table/emotion_class.txt");
  learning_data_file.open("./data/learning_set/learning.db.txt");
  string input_text;
  
  // Read Class data
  getClassTable();

  // Read training data
  getLearningData();

  // Input text
  cout << "Input text >> ";
  cin >> input_text;

  // use Naive Bayes algorithm
  NaiveBayes nb(learning_map);
  int emotion_num = nb.getEmotion(input_text);
  cout << "emotion number : " << emotion_num << endl;
  
  // close files
  emotion_class_file.close();
  learning_data_file.close();

  return 0;
}

