#include <random>
#include <string>
#include <fstream>
#include <iostream>
#include "Genome.hpp"

using namespace std;

int main(int argc, char* argv[]) {
  if (argc != 4) { return 1; }

  string parentOneFileName = argv[1];
  string parentTwoFileName = argv[2];
  string childFileName = argv[3];

  string buf;
  string parentOneGenome;
  ifstream parentOneFile(parentOneFileName);
  if (!parentOneFile.is_open()) { return 2; }
  while (parentOneFile >> buf) {
    parentOneGenome += buf;
  }
  parentOneFile.close();

  string parentTwoGenome;
  ifstream parentTwoFile(parentTwoFileName);
  if (!parentTwoFile.is_open()) { return 3; }
  while (parentTwoFile >> buf) {
    parentTwoGenome += buf;
  }
  parentTwoFile.close();

  Genome parentOne(parentOneGenome);
  Genome parentTwo(parentTwoGenome);

  cout << parentOne.getGeneCount() << " " << parentTwo.getGeneCount() << endl;
  if (parentOne.getGeneCount() != parentTwo.getGeneCount()) { return 4; }

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

  ofstream childFile(childFileName);
  if (!childFile.is_open()) { return 4; }
  childFile << childGenome;
  childFile.close();

  return 0;

}
