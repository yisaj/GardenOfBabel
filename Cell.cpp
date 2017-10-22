#include "Cell.hpp"
#include "Plant.hpp"
#include <cmath>
#include <iostream>

#define PI 3.14159265

Cell::Cell(Plant *plant, Cell *parent, Genome *genome, int cellNumber, int budPoint, int direction, int startGene) {
  m_plant = plant;
  m_parent = parent;
  m_genome = genome;
  m_cell_number = cellNumber;
  m_direction = direction;
  m_bud_point = budPoint;
  m_start_gene = startGene;
  m_accumulator[0] = 0;
  m_accumulator[1] = 0;
  m_shape = 0;
  m_line_color = 0x387257;
  m_fill_color = 0x7cffc2;
  m_height = 0;
  m_width = 0;
  cout << cellNumber << " Cell Constructed" << endl;
}

void Cell::grow() {
  cout << m_cell_number << " Cell Growing" << endl;
  if (m_parent == nullptr) {
    m_x = 0;
    m_y = 0;
    m_direction = 0;
  }
  else {
    positionCell(m_bud_point);
  }
  
  processGene(m_start_gene, 0);
  // grow children, if any
  vector<Cell*>::iterator cit = m_children.begin();
  while (cit != m_children.end()) {
    (*cit)->grow();
    cit++;
  }

  return;
}

Cell::~Cell() {
  // TODO: delete all children
  vector<Cell*>::iterator cit = m_children.begin();
  while (cit != m_children.end()) {
    delete *cit;
    cit++;
  }
}

// GETTERS
Cell *Cell::getParent() const {
  return m_parent;
}

const vector<Cell*> *Cell::getChildren() const {
  return &m_children;
}

int Cell::getCellNumber() const {
  return m_cell_number;
}

int Cell::getStartGene() const {
  return m_start_gene;
}

int Cell::getHeight() const {
  return m_height;
}

int Cell::getWidth() const {
  return m_width;
}

double Cell::getX() const {
  return m_x;
}

double Cell::getY() const {
  return m_y;
}

int Cell::getShape() const {
  return m_shape;
}

int Cell::getDirection() const {
  return m_direction;
}

int Cell::getFillColor() const {
  return m_fill_color;
}

int Cell::getLineColor() const {
  return m_line_color;
}

// SETTERS
void Cell::setHeight(int height) {
  m_height = height;
}

void Cell::setWidth(int width) {
  m_width = width;
}

void Cell::setX(double x) {
  m_x = x;
}

void Cell::setY(double y) {
  m_y = y;
}

void Cell::setShape(int shape) {
  m_shape = shape;
}

void Cell::setDirection(int direction) {
  m_direction = direction;
}

void Cell::setFillColor(int fillColor) {
  m_fill_color = fillColor;
}

void Cell::setLineColor(int lineColor) {
  m_line_color = lineColor;
}

// PRIVATE UTILS
int Cell::getArgument(int *index, bool hex) {
  char prefix = m_genome->getChar(*index);
  char ins = m_genome->getChar(*index - 1);
  short s;

  // immediate value
  if (prefix == '2' && hex) {
    cout << "HEXED" << endl;
    *index += 7;
    return stoi(m_genome->getSubstring(*index - 6, 6), 0, 16);
  }
  else if (prefix == '2' && !hex) {
    *index += 5;
    s = stoi(m_genome->getSubstring(*index - 4, 4), 0, 16);
    return s;
  }
  // cell value
  else if (prefix == '3') {
    *index += 5;
    return getAttribute(stoi(m_genome->getSubstring(*index - 4, 4), 0, 16));
  }
  // accumulator 0
  else if (prefix == '0') {
    *index += 1;
    return m_accumulator[0];
  }
  // accumulator 1
  else if (prefix == '1') {
    *index += 1;
    return m_accumulator[1];
  }
  return -1;
}

int Cell::getAttribute(int att) {
  att = att % 7;
  switch (att) {
  case 0:
    return m_cell_number;
  case 1:
    return m_shape;
  case 2:
    return m_line_color;
  case 3:
    return m_fill_color;
  case 4:
    return m_height;
  case 5:
    return m_width;
  case 6:
    return m_direction;
  }
  return -1;
}

