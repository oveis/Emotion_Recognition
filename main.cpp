#include <iostream>
#include <vector>
#include "naiveBayes.h"
#include "emotion_protocol.h"
#include "database.h"
#include "nGram.h"
#include "viterbi.h"
using namespace std;

void showResult(Database *db);
void showStatistics(int N, Database *db);

int main(int argc, char* argv[]){

  if(argc < 4){
    cout << "FORMAT : ./emotion  N  vesion  verbos(0:off, 1:on)" << endl;
    cout << "** version (1:Statictics, 2:Hybrid N-gram, 3:Hybrid Naive Bayes)" << endl;
    exit(1);
  }

  int N = atoi(argv[1]);
  int ver = atoi(argv[2]);
  int verbos = atoi(argv[3]);
  string input_text;
  // Database
  Database *db = new Database();  

  // Input text
  cout << "\n******************************************************\n";
  cout << "Thank you for comming in Emotion Recognition Project\n";
  cout << "If you want to out, please enter 'q'\n";
  cout << "******************************************************\n";


  // show statistics
  if(ver == 1){
    showStatistics(N, db);
    goto Quit;
  }

  while(true){
    cout << "Input text >> ";
    getline(cin, input_text);
    
    if(input_text == "q"){
      goto Quit;
      break;
    }

    // Input new text with (Hybrid N-gram & Naive Bayes)
    if(ver == 2){
      Learn l(N, db);
      double l_prob;
      int l_emotion_id = l.getEmotion(input_text, l_prob);
      string l_emotion_name = db->getEmotionName(l_emotion_id);
      cout << "emotion(Hybrid N-gram) : " << l_emotion_name << endl;
    }

    // Input new text with (Hybrid naive Bayes HMM)
    else if(ver == 3){
      // use Naive Bayes algorithm
      NaiveBayes nb(db);
      nb.setVerbos(verbos);
      double nb_prob;
    
      int emotion_id = nb.getEmotion(input_text, nb_prob);
      string emotion_name = db->getEmotionName(emotion_id);
      cout << "emotion(Naive) : " << emotion_name << endl;

      // use Viterbi algorithm
      if(db->getTextSequence().size() >= N-1){
	Viterbi vt(db, N);
	double vt_prob;
	int vt_emotion_id = vt.getEmotion(input_text, vt_prob);
	string vt_emotion_name = db->getEmotionName(vt_emotion_id);
	cout << "emotion(Viterbi) : " << vt_emotion_name << endl;
	//cout << "probability : " << vt_prob << endl << endl;
	// Save input text as history
	db->pushTextSequence(new TextInfo(input_text, vt_emotion_id, vt_prob));
      }else{
	// Save input text as history
	db->pushTextSequence(new TextInfo(input_text, emotion_id, nb_prob));
      }
    }
  }

  Quit:
  showResult(db);    

  return 0;
}

void showResult(Database *db){
  cout << "\n******************************************************\n";
  cout << "Result History\n";
  cout << "******************************************************\n";
  vector<TextInfo*> text_sequence = db->getTextSequence();
  for(int i=0; i<text_sequence.size(); i++){
    TextInfo *ti = text_sequence[i];
    cout << ti->text << " [" << db->getEmotionName(ti->emotion_id) << " : " << (int)(ti->nb_prob * 100) << "%]\n";
  }
  cout << "******************************************************\n";
  cout << "Thank you very much!!\nHave a good day!\n\n";
}

void showStatistics(int N, Database *db){
  cout<<"Naive Bayes"<<endl;
  Learn l(1, db);
  l.test3();

  cout << endl;
  cout<<"Naive N-gram" <<endl;
  for (int i = 1; i<=N;i++) {
    Learn l(i, db);
    l.test();
  }

  cout << endl;
  cout<<"Hybrid N-gram and Naive Bayes"<<endl;
  for (int i = 1; i<=N;i++) {
    Learn l(i, db);
    l.test2();
  }
  
  cout << endl;
  cout << "Hybrid naive Bayes HMM" << endl;
  for(int i=1; i<=N; i++){
    Viterbi v(db, i);
    v.test();
  }

  cout << endl;
  cout << "Sate Transition Probability" << endl;
  db->showStateTransProb();
  
  cout << endl;
  cout << "Database" << endl;
  cout << "Total State of Emotion : 7" << endl;
  cout << "Total Sentences : " << db->getTrainTotalSentence() << endl;
  cout << "Total Words : " << db->getTrainTotalWord() << endl;
}
