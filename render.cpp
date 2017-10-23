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

  cout << "======";
  cout << x << "," << y;
  cout << "height: " << height << " width: " << width;

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
  int width = cell->getWidth();
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
  
  ifstream genomeFile(argv[1]);
  if (!genomeFile.is_open()) { return 2; }
  
  bool comment = false;
  string buf;
  string genome;
  
  while (genomeFile >> buf) {
    if (!comment) {
      if (buf[0] == '(') {
        comment = true;
      }
      else {
        genome += buf;
      }
    }
    else if (comment && buf[buf.size() - 1] == ')') {
      comment = false;
    }
  }

  genomeFile.close();

  ofstream svgFile(argv[2]);
  if (!svgFile.is_open()) { return 3; }

  svgFile << "<svg width=\"1000\" height=\"1000\" xmlns=\"http://www.w3.org/2000/svg\">\n"
          << "<g transform=\"translate(500, 1000) matrix(1, 0, 0, -1, 0, 0)\">\n";

  Plant plant(genome);
  stack<Cell *> undrawnCells;
  undrawnCells.push(plant.getRootCell());
  cout << "++++++++\n";
  while (!undrawnCells.empty()) {
    Cell *curCell = undrawnCells.top();
    undrawnCells.pop();

    cout << "x: " << curCell->getX() << "\n";
    cout << "y: " << curCell->getY() << "\n";

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

  /*
  string g = "04200005245ba10720020A2000030000F200002000a32000220000200001";
  Plant p(g);
  Cell *c = p.getRootCell();

  const vector<Cell *>* children;
  stack<Cell *> cells;
  cells.push(c);

  while (!cells.empty()) {
    c = cells.top();
    cells.pop();
    cout << "Whatupmyboy\n";
    children = c->getChildren();
    vector<Cell *>::const_iterator it = children->begin();
    while (it != children->end()) {
      cout << "heyo\n";
      cells.push(*it);
      it++;
    }
  }
  */
  
}
