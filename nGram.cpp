#include "nGram.h"
#include <fstream>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include "naiveBayes.h"
using namespace std;
Learn::Learn(int n, Database *db_):N(n),curState(0), db(db_) {
  max = 1;
  for (int i = 0; i<N; i++)
    max*=N;
  loadHistSeq();
  countIng();
}
void Learn::loadHistSeq() {
  fstream input("./data/learning_set/learning.db.txt");
  string buff;
  eachEmcount.resize(7,0);
  while (getline(input,buff)){
    int em;
    int pos;
    if((pos = buff.find("@")) != string::npos){         // sentence
      if(buff.substr(pos+1) == " ")
        em = 0;
      else
        em = atoi( buff.substr(pos+1).c_str() );
      dbSeq.push_back(em);
      dbSen.push_back(buff);
      eachEmcount[em] ++;

    }
  }
  /*  for (int i = 0; i<eachEmcount.size();i++)
    cout<<eachEmcount[i]<<' ';
    cout<<endl;*/
}
void Learn::countIng() {
  prob.resize(max);
  for (int i = 0; i<max;i++)
    prob[i].resize(7,1);
  int cur = 0;
  for (int i = 0; i<dbSeq.size();i++){
    prob[cur][dbSeq[i]]++;
    cur= (cur*N + dbSeq[i])%max;
  }
  for (int i = 0; i<max; i++) {
    int sum = 0;
    for (int j = 0; j<7;j++)
      sum+= prob[i][j];
    prob[i].push_back(sum);
  }
}
int Learn::preDict() {
  int ans = -1;
  double pa = 0;
  double p1;
  int sum = 0;

  for (int i = 1; i<7;i++) {
    p1 = ((double)prob[curState][i])/ ((double)prob[curState][7]);
    p1 = p1 / ((double)eachEmcount[i]);
    if (p1>pa) {
      pa = p1;
      ans = i;
    }
  }
  return ans;
}

int Learn::preDict(NaiveBayes *nb,string s) {
  int ans = -1;
  double pa = 0;
  double p1;
  int sum = 0;

  for (int i = 1; i<7;i++) {
    p1 = ((double)prob[curState][i])/ ((double)prob[curState][7]);
    p1 = p1 / ((double)eachEmcount[i])*
      nb->calNaiveBayesWithID(s, i);

    if (p1>pa) {
      pa = p1;
      ans = i;
    }
  }
  preDic_prob = pa;
  return ans;
}

int Learn::preDict2(NaiveBayes *nb,string s) {
  int ans = -1;
  double pa = 0;
  double p1;
  int sum = 0;

  for (int i = 1; i<7;i++) {
    p1 = nb->calNaiveBayesWithID(s, i);

    if (p1>pa) {
      pa = p1;
      ans = i;
    }
  }
  return ans;
}

void Learn::feedBack(int em) {
  histSeq.push_back(em);
  curState = (curState*N+em)%max;
}
void Learn::test2() {
  int matched = 0;
  int count = 0;
  NaiveBayes nb(db);
  for (int i = 0; i< dbSeq.size();i++) {
  //  for (int i = dbSeq.size()/2+1; i< dbSeq.size();i++) {
    int pre = preDict(&nb,dbSen[i]);
    if (dbSeq[i]!=0) {
      if (pre == dbSeq[i])
	matched++;
      count++;
    }
    feedBack(dbSeq[i]);
  }
  //  double acc = ((double)matched)/((double)dbSeq.size());
  double acc = ((double)matched)/((double)count);
  cout.setf(ios::fixed, ios::floatfield);
  cout.setf(ios::showpoint);
  cout.precision(2);
  cout<<"N="<<N<<"  Accuracy ="<< (acc*100) << "%\n";
}

void Learn::test3() {
  int matched = 0;
  int count = 0;
  NaiveBayes nb(db);
  for (int i = 0; i< dbSeq.size();i++) {
    //for (int i = dbSeq.size()/2+1; i< dbSeq.size();i++) {
    int pre = preDict2(&nb,dbSen[i]);
    if (dbSeq[i]!=0){
      if (pre == dbSeq[i])
	matched++;
      count++;
    }
    feedBack(dbSeq[i]);
  }
  //  double acc = ((double)matched)/((double)dbSeq.size());
  double acc = ((double)matched)/((double)count);
  cout.setf(ios::fixed, ios::floatfield);
  cout.setf(ios::showpoint);
  cout.precision(2);
  cout<<"Accuracy = "<<acc << "%\n";
}
void Learn::test() {
  int matched = 0;
  int count = 0;
  for (int i = 0; i< dbSeq.size();i++) {
  // for (int i = dbSeq.size()/2+1; i< dbSeq.size();i++) {  
    int pre = preDict();
    if (dbSeq[i]!=0) {
      if (pre == dbSeq[i])
	matched++;
      count++;
    }
    feedBack(dbSeq[i]);
  }
  //  double acc = ((double)matched)/((double)dbSeq.size());
  double acc = ((double)matched)/((double)count);
  cout.setf(ios::fixed, ios::floatfield);
  cout.setf(ios::showpoint);
  cout.precision(2);
  cout<<"N="<<N<<"  Accuracy = "<< (acc*100) << "%\n";
  //cout<<acc<<endl;
}

int Learn::getEmotion(string text, double &prob){
  int matched = 0;
  int count = 0;
  NaiveBayes nb(db);
  int pre = preDict(&nb,text);
  prob = preDic_prob;
  return pre;
}
