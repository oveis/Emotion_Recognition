#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

int main(int argc, char* argv[])
{
  if(argc < 1){
    cout << "FORMAT : ./extractor  input_file(.sim)  output_file(.txt)" << endl;
    return 1;
  }
  
  ifstream input_file(argv[1]);
  ofstream output_file(argv[2], ios_base::out|ios_base::app);
  string buff;
  
  bool isStart = false;
  while(getline(input_file, buff)){
    if(buff.find("<!-- English Subtitle -->") != string::npos){
      isStart = true;
      continue;
    }else if(buff.find("<!-- English Subtitle End -->") != string::npos){
      isStart = false;
      continue;
    }
    if(!isStart)
      continue;

    if(buff.find("<SYNC") != string::npos)
      continue;

    // Make clean a line
    buff = buff.substr(0, buff.size()-1); // remove the last character
    int pos;
    if((pos=buff.find("<br>")) != string::npos){
      buff.replace(pos, 4, " ");
    }

    stringstream ss;
    ss << buff;
    // Write whole line

    output_file << buff << " @ " << endl;
    string word;
    while( ss >> word ){
      int pos;
      bool checking = true;
      while(checking){
	if((pos = word.find(".")) != string::npos)
	  word.replace(pos, 1, "");
	else if((pos = word.find(",")) != string::npos)
	  word.replace(pos, 1, "");
	else if((pos=word.find("-")) != string::npos){
	  word.replace(pos, 1, "");
	}else if((pos=word.find("l' m")) != string::npos)
	  word.replace(pos, 4, "I'm");
	else if((pos=word.find("you' re")) != string::npos)
	  word.replace(pos, 7, "you're");
	else if((pos=word.find("They' re")) != string::npos)
	  word.replace(pos, 8, "They're");
	else
	  checking = false;
      }
      if((pos = word.find("?")) != string::npos){
	output_file << word.substr(0, word.size()-1) << " : \n";
	output_file << "? : \n";
      }else if((pos = word.find("!")) != string::npos){
	output_file << word.substr(0, word.size()-1) << " : \n";
	output_file << "! : \n";
      }
      else
	output_file << word << " : \n";
    }
    output_file << "\n";
  }
  input_file.close();
  output_file.close();
  return 0;
}
