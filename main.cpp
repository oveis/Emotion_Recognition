#include <iostream>
#include "naiveBayes.h"
#include "emotion_protocol.h"
#include "database.h"
using namespace std;

int main(int argc, char* argv[]){
  
  string input_text;
  
  // Database
  Database *db = new Database();

  // Input text
  cout << "Input text >> ";
  getline(cin, input_text);

  // use Naive Bayes algorithm
  NaiveBayes nb(db->getLearningMap());

  int emotion_id = nb.getEmotion(input_text);
  string emotion_name = db->getEmotionName(emotion_id);
  cout << "emotion : " << emotion_name << endl;
  
  return 0;
}

