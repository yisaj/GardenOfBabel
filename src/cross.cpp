#include <random>
#include <string>
#include <fstream>
#include <iostream>
#include "Genome.hpp"

using namespace std;

int main(int argc, char* argv[]) {
  if (argc != 4) {
    if (argc == 2) {
      string a = argv[1];
      if (a != "-s") { return 1; }
    }
    else {
      return 1;
    }
  }

  string buf;
  string parentOneGenome;
  string parentTwoGenome;

  string parentOneFileName;
  string parentTwoFileName;
  string childFileName;
  ifstream parentOneFile;
  ifstream parentTwoFile;
  if (argc == 4) {
    parentOneFileName = argv[1];
    parentTwoFileName = argv[2];
    childFileName = argv[3];

    parentOneFile.open(parentOneFileName);
    if (!parentOneFile.is_open()) { return 2; }
    while (parentOneFile >> buf) {
      parentOneGenome += buf;
    }
    parentOneFile.close();

    parentTwoFile.open(parentTwoFileName);
    if (!parentTwoFile.is_open()) { return 3; }
    while (parentTwoFile >> buf) {
      parentTwoGenome += buf;
    }
    parentTwoFile.close();
  }
  else if (argc == 2) {
    while (!cin.eof()) {
      if (cin >> buf) {
        if (buf == "$") {
          break;
        }
        parentOneGenome += buf;
      }
    }
    while (!cin.eof()) {
      if (cin >> buf) {
        parentTwoGenome += buf;
      }
    }
  }
  
  cerr << "%%%%" << endl << parentOneGenome << endl << "%%%%%%%" << endl << parentTwoGenome << endl;

  Genome parentOne(parentOneGenome);
  if (!parentOne.isValid()) {
    cout << "ERROR: Invalid parent one genome. Error at index " << parentOne.getErr() << ".";
    return 1;
  }

  Genome parentTwo(parentTwoGenome);
  if (!parentTwo.isValid()) {
    cout << "ERROR: Invalid parent two genome. Error at index " << parentTwo.getErr() << ".";
  }

  if (parentOne.getGeneCount() != parentTwo.getGeneCount()) { 
    cout << "ERROR: Parent gene count mismatch. Parent one: " 
         << parentOne.getGeneCount() << " genes. Parent two: " 
         << parentTwo.getGeneCount() << " genes.";
    return 4; 
  }

  string childGenome;
  random_device r;
  default_random_engine engine(r());
  uniform_int_distribution<int> dist(0, 1);

  for (int i = 0; i < parentOne.getGeneCount(); i++) {
    int geneStart;
    int geneEnd;

    if (dist(engine) == 0) {
      geneStart = parentOne.getGeneStart(i);
      geneEnd = parentOne.getGeneEnd(i);

      childGenome += parentOne.getSubstring(geneStart, geneEnd - geneStart);
    }
    else {
      geneStart = parentTwo.getGeneStart(i);
      geneEnd = parentTwo.getGeneEnd(i);

      childGenome += parentTwo.getSubstring(geneStart, geneEnd - geneStart);
    }
  }
  
  if (argc == 4) {
    ofstream childFile(childFileName);
    if (!childFile.is_open()) { return 5; }
    childFile << childGenome;
    childFile.close();
  }
  else if (argc == 2) {
    cout << childGenome;
  }

  return 0;

}
