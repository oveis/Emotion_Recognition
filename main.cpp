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
ifstream not_important_words_file;
map<int, Emotion*> learning_map;
map<string, int>  not_important_map;

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

      // Check it this word is not-importat or not
      if(not_important_map.find(word) != not_important_map.end()){
	continue;  // If this is not important word, skip this.
      }

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

// Get not important words to skip this words in the training data
void getNotImportantData(){
  string buff;
  int index = 0;
  while(getline(not_important_words_file, buff)){
    not_important_map[buff] = index;
  }
}

int main(int argc, char* argv[]){
  emotion_class_file.open("./data/class_table/emotion_class.txt");
  learning_data_file.open("./data/learning_set/learning.db.txt");
  not_important_words_file.open("./data/learning_set/not_important_words.txt");
  string input_text;
  
  // Read Class data
  getClassTable();

 // Read not-important-data
  getNotImportantData();

  // Read training data
  getLearningData();

  // Input text
  cout << "Input text >> ";
  //cin >> input_text;
  getline(cin, input_text);

  // use Naive Bayes algorithm
  NaiveBayes nb(learning_map);
  cout << "main : input : " << input_text << endl;
  int emotion_num = nb.getEmotion(input_text);
  string emotion_name = learning_map[emotion_num]->name;
  cout << "emotion : " << emotion_name << endl;
  
  // close files
  emotion_class_file.close();
  learning_data_file.close();
  not_important_words_file.close();

  return 0;
}

