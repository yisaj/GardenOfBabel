#include "Plant.hpp"
#include "Cell.hpp"
#include <iostream>
#include <string>
#include <stack>
#include <queue>
#include <vector>
#include <fstream>
#include <cmath>

using namespace std;

static const double PI = 3.14159265359;
static const int STROKE_WIDTH = 8;

void drawLine(Cell *cell) {
  double x = cell->getX();
  double y = cell->getY();
  int height = cell->getHeight();
  //int width = cell->getWidth();
  int direction = cell->getDirection();
  int lineColor = cell->getLineColor() & 0x00ffffff;

  cout << "<line x1=\"" << x << "\" y1=\"" << y
       << "\" x2=\"" << x << "\" y2=\"" << (y + height)
       << "\" transform=\"rotate(" << -direction << " " << x << " " << y
       << ")\" stroke=\"#" << hex << lineColor << dec
       << "\" stroke-width=\"10" 
       << "\" stroke-linecap=\"round\"/>\n"; 
}

void drawTriangle(Cell *cell) {
  double x = cell->getX();
  double y = cell->getY();
  int height = cell->getHeight();
  int width = cell->getWidth();
  int direction = cell->getDirection();
  int lineColor = cell->getLineColor() & 0x00ffffff;
  int fillColor = cell->getFillColor() & 0x00ffffff;

  cout << "<polygon points=\"" << x << "," << y
       << " " << (x + width / 2.0) << "," << (y + height)
       << " " << (x - width / 2.0) << "," << (y + height)
       << "\" transform=\"rotate(" << -direction << " " << x << " " << y
       << ")\" stroke=\"#" << hex << lineColor
       << "\" fill=\"#" << fillColor << dec
       << "\" stroke-width=\"" << STROKE_WIDTH << "\"/>\n";
}

void drawRhombus(Cell *cell) {
  double x = cell->getX();
  double y = cell->getY();
  int height = cell->getHeight();
  int width = cell->getWidth();
  int direction = cell->getDirection();
  int lineColor = cell->getLineColor() & 0x00ffffff;
  int fillColor = cell->getFillColor() & 0x00ffffff;

  cout << "<polygon points=\"" << x << "," << y
       << " " << (x + width / 2.0) << "," << (y + height / 2.0)
       << " " << x << "," << (y + height)
       << " " << (x - width / 2.0) << "," << (y + height / 2.0)
       << "\" transform=\"rotate(" << -direction << " " << x << " " << y
       << ")\" stroke=\"#" << hex << lineColor
       << "\" fill=\"#" << fillColor << dec
       << "\" stroke-width=\"" << STROKE_WIDTH << "\"/>\n";
}

void drawPentagon(Cell *cell) {
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

  cout << "<polygon points=\"" << x << "," << y
           << " " << (x + botX) << "," << (y + botY)
           << " " << (x + sideLength / 2.0) << "," << (y + height)
           << " " << (x - sideLength / 2.0) << "," << (y + height)
           << " " << (x - botX) << "," << (y + botY)
           << "\" transform=\"rotate(" << -direction << " " << x << " " << y
           << ")\" stroke=\"#" << hex << lineColor
           << "\" fill=\"#" << fillColor << dec
           << "\" stroke-width=\"" << STROKE_WIDTH << "\"/>\n";
}

void drawEllipse(Cell *cell) {
  double x = cell->getX();
  double y = cell->getY();
  int height = cell->getHeight();
  int width = cell->getWidth();
  int direction = cell->getDirection();
  int lineColor = cell->getLineColor() & 0x00ffffff;
  int fillColor = cell->getFillColor() & 0x00ffffff;
  
  cout << "<ellipse cx=\"" << x << "\" cy=\"" << y
       << "\" rx=\"" << (width / 2.0) << "\" ry=\"" << (height / 2.0)
       << "\" transform=\"rotate(" << -direction << " " << x << " " << y
       << ")\" stroke=\"#" << hex << lineColor
       << "\" fill=\"#" << fillColor << dec
       << "\" stroke-width=\"" << STROKE_WIDTH << "\"/>\n";
}

int main(int argc, char *argv[]) {
  bool web = false;
  if (argc > 1) {
    string arg = argv[1];
    web = (arg == "-w");
  }

  string buf;
  string genome;
  while (!cin.eof()) {
    cin >> buf;
    genome += buf;
  }
  cerr << genome;
  Plant plant(genome);
  queue<Cell *> undrawnCells;
  undrawnCells.push(plant.getRootCell());

  if (web) {
    cout << "<svg width=\"100%\" height=\"100%\" viewbox=\"0 0 1000 1000\" preserveAspectRatio=\"xMidYMax meet\" xmlns=\"http://www.w3.org/2000/svg\">\n";
  }
  else {
    cout << "<svg width=\"1000\" height=\"1000\" xmlns=\"http://www.w3.org/2000/svg\">\n";
  }
  
  cout << "<g transform=\"translate(500, 1000) matrix(1, 0, 0, -1, 0, 0)\">\n";

  while (!undrawnCells.empty()) {
    Cell *curCell = undrawnCells.front();
    undrawnCells.pop();

    switch (curCell->getShape()) {
    case 0:
      drawLine(curCell);
      break;
    case 1:
      drawTriangle(curCell);
      break;
    case 2:
      drawRhombus(curCell);
      break;
    case 3:
      drawPentagon(curCell);
      break;
    case 4:
      drawEllipse(curCell);
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

  cout << "</g>\n" << "</svg>";

  return 0;
}