void Cell::executeInstruction(int *index, int recursionDepth) {
  int nextGene;
  int arguments[3];
  int argument;
  int times;
  int placeholder;
  int operation;
  int accNum;
  char ins;

  //cout << "index: " << *index << endl;
  if (m_genome->getChar(*index) != '0') {
  cout << "ins: " << m_genome->getChar(*index) << endl;
  }

  switch (m_genome->getChar(*index)) {
    // call to another gene
  case '2':
    ++*index;
    nextGene = getArgument(index) % m_genome->getGeneCount();
    cout << "jumping to gene " << nextGene << endl;
    processGene(nextGene, recursionDepth + 1);
    break;
    // spawn new cell
  case '3':
    ++*index;
    for (int j = 0; j < 3; j++) {
      arguments[j] = getArgument(index);
    }
    cout << "spawning cell at position " << arguments[0] << ", with direction " << arguments[1] << ", and gene " << arguments[2] << endl;
    m_children.push_back(new Cell(m_plant, this, m_genome, m_cell_number + 1, arguments[0], arguments[1], arguments[2]));
    m_plant->incNumCells();
    break;
    // set shape
  case '4':
    ++*index;
    argument = getArgument(index);
    cout << "setting shape to " << argument << endl;
    setShape(argument);                                             
    break;
    // set line color
  case '5':
    ++*index;
    argument = getArgument(index, true);
    cout << "setting line color to " << argument << endl;
    setLineColor(argument);
    break;
    // set fill color
  case '6':
    ++*index;
    argument = getArgument(index, true);
    cout << "setting fill color to " << argument << endl;
    setFillColor(argument);
    break;
    // set height
  case '7':
    ++*index;
    argument = getArgument(index);
    cout << "setting height to " << argument << endl;
    setHeight(argument);
    break;
    // set width
  case '8':
    ++*index;
    argument = getArgument(index);
    cout << "setting width to " << argument << endl;
    setWidth(argument);
    break;
    // set direction
  case '9':
    ++*index;
    argument = getArgument(index);
    cout << "setting direction to " << argument << endl;
    setDirection(argument);
    break;
    // get value and put in accumulator
  case 'a':
  case 'A':
    ++*index;
    accNum = getArgument(index) % 2;
    argument = getArgument(index);
    cout << "setting accumulator " << accNum << " to " << argument << endl;
    m_accumulator[accNum] = argument;
    break;
    // operation accumulator by value
  case 'b':
  case 'B':
    ++*index;
    accNum = getArgument(index) % 2;
    operation = getArgument(index) % 5;
    argument = getArgument(index);
    switch (operation) {
    case 0:
      cout << "adding " << argument << " to accumulator " << accNum << ": " << m_accumulator[accNum] << endl;
      m_accumulator[accNum] += argument;
      break;
    case 1:
      cout << "subtracting " << argument << " from accumulator " << accNum << ": " << m_accumulator[accNum] << endl;
      m_accumulator[accNum] -= argument;
      break;
    case 2:
      cout << "multiplying " << argument << " with accumulator " << accNum << ": " << m_accumulator[accNum] << endl;
      m_accumulator[accNum] *= argument;
      break;
    case 3:
      cout << "dividing " << argument << " from accumulator " << accNum << ": " <<  m_accumulator[accNum] << endl;
      m_accumulator[accNum] /= argument;
      break;
    case 4:
      cout << "moduloing " << argument << " from accumulator " << accNum << ": " << m_accumulator[accNum] << endl;
      m_accumulator[accNum] %= argument;
      break;
    default:
      break;
    }
    cout << "result " << m_accumulator[accNum] << endl;
    break;
    // repeat next instruction
  case 'c':
  case 'C':
    ++*index;                                      
    times = getArgument(index);
    placeholder;
    cout << "repeating instruction " << m_genome->getChar(*index) << " " << times << " times" << endl;
    for (int j = 0; j < times; j++) {
      placeholder = *index;
      executeInstruction(&placeholder, recursionDepth);
    }
    *index = placeholder;
    break;
    // execute if arg less than acc
  case 'd':
  case 'D':
    ++*index;
    accNum = getArgument(index) % 2;
    argument = getArgument(index);
    cout << "checking if " << argument << " is less than accumulator " << accNum << ": " << m_accumulator[accNum] << endl;
    if (argument < m_accumulator[accNum]) {
      cout << "result true" << endl;
      executeInstruction(index, recursionDepth);
    }                           
    else {
      cout << "result false" << endl;
      ins = m_genome->getChar(*index);
      ++*index;
      getArgument(index);
      if (ins == 'a' || ins == 'A') {
        cout << "skipping instruction A" << endl;
        getArgument(index);
      }
      else if (ins == '3' || ins == 'b' || ins == 'B') {
        cout << "skipping instruction " << ins << endl;
        getArgument(index);
        getArgument(index);
      }
      else {cout << "skipping single operand instruction" << endl;}
    }
    break;
    // execute if arg equal to acc
  case 'e':
  case 'E':
    ++*index;                                 
    accNum = getArgument(index) % 2;
    argument = getArgument(index);
    cout << "checking if " << argument << " is equal to accumulator " << accNum << ": " << m_accumulator[accNum] << endl;
    if (argument == m_accumulator[accNum]) {       
      cout << "result true" << endl;
      executeInstruction(index, recursionDepth);                   
    }                                              
    else {                          
      cout << "result false" << endl;               
      ins = m_genome->getChar(*index);        
      ++*index;                                    
      getArgument(index);
      if (ins == 'a' || ins == 'A') {
        cout << "skipping instruction A" << endl;
        getArgument(index);                        
      }                  
      else if (ins == '3' || ins == 'b' || ins == 'B') {
        cout << "skipping instruction " << ins << endl;
        getArgument(index);
        getArgument(index);
      }
      else {cout << "skipping single operand instruction" << endl;}
    }                                              
    break;                                                
    // execute if arg above acc
  case 'f':
  case 'F':
    ++*index;                      
    accNum = getArgument(index);
    argument = getArgument(index);
    cout << "checking if " << argument << " is greater than accumulator " << accNum << ": " << m_accumulator[accNum] << endl;
    if (argument > m_accumulator[accNum]) {       
      cout << "result true" << endl;
      executeInstruction(index, recursionDepth);                   
    }                                              
    else {                          
      cout << "result false" << endl;               
      ins = m_genome->getChar(*index);        
      ++*index;                                    
      getArgument(index);                          
      if (ins == 'a' || ins == 'A') {
        cout << "skipping instruction A" << endl;
        getArgument(index);                        
      }   
      else if (ins == '3' || ins == 'b' || ins == 'B') {
        cout << "skipping instruction " << ins << endl;
        getArgument(index);
        getArgument(index);
      }
      else {cout << "skipping single operand instruction" << endl;}
    }                                              
    break;                                                                                            
  default:
    break;
  }
}


