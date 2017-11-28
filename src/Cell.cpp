#include "Cell.hpp"
#include "Plant.hpp"
#include <cmath>
#include <iostream>

static const int MAX_NUM_CELLS = 200;
static const int MAX_RECURSION_DEPTH = 50;
static const int MAX_NUM_INSTRUCTIONS = 300;
static const double PI = 3.14159265;

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
  m_num_instructions = 0;
  cerr << cellNumber << " Cell Constructed" << endl;
}

void Cell::grow() {
  if (m_plant->getNumCells() > MAX_NUM_CELLS) { return; }
  cerr << m_cell_number << " Cell Growing" << endl;
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

int Cell::getNumInstructions() const {
  return m_num_instructions;
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

void Cell::incNumInstructions() {
  m_num_instructions++;
}

// PRIVATE UTILS
int Cell::getArgument(int *index, bool hex) {
  char prefix = m_genome->getChar(*index);

  // immediate value
  if (prefix == '2' && hex) {
    cerr << "HEXED" << endl;
    *index += 7;
    return stoi(m_genome->getSubstring(*index - 6, 6), 0, 16);
  }
  else if (prefix == '2' && !hex) {
    *index += 5;
    return stoi(m_genome->getSubstring(*index - 4, 4), 0, 16);
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

  //cerr << "index: " << *index << endl;
  if (m_genome->getChar(*index) != '0') {
  cerr << "ins: " << m_genome->getChar(*index) << endl;
  }

  switch (m_genome->getChar(*index)) {
    // call to another gene
  case '2':
    ++*index;
    nextGene = getArgument(index) % m_genome->getGeneCount();
    if (nextGene < 0) { nextGene = -nextGene; }
    cerr << "jumping to gene " << nextGene << endl;
    processGene(nextGene, recursionDepth + 1);
    break;
    // spawn new cell
  case '3':
    ++*index;
    for (int j = 0; j < 3; j++) {
      arguments[j] = getArgument(index);
    }
    arguments[2] = arguments[2] % m_genome->getGeneCount();
    cerr << "spawning cell at position " << arguments[0] << ", with direction " << arguments[1] << ", and gene " << arguments[2] << endl;
    m_children.push_back(new Cell(m_plant, this, m_genome, m_cell_number + 1, arguments[0], arguments[1], arguments[2]));
    m_plant->incNumCells();
    break;
    // set shape
  case '4':
    ++*index;
    argument = getArgument(index);
    cerr << "setting shape to " << argument << endl;
    setShape(argument);                                             
    break;
    // set line color
  case '5':
    ++*index;
    argument = getArgument(index, true);
    cerr << "setting line color to " << argument << endl;
    setLineColor(argument);
    break;
    // set fill color
  case '6':
    ++*index;
    argument = getArgument(index, true);
    cerr << "setting fill color to " << argument << endl;
    setFillColor(argument);
    break;
    // set height
  case '7':
    ++*index;
    argument = getArgument(index);
    cerr << "setting height to " << argument << endl;
    setHeight(argument);
    break;
    // set width
  case '8':
    ++*index;
    argument = getArgument(index);
    cerr << "setting width to " << argument << endl;
    setWidth(argument);
    break;
    // set direction
  case '9':
    ++*index;
    argument = getArgument(index);
    cerr << "setting direction to " << argument << endl;
    setDirection(argument);
    break;
    // get value and put in accumulator
  case 'a':
  case 'A':
    ++*index;
    accNum = getArgument(index) % 2;
    argument = getArgument(index);
    cerr << "setting accumulator " << accNum << " to " << argument << endl;
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
      cerr << "adding " << argument << " to accumulator " << accNum << ": " << m_accumulator[accNum] << endl;
      m_accumulator[accNum] += argument;
      break;
    case 1:
      cerr << "subtracting " << argument << " from accumulator " << accNum << ": " << m_accumulator[accNum] << endl;
      m_accumulator[accNum] -= argument;
      break;
    case 2:
      cerr << "multiplying " << argument << " with accumulator " << accNum << ": " << m_accumulator[accNum] << endl;
      m_accumulator[accNum] *= argument;
      break;
    case 3:
      cerr << "dividing " << argument << " from accumulator " << accNum << ": " <<  m_accumulator[accNum] << endl;
      m_accumulator[accNum] /= argument;
      break;
    case 4:
      cerr << "moduloing " << argument << " from accumulator " << accNum << ": " << m_accumulator[accNum] << endl;
      m_accumulator[accNum] %= argument;
      break;
    default:
      break;
    }
    cerr << "result " << m_accumulator[accNum] << endl;
    break;
    // repeat next instruction
  case 'c':
  case 'C':
    ++*index;                                      
    times = getArgument(index);
    cerr << "repeating instruction " << m_genome->getChar(*index) << " " << times << " times" << endl;
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
    cerr << "checking if " << argument << " is less than accumulator " << accNum << ": " << m_accumulator[accNum] << endl;
    if (argument < m_accumulator[accNum]) {
      cerr << "result true" << endl;
      executeInstruction(index, recursionDepth);
    }                           
    else {
      cerr << "result false" << endl;
      ins = m_genome->getChar(*index);
      ++*index;
      getArgument(index);
      if (ins == 'a' || ins == 'A') {
        cerr << "skipping instruction A" << endl;
        getArgument(index);
      }
      else if (ins == '3' || ins == 'b' || ins == 'B') {
        cerr << "skipping instruction " << ins << endl;
        getArgument(index);
        getArgument(index);
      }
      else {cerr << "skipping single operand instruction" << endl;}
    }
    break;
    // execute if arg equal to acc
  case 'e':
  case 'E':
    ++*index;                                 
    accNum = getArgument(index) % 2;
    argument = getArgument(index);
    cerr << "checking if " << argument << " is equal to accumulator " << accNum << ": " << m_accumulator[accNum] << endl;
    if (argument == m_accumulator[accNum]) {       
      cerr << "result true" << endl;
      executeInstruction(index, recursionDepth);                   
    }                                              
    else {                          
      cerr << "result false" << endl;               
      ins = m_genome->getChar(*index);        
      ++*index;                                    
      getArgument(index);
      if (ins == 'a' || ins == 'A') {
        cerr << "skipping instruction A" << endl;
        getArgument(index);                        
      }                  
      else if (ins == '3' || ins == 'b' || ins == 'B') {
        cerr << "skipping instruction " << ins << endl;
        getArgument(index);
        getArgument(index);
      }
      else {cerr << "skipping single operand instruction" << endl;}
    }                                              
    break;                                                
    // execute if arg above acc
  case 'f':
  case 'F':
    ++*index;                      
    accNum = getArgument(index);
    argument = getArgument(index);
    cerr << "checking if " << argument << " is greater than accumulator " << accNum << ": " << m_accumulator[accNum] << endl;
    if (argument > m_accumulator[accNum]) {       
      cerr << "result true" << endl;
      executeInstruction(index, recursionDepth);                   
    }                                              
    else {                          
      cerr << "result false" << endl;               
      ins = m_genome->getChar(*index);        
      ++*index;                                    
      getArgument(index);                          
      if (ins == 'a' || ins == 'A') {
        cerr << "skipping instruction A" << endl;
        getArgument(index);                        
      }   
      else if (ins == '3' || ins == 'b' || ins == 'B') {
        cerr << "skipping instruction " << ins << endl;
        getArgument(index);
        getArgument(index);
      }
      else {cerr << "skipping single operand instruction" << endl;}
    }                                              
    break;                                                                                            
  default:
    break;
  }

  incNumInstructions();
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
  double w;
  double h;

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
    // isosceles triangle; width is top side length, height is center length
  case 1:
    budPoint = budPoint % 4;
    
    if (budPoint == 0) {
      m_x = parentX;
      m_y = parentY;
    }
    // center
    else if (budPoint == 1) {
      avgY = parentHeight * 2 / 3;
      
      m_x = parentX + avgY * sint;
      m_y = parentY + avgY * cost;
    }
    // top left vertex
    else if (budPoint == 2) {
      m_x = parentX + parentHeight * sint - parentWidth / 2.0 * cost;
      m_y = parentY + parentHeight * cost + parentWidth / 2.0 * sint;
    }
    // top right vertex
    else if (budPoint == 3) {
      m_x = parentX + parentHeight * sint + parentWidth / 2.0 * cost;
      m_y = parentY + parentHeight * cost - parentWidth / 2.0 * sint;
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
    // pentagon; height is perpendicular from base to point
  case 3:
    budPoint = budPoint % 6;    

    // 6 o clock vertex
    if (budPoint == 0) {
      m_x = parentX;
      m_y = parentY;
    }
    // center
    else if (budPoint == 1) {      
      m_x = parentX + parentHeight * (1.0 - tan(54 / 180.0 * PI) / tan(72 / 180.0 * PI)) * sint;
      m_y = parentY + parentHeight * (1.0 - tan(54 / 180.0 * PI) / tan(72 / 180.0 * PI)) * cost;
    }
    // 8 o clock vertex
    else if (budPoint == 2) {
      m_x = parentX + 2 * parentHeight / tan(72 / 180.0 * PI) * sin((parentDir + 306) * PI / 180.0);
      m_y = parentY + 2 * parentHeight / tan(72 / 180.0 * PI) * cos((parentDir + 306) * PI / 180.0);
    }
    // 11 o clock vertex
    else if (budPoint == 3) {
      m_x = parentX + parentHeight / cos(18 / 180.0 * PI) * sin((parentDir + 342) * PI / 180.0);
      m_y = parentY + parentHeight / cos(18 / 180.0 * PI) * cos((parentDir + 342) * PI / 180.0);
    }
    // 1 o clock vertex
    else if (budPoint == 4) {
      m_x = parentX + parentHeight / cos(18 / 180.0 * PI) * sin((parentDir + 18) * PI / 180.0);
      m_y = parentY + parentHeight / cos(18 / 180.0 * PI) * cos((parentDir + 18) * PI / 180.0);
    }
    // 4 o clock vertex
    else if (budPoint == 5) {
      m_x = parentX + 2 * parentHeight / tan(72 / 180.0 * PI) * sin((parentDir + 54) * PI / 180.0);
      m_y = parentY + 2 * parentHeight / tan(72 / 180.0 * PI) * cos((parentDir + 54) * PI / 180.0);
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
      w = parentWidth / 2.0 * cos(budPoint * PI / 180.0);
      h = parentHeight / 2.0 * sin(budPoint * PI / 180.0);

      m_x = parentX + h * sint + w * cost;
      m_y = parentY + h * cost - w * sint;
    }
    break;
  default:
    break;
  }

  return;
}

void Cell::processGene(unsigned int geneNum, int recursionDepth) {
  if (recursionDepth > MAX_RECURSION_DEPTH) {return;}
  
  int i = m_genome->getGeneStart(geneNum) + 1;
  while (m_genome->getChar(i) != '1' && getNumInstructions() < MAX_NUM_INSTRUCTIONS) {
    executeInstruction(&i, recursionDepth);
    //cerr << "geneNum: " << geneNum << endl;
    //cerr << "shape: " << m_shape << endl;  
  }
}
