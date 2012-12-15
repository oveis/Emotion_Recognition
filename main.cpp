#include <iostream>
#include "naiveBayes.h"
#include "emotion_protocol.h"
#include "database.h"
using namespace std;

void showResult(Database *db);

int main(int argc, char* argv[]){
  
  string input_text;
  
  // Database
  Database *db = new Database();

  // Input text
  cout << "\n******************************************************\n";
  cout << "Thank you for comming in Emotion Recognition Project\n";
  cout << "If you want to out, please enter 'q'\n";
  cout << "******************************************************\n";

  while(true){
    cout << "Input text >> ";
    getline(cin, input_text);

    if(input_text == "q"){
      cout << "Thank you very much!!\nHave a good day!\n\n";
      break;
    }

    // use Naive Bayes algorithm
    NaiveBayes nb(db->getLearningMap());

    int emotion_id = nb.getEmotion(input_text);
    string emotion_name = db->getEmotionName(emotion_id);
    cout << "emotion : " << emotion_name << endl << endl;
  }

  return 0;
}

void showResult(Database *db){
  for(int i=0; i<7; i++){
    for(int j=0; j<7; j++){
      double prob = db->getStateTransitionProb(i, j);
      cout << prob << endl;
      //printf("[%d][%d] %f\t", i, j, prob);
    }
    cout << endl;
  }
}

