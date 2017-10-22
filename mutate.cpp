#include <fstream>
#include <string>
#include <ctime>
#include <iostream>

using namespace std;

int main (int argc, char *argv[]) {
  if (argc != 3) {return -1;}

  char hexDigit[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'}; 

  // read genome text
  string buf;
  bool comment = false;
  string genText;
  ifstream genFile;

  genFile.open(argv[1]);
  while (!genFile.eof()) {
    genFile >> buf;       
                                     
    if (buf.front() == '(' && !comment) {
      comment = true;                    
    }                                    
    if (!comment) {                      
      genText += buf;              
    }                                    
    if (buf.back() == ')' && comment) {  
      comment = false;                   
    }                                    
  }
  genFile.close();
  
  // seed rng
  srand(time(NULL));

  int len = genText.size();
  for (int i = 0; i < len; i++) {
    if (rand() % 1000 == 0) {
      genText[i] = hexDigit[rand() % 16];
    }
  }

  // save genome text to destination file
  ofstream dest;
  dest.open(argv[2]);
  dest << genText;
  dest.close();

  return 0;
}
