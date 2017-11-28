#ifndef PLANT_H
#define PLANT_H

#include "Genome.hpp"
#include "Cell.hpp"
#include <string>

using namespace std;

class Cell;

class Plant {
 public:
  Plant(string genome);
  ~Plant();
  Cell *getRootCell() const;
  Genome *getGenome() const;
  unsigned int getNumCells() const;
  void incNumCells();
  bool isValid() const;
  int getErr() const;
  
 private:
  Genome *m_genome;
  Cell *m_root_cell;
  unsigned int m_num_cells;
  int m_err;
};

#endif