void Cell::positionCell(int budPoint) {
  double parentX = m_parent->getX();
  double parentY = m_parent->getY();
  int parentDir = m_parent->getDirection();
  int parentHeight = m_parent->getHeight();
  int parentWidth = m_parent->getWidth();
  int parentShape = m_parent->getShape() % 5;

  double theta = parentDir * PI / 180.0;
  double sint = sin(theta);
  double cost = cos(theta);

  double avgY;
  double height;
  double w;
  double h;
  double cos36;
  double sin36;

  switch (parentShape) {
    // line; height is length
  case 0:
    budPoint = budPoint % 3;
    
    // origin
    if (budPoint == 0) {
      m_x = parentX;
      m_y = parentY;
    }
    // middle
    else if (budPoint == 1) {
      m_x = parentX + parentHeight / 2 * sint;
      m_y = parentY + parentHeight / 2 * cost;
    }
    // end
    else if (budPoint == 2) {
      m_x = parentX + parentHeight * sint;
      m_y = parentY + parentHeight * cost;
    }
    break;
    // isosceles triangle; width is top side length, height is side side length
  case 1:
    budPoint = budPoint % 4;
    height = sqrt(pow(parentHeight, 2) - pow(parentWidth, 2));
    
    if (budPoint == 0) {
      m_x = parentX;
      m_y = parentY;
    }
    // center
    else if (budPoint == 1) {
      avgY = height * 2 / 3;
      
      m_x = parentX + avgY * sint;
      m_y = parentY + avgY * cost;
    }
    // top left vertex
    else if (budPoint == 2) {
      m_x = parentX + height * sint - parentWidth / 2 * cost;
      m_y = parentY + height * cost + parentWidth / 2 * sint;
    }
    // top right vertex
    else if (budPoint == 3) {
      m_x = parentX + height * sint + parentWidth / 2 * cost;
      m_y = parentY + height * cost - parentWidth / 2 * sint;
    }
    break;
    // rhombus; width is horizontal axis, height is vertical axis
  case 2:
    budPoint = budPoint % 5;

    if (budPoint == 0) {
      m_x = parentX;
      m_y = parentY;
    }
    // center
    else if (budPoint == 1) {
      m_x = parentX + parentHeight / 2 * sint;
      m_y = parentY + parentHeight / 2 * cost;
    }
    // left vertex
    else if (budPoint == 2) {
      m_x = parentX + parentHeight / 2 * sint - parentWidth / 2 * cost;
      m_y = parentY + parentHeight / 2 * cost + parentWidth / 2 * sint;
    }
    // top vertex
    else if (budPoint == 3) {
      m_x = parentX + parentHeight * sint;
      m_y = parentY + parentHeight * cost;
    }
    // right vertex
    else if (budPoint == 4) {
      m_x = parentX + parentHeight / 2 * sint + parentWidth / 2 * cost;
      m_y = parentY + parentHeight / 2 * cost - parentWidth / 2 * sint;
    }
    break;
    // pentagon; height is radius length
  case 3:
    budPoint = budPoint % 6;
    
    // center
    if (budPoint == 0) {
      m_x = parentX;
      m_y = parentY;
    }
    // 12 o clock vertex
    else if (budPoint == 1) {
      m_x = parentX + parentHeight * sint;
      m_y = parentY + parentHeight * cost;
    }
    // 2 o clock vertex
    else if (budPoint == 2) {
      m_x = parentX + parentHeight * sin((parentDir + 72) * PI / 180.0);
      m_y = parentY + parentHeight * cos((parentDir + 72) * PI / 180.0);
    }
    // 5 o clock vertex
    else if (budPoint == 3) {
      m_x = parentX + parentHeight * sin((parentDir + 144) * PI / 180.0);
      m_y = parentY + parentHeight * cos((parentDir + 144) * PI / 180.0);
    }
    // 7 o clock vertex
    else if (budPoint == 4) {
      m_x = parentX + parentHeight * sin((parentDir + 216) * PI / 180.0);
      m_y = parentY + parentHeight * cos((parentDir + 216) * PI / 180.0);
    }
    // 10 o clock vertex
    else if (budPoint == 5) {
      m_x = parentX + parentHeight * sin((parentDir + 288) * PI / 180.0);
      m_y = parentY + parentHeight * cos((parentDir + 288) * PI / 180.0);
    }
    break;
    // ellipse; width is horizontal axis, height is vertical axis
  case 4:
    budPoint = budPoint % 361;
    // center
    if (budPoint == 0) {
      m_x = parentX;
      m_y = parentY;
    }
    // edge
    else {
      budPoint = -(budPoint - 90);
      w = parentWidth * cos(budPoint * PI / 180.0);
      h = parentHeight * sin(budPoint * PI / 180.0);

      m_x = parentX + h * sint + w * cost;
      m_y = parentY + h * cost - w * sint;
    }
    break;
  default:
    break;
  }

  return;
}

void Cell::processGene(int geneNum, int recursionDepth) {
  if (recursionDepth > 3000) {return;}
  
  int i = m_genome->getGeneStart(geneNum) + 1;
  while (m_genome->getChar(i) != '1') {
    executeInstruction(&i, recursionDepth);
    //cout << "geneNum: " << geneNum << endl;
    //cout << "shape: " << m_shape << endl;  
  }
}
