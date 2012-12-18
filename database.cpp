#include "database.h"

Database::Database(){
  // open files
  emotion_class_file.open("./data/class_table/emotion_class.txt");
  learning_data_file.open("./data/learning_set/learning.db.txt");
  not_important_words_file.open("./data/learning_set/not_important_words.txt");
  transfer_file.open("./data/learning_set/transfer_table.txt");

  for(int i=0; i<7; i++)
    for(int j=0; j<7; j++)
      state_transition_prob[i][j] = 0;

  // Get data from files
  // Read Class data
  getClassTable();
 // Read not-important-data
  getNotImportantData();
  // Read training data
  getLearningData();
  // Read transfer data
  getTransferData();
  // Calculate Initial Emotion Probability / Total Train word & sentence
  calInitialEmotionProb();
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
  int prev_sentence_emotion = 0;
  int total_sentence = 0;

  while(getline(learning_data_file, buff)){
    int pos;
    if((pos = buff.find("@")) != string::npos){         // sentence
      if(buff.substr(pos+1) == " ")
        sentence_emotion = 0;
      else
        sentence_emotion = atoi( buff.substr(pos+1).c_str() );
      
      dbSeq.push_back(sentence_emotion);
      dbSen.push_back(buff.substr(0, pos-1));
      // Increate count
      state_transition_prob[prev_sentence_emotion][sentence_emotion]++;  // state_transition_probability
      learning_map[sentence_emotion]->sentence_count++;
      prev_sentence_emotion = sentence_emotion;
      total_sentence++;
    } else if((pos = buff.find(":")) != string::npos){   // word
      stringstream ss;
      string word;
      ss << buff;
      ss >> word;

      // Check it this word is not-importat or not
      if(not_important_map.find(word) != not_important_map.end()){
        continue;  // If this is not important word, skip this.
      }

      // Transfer this word into original word
      if(transfer_map.find(word) != transfer_map.end())
        word = transfer_map[word];

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

  // Calculate State Transition Probability
  for(int i=0; i<7; i++){
    int total = 0;
    for(int j=0; j<7; j++)
      total += state_transition_prob[i][j];
    for(int j=0; j<7; j++)
      state_transition_prob[i][j] /= total;
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
void
Database::getTransferData(){
  string buff;
  string aim_word;
  while(getline(transfer_file, buff)){
    stringstream ss;
    string word, division;
    ss << buff;
    ss >> aim_word;
    ss >> division;
    if(division == ":"){
      while(ss >> word){
	transfer_map[word] = aim_word;
      }
    }
  }
}

// Calculate Initial Emotion Probability
void
Database::calInitialEmotionProb(){
  train_total_sentence = 0;
  train_total_word = 0;
  map<int, Emotion*>::iterator it;
  for(it = learning_map.begin(); it != learning_map.end(); it++){
    Emotion *emotion = it->second;

    train_total_sentence += emotion->sentence_count;
    initial_emotion_prob[it->first] = emotion->sentence_count;

    map<string, WordInfo*> words_map = emotion->words_map;;
    map<string, WordInfo*>::iterator w_it;
    for(w_it = words_map.begin(); w_it != words_map.end(); w_it++){
      train_total_word += w_it->second->count;
    }
  }
  
  map<int, double>::iterator e_it;
  for(e_it = initial_emotion_prob.begin(); e_it != initial_emotion_prob.end(); e_it++){
    int emotion_num = e_it->first;
    double prob = e_it->second / (double)train_total_sentence;
    initial_emotion_prob[emotion_num] = prob;
  }
}

// Parse String as vector<string>
// if the word can transfer, transfer it.
vector<string>
Database::parseText(string text){
  stringstream ss;
  vector<string> words;
  string word;
  ss << text;
  while(ss >> word){
    if(transfer_map.find(word) != transfer_map.end())
      word = transfer_map[word];
    words.push_back(word);
  }
  return words;
}

// Show State Transition Probability
void
Database::showStateTransProb(){
  cout << "\t";
  for(int i=0; i<7; i++){
      cout << getEmotionName(i) << "\t";
  }
  cout << endl;
    
  for(int i=0; i<7; i++){
    cout << getEmotionName(i) << "\t| ";
    for(int j=0; j<7; j++){
      cout << state_transition_prob[i][j] << "\t| ";
    }
    cout << endl;
  }
}
