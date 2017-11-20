#ifndef CELL_H
#define CELL_H

#include "Genome.hpp"
#include "Plant.hpp"
#include <vector>

using namespace std;

class Plant;

class Cell {
 public:
  Cell(Plant *plant, Cell *parent, Genome *genome, int cellNumber, int budPoint, int direction, int startGene);
  ~Cell();
  void grow();

  Cell *getParent() const;
  const vector<Cell*> *getChildren() const;
  int getCellNumber() const;
  int getStartGene() const;
  int getHeight() const;
  int getWidth() const;
  double getX() const;
  double getY() const;
  int getShape() const;
  int getDirection() const;
  int getFillColor() const;
  int getLineColor() const;

  void setHeight(int height);
  void setWidth(int width);
  void setX(double x);
  void setY(double y);
  void setShape(int shape);
  void setDirection(int direction);
  void setFillColor(int fillColor);
  void setLineColor(int lineColor);
 private:
  Plant *m_plant;
  Cell *m_parent;
  Genome *m_genome;
  vector<Cell*> m_children;
  int m_start_gene;
  int m_cell_number;
  int m_bud_point;
  int m_height;
  int m_width;
  double m_x;
  double m_y;
  int m_shape;
  int m_direction;
  int m_fill_color;
  int m_line_color;
  
  int m_accumulator[2];

  unsigned int getArgument(int *index, bool hex = false);
  void executeInstruction(int *index, int recursionDepth);
  int getAttribute(int att);
  void positionCell(int budPoint);
  void processGene(unsigned int geneNum, int recursionDepth);
};

#endif
