#include "Plant.hpp"
#include "Genome.hpp"
#include "Cell.hpp"
#include <string>

using namespace std;

Plant::Plant(string genome) {
  m_genome = new Genome(genome);
  m_err = m_genome->getErr();
  if (m_err != -1) { return; }
  m_root_cell = new Cell(this, nullptr, m_genome, 0, 0 , 0, 0);
  m_num_cells = 1;
  m_root_cell->grow();
}

Plant::~Plant() {
  if (isValid()) {
    delete m_root_cell;
  }
  delete m_genome;
}

Cell *Plant::getRootCell() const {
  return m_root_cell;
}

Genome *Plant::getGenome() const {
  return m_genome;
}

unsigned int Plant::getNumCells() const {
  return m_num_cells;
}

void Plant::incNumCells() {
  m_num_cells++;
  return;
}

bool Plant::isValid() const {
  return m_err == -1;
}

int Plant::getErr() const {
  return m_err;
}
