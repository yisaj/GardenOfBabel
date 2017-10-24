#include "Plant.hpp"
#include "Cell.hpp"
#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <fstream>
#include <cmath>

using namespace std;

static const double PI = 3.14159265359;
static const int STROKE_WIDTH = 3;

void drawLine(ofstream *svgFile, Cell *cell) {
  double x = cell->getX();
  double y = cell->getY();
  int height = cell->getHeight();
  // int width = cell->getWidth();
  int direction = cell->getDirection();
  int lineColor = cell->getLineColor() & 0x00ffffff;

  *svgFile << "<line x1=\"" << x << "\" y1=\"" << y
           << "\" x2=\"" << x << "\" y2=\"" << (y + height)
           << "\" transform=\"rotate(-" << direction << " " << x << " " << y
           << ")\" stroke=\"#" << hex << lineColor << dec
           << "\" stroke-width=\"" << STROKE_WIDTH << "\"/>\n"; 
}

void drawTriangle(ofstream *svgFile, Cell *cell) {
  double x = cell->getX();
  double y = cell->getY();
  int height = cell->getHeight();
  int width = cell->getWidth();
  int direction = cell->getDirection();
  int lineColor = cell->getLineColor() & 0x00ffffff;
  int fillColor = cell->getFillColor() & 0x00ffffff;

  *svgFile << "<polygon points=\"" << x << "," << y
           << " " << (x + width / 2.0) << "," << (y + height)
           << " " << (x - width / 2.0) << "," << (y + height)
           << "\" transform=\"rotate(-" << direction << " " << x << " " << y
           << ")\" stroke=\"#" << hex << lineColor
           << "\" fill=\"#" << fillColor << dec
           << "\" stroke-width=\"" << STROKE_WIDTH << "\"/>\n";
}

void drawRhombus(ofstream *svgFile, Cell *cell) {
  double x = cell->getX();
  double y = cell->getY();
  int height = cell->getHeight();
  int width = cell->getWidth();
  int direction = cell->getDirection();
  int lineColor = cell->getLineColor() & 0x00ffffff;
  int fillColor = cell->getFillColor() & 0x00ffffff;

  *svgFile << "<polygon points=\"" << x << "," << y
           << " " << (x + width / 2.0) << "," << (y + height / 2.0)
           << " " << x << "," << (y + height)
           << " " << (x - width / 2.0) << "," << (y + height / 2.0)
           << "\" transform=\"rotate(-" << direction << " " << x << " " << y
           << ")\" stroke=\"#" << hex << lineColor
           << "\" fill=\"#" << fillColor << dec
           << "\" stroke-width=\"" << STROKE_WIDTH << "\"/>\n";
}

void drawPentagon(ofstream *svgFile, Cell *cell) {
  double x = cell->getX();
  double y = cell->getY();
  int height = cell->getHeight();
  // int width = cell->getWidth();
  int direction = cell->getDirection();
  int lineColor = cell->getLineColor() & 0x00ffffff;
  int fillColor = cell->getFillColor() & 0x00ffffff;

  double sideLength = 2.0 * height / tan(72 / 180.0 * PI);
  double botY = sideLength * sin(36 / 180.0 * PI);
  double botX = sideLength * cos(36 / 180.0 * PI);

  *svgFile << "<polygon points=\"" << x << "," << y
           << " " << (x + botX) << "," << (y + botY)
           << " " << (x + sideLength / 2.0) << "," << (y + height)
           << " " << (x - sideLength / 2.0) << "," << (y + height)
           << " " << (x - botX) << "," << (y + botY)
           << "\" transform=\"rotate(-" << direction << " " << x << " " << y
           << ")\" stroke=\"#" << hex << lineColor
           << "\" fill=\"#" << fillColor << dec
           << "\" stroke-width=\"" << STROKE_WIDTH << "\"/>\n";
}

void drawEllipse(ofstream *svgFile, Cell *cell) {
  double x = cell->getX();
  double y = cell->getY();
  int height = cell->getHeight();
  int width = cell->getWidth();
  int direction = cell->getDirection();
  int lineColor = cell->getLineColor() & 0x00ffffff;
  int fillColor = cell->getFillColor() & 0x00ffffff;
  
  *svgFile << "<ellipse cx=\"" << x << "\" cy=\"" << y
           << "\" rx=\"" << (width / 2.0) << "\" ry=\"" << (height / 2.0)
           << "\" transform=\"rotate(-" << direction << " " << x << " " << y
           << ")\" stroke=\"#" << hex << lineColor
           << "\" fill=\"#" << fillColor << dec
           << "\" stroke-width=\"" << STROKE_WIDTH << "\"/>\n";
}

int main(int argc, char *argv[]) {
  if (argc != 3) { return 1; }

  char *genomeFileName = argv[1];
  char *svgFileName = argv[2];
  
  ifstream genomeFile(genomeFileName);
  if (!genomeFile.is_open()) { return 2; }
  
  char buf;
  string genome;
  while (genomeFile >> buf) {
    genome += buf;
  }

  genomeFile.close();

  Plant plant(genome);
  stack<Cell *> undrawnCells;
  undrawnCells.push(plant.getRootCell());

  ofstream svgFile(svgFileName);
  if (!svgFile.is_open()) { return 3; }

  svgFile << "<svg width=\"1000\" height=\"1000\" xmlns=\"http://www.w3.org/2000/svg\">\n"
          << "<g transform=\"translate(500, 1000) matrix(1, 0, 0, -1, 0, 0)\">\n";

  while (!undrawnCells.empty()) {
    Cell *curCell = undrawnCells.top();
    undrawnCells.pop();

    switch (curCell->getShape()) {
    case 0:
      drawLine(&svgFile, curCell);
      break;
    case 1:
      drawTriangle(&svgFile, curCell);
      break;
    case 2:
      drawRhombus(&svgFile, curCell);
      break;
    case 3:
      drawPentagon(&svgFile, curCell);
      break;
    case 4:
      drawEllipse(&svgFile, curCell);
      break;
    default:
      break;
    }

    const vector<Cell *> *children = curCell->getChildren();
    vector<Cell *>::const_iterator it = children->begin();
    while (it != children->end()) {
      undrawnCells.push(*it);
      it++;
    }
  }

  svgFile << "</g>\n" << "</svg>";

  svgFile.close();  
  
  return 0;
}
