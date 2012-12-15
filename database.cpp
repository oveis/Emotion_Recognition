#include "database.h"

Database::Database(){
  // open files
  emotion_class_file.open("./data/class_table/emotion_class.txt");
  learning_data_file.open("./data/learning_set/learning.db.txt");
  not_important_words_file.open("./data/learning_set/not_important_words.txt");

  // Get data from files
  // Read Class data
  getClassTable();
 // Read not-important-data
  getNotImportantData();
  // Read training data
  getLearningData();
}

Database::~Database(){
  // close files
  emotion_class_file.close();
  learning_data_file.close();
  not_important_words_file.close();
}

// Get the emotion_class_file data
void 
Database::getClassTable(){
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
void 
Database::getLearningData(){
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
void 
Database::getNotImportantData(){
  string buff;
  int index = 0;
  while(getline(not_important_words_file, buff)){
    not_important_map[buff] = index;
  }
}

// Transfer 
// ex) loving, lovely, love   ==> love
