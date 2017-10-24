#include "Genome.hpp"
#include <fstream>
#include <string>
#include <random>
#include <iostream>

using namespace std;

static const int MUTATION_CHANCE = 3;
static const char HEX_LETTERS[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

int main (int argc, char *argv[]) {
  if (argc != 3) { return 1; }

  char *genomeFileName = argv[1];
  char *mutantFileName = argv[2];

  ifstream genomeFile(genomeFileName);
  if (!genomeFile.is_open()) { return 2; }
  
  char buf;
  string genome;
  while (genomeFile.get(buf)) {
    genome += buf;
  }

  // check for a malformed genome
  new Genome(genome);

  string mutantGenome;
  bool validGenome = false;
  random_device r;
  default_random_engine engine(r());
  uniform_int_distribution<int> mut_dist(0, 99);
  uniform_int_distribution<int> hex_dist(0, 15);

  while (!validGenome) {
    mutantGenome = "";
    bool comment = false;
    for (int i = 0; i < genome.size(); i++) {
      if (!comment) {
        if (genome[i] == '(') {
          comment = true;
          mutantGenome += genome[i];
        }
        else {
          if (genome[i] != ' ' && genome[i] != '\n' && genome[i] != '\r' && mut_dist(engine) < MUTATION_CHANCE) {
            mutantGenome += HEX_LETTERS[hex_dist(engine)];
          }
          else {
            mutantGenome += genome[i];
          }
        }
      }
      else {
        if (genome[i] == ')') {
          comment = false;
        }
        mutantGenome += genome[i];
      }
    }
    validGenome = true;
    try {
      new Genome(mutantGenome);
    } catch(...) {
      validGenome = false;
    }
  }

  ofstream mutantFile(mutantFileName);
  if (!mutantFile.is_open()) { return 3; }
  mutantFile << mutantGenome;
  mutantFile.close();

  return 0;
}
