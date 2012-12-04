Emotion::Emotion(){
  angry_file = "angry.txt";
  excited_file = "excited.txt";
  happ_file = "happy.txt";
  sad_file = "sad.txt";
  scared_file = "scared.txt";
  tender_file = "tender.txt";
}

void Emotion::getEmotionWords(){
  std::ifstream fp;
  std::string buf;
  fp.open(angry_file.c_str());
  fp.getline(fp, buf);
  
  FILE *fl;
  char s[30];
  fopen("happy.txt", "r")
}
