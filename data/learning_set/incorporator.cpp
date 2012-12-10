#include <fstream>
#include <string>
#include <stdlib.h>
#include <iostream>
using namespace std;

int main(int argc, char* argv[]){
  if(argc < 4){
    cout << "FORMAT : ./incorporator  output_file  output_mode(1:create, 2:add)  input_file1  input_file2 ...." << endl;
    return 1;
  }
  
  int output_mode = atoi(argv[2]);
  ofstream output_file;
  ifstream input_file;

  if(output_mode == 1)
    output_file.open(argv[1]);    
  else
    output_file.open(argv[1], ios_base::out|ios_base::app);
  
  int input_count = argc - 3;
  for(int i=0; i<input_count; i++){
    input_file.open(argv[i+3]);
    cout << "Input file : " << argv[i+3] << endl;
    string buff;
    while(getline(input_file, buff))
      output_file << buff << endl;

    input_file.close();
  }
  output_file.close();
}
