#include "Genome.hpp"
#include <fstream>
#include <string>
#include <random>
#include <iostream>

using namespace std;

static const int MUTATION_CHANCE = 1;
static const char HEX_LETTERS[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

int main (int argc, char *argv[]) {
  bool force = false;
  if (argc != 1) {
    if (argc == 2) {
      string a = argv[1];
      if (a != "-f") { return 1; }
      force = true;
    }
    else {
      return 1;
    }
  }

  string buf;
  string genome;
  while (!cin.eof()) {
    getline(cin, buf);
    genome += buf + '\n';
  }
  genome.pop_back();
  
  // check for a malformed genome
  Genome g(genome);
  if (!g.isValid()) {
    cout << "ERROR: Invalid genome. Error at index " << g.getErr() << ".";
    return 1;
  }

  string mutantGenome;
  bool validGenome = false;
  random_device r;
  default_random_engine engine(r());
  uniform_int_distribution<int> mut_dist(0, 999);
  uniform_int_distribution<int> hex_dist(0, 15);

  while (!validGenome) {
    mutantGenome = "";
    bool comment = false;
    for (unsigned int i = 0; i < genome.size(); i++) {
      if (!comment) {
        if (genome[i] == '(') {
          comment = true;
          mutantGenome += genome[i];
        }
        else {
          if (genome[i] != ' ' && 
              genome[i] != '\n' && 
              genome[i] != '\r' && 
              mut_dist(engine) < MUTATION_CHANCE) {
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
    if (!force) {
      Genome g(mutantGenome);
      validGenome = g.isValid();
    }
  }

  cout << mutantGenome;
  return 0;
}
