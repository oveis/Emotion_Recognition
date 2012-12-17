#include "nGram.h"

#include <iostream>
using namespace std;
int main () {
  Database *db = new Database();
  cout << endl;
  cout<<"Naive Bayes"<<endl;
  Learn l(1, db);
  l.test3();

  cout<<"Naive N-gram" <<endl;
  for (int i = 1; i<9;i++) {
    Learn l(i, db);
    l.test();
  }

  cout << endl;
  cout<<"Hybrid N-gram and Naive Bayes"<<endl;
  for (int i = 1; i<9;i++) {
    Learn l(i, db);
    l.test2();
  }
}
