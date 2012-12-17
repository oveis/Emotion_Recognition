#include "naiveBayes.h"

NaiveBayes::NaiveBayes(Database *db_)
  :db(db_)
{
  learning_map = db->getLearningMap();
  total_word = db->getTrainTotalWord();
  emotion_prob = db->getInitialEmotionProb();
  verbos = 0;
}

NaiveBayes::~NaiveBayes()
{
}

int NaiveBayes::getEmotion(string input, double &prob){
  vector<string> words = db->parseText(input);
  int ret = calNaiveBayes(words, prob);
  return ret;
}

// Calculate the probability of Naive Bayes algorithm with Emotion ID
double
NaiveBayes::calNaiveBayesWithID(string text, int id){
  vector<string> words = db->parseText(text);
  double result = db->getInitialEmotionProb(id);
  Emotion *emotion = db->getLearningMap()[id];
  for(int i=0; i<words.size(); i++){
    if(emotion->words_map.find( words[i] ) != emotion->words_map.end()){
        //result *= emotion->words_map[ words[i] ]->count / total_word;
        result *= emotion->words_map[ words[i] ]->count;
    }else{
        //result *= 1 / total_word;
    }
  }
  return result;
}

int 
NaiveBayes::calNaiveBayes(vector<string> words, double& prob){
  double max_emotion_prob = 0;
  int max_emotion_num = 0;
  map<int, Emotion*>::iterator it;
  for(it=learning_map.begin(); it!=learning_map.end(); it++){
    int emotion_num = it->first;
    if(emotion_num == 0)   // skip the 'nothing-emotion'
      continue;
    double result = emotion_prob[emotion_num];
    Emotion *emotion = it->second;
    
    if(verbos == 1)
      cout << "\nEmotion [" << emotion->name << "] : " << result << endl;

    for(int i=0; i<words.size(); i++){
      if(emotion->words_map.find( words[i] ) != emotion->words_map.end()){
        //result *= emotion->words_map[ words[i] ]->count / total_word;
        result *= emotion->words_map[ words[i] ]->count;
	if(verbos == 1)
	  cout << "word : " << words[i] << ", count : " << emotion->words_map[ words[i] ]->count << endl;
      }else{
        //result *= 1 / total_word;
      }
    }
    if(verbos == 1)
      cout << "result >> " << result << endl;
    
    
    if(result > max_emotion_prob){
      max_emotion_prob = result;
      max_emotion_num = emotion_num;
    }
  }
  prob = max_emotion_prob;
  return max_emotion_num;
}
