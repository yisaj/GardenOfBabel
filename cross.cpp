#include <string>
#include <cstring>
#include <fstream>
#include <ctime>
#include "Genome.hpp"

using namespace std;

int main(int argc, char* argv[]) {
  // three arguments: parent one genome text file, parent two file, destination file
  if (argc != 4) {return -1;}

  // read parent one genome text
  string buf;
  bool comment = false;
  ifstream parentOneFile;
  string parentOneText;

  parentOneFile.open(argv[1]);
  while (!parentOneFile.eof()) {
    parentOneFile >> buf;

    if (buf.front() == '(' && !comment) {
      comment = true;                    
    }                                    
    if (!comment) {                      
      parentOneText += buf;                    
    }                                    
    if (buf.back() == ')' && comment) {  
      comment = false;                   
    }                                    
  }
  parentOneFile.close();

  // read parent two genome text
  comment = false;
  ifstream parentTwoFile;
  string parentTwoText;

  parentTwoFile.open(argv[2]);
  while (!parentTwoFile.eof()) {
    parentTwoFile >> buf;

    if (buf.front() == '(' && !comment) {
      comment = true;                    
    }                                    
    if (!comment) {                      
      parentTwoText += buf;                    
    }                                    
    if (buf.back() == ')' && comment) {  
      comment = false;                   
    }                                    
  }
  parentTwoFile.close();

  // construct plant objects
  Genome parentOneGen = Genome(parentOneText);
  Genome parentTwoGen = Genome(parentTwoText);
  
  if (parentOneGen.getGeneCount() != parentTwoGen.getGeneCount()) {return -1;}

  // seed rng
  srand(time(NULL));

  // randomly choose genes to add to destination text file
  string child;
  int numGenes = parentOneGen.getGeneCount();
  for (int i = 0; i < numGenes; i++) {
    int geneStart;
    int geneEnd;
    int r = rand() % 2;
    
    if (r == 0) {
      geneStart = parentOneGen.getGeneStart(i);
      geneEnd = parentOneGen.getGeneEnd(i);

      child += parentOneGen.getSubstring(geneStart, geneEnd - geneStart);
    }
    else {
      geneStart = parentTwoGen.getGeneStart(i);
      geneEnd = parentTwoGen.getGeneEnd(i);

      child += parentTwoGen.getSubstring(geneStart, geneEnd - geneStart);
    }

    
  }

  // save to destination text file
  ofstream dest;              
  dest.open(argv[3]);
  dest << child;
  dest.close();

  return 0;
}
