#include "Genome.hpp"
#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>

using namespace std;

Genome::Genome(string text) {
  bool comment = false;
  for (int i = 0; i < text.size(); i++) {
    if (!comment) {
      if (text[i] == '(') {
        comment = true;
      }
      else if (text[i] != ' ' && text[i] != '\n' && text[i] != '\r') {
        m_text += text[i];
      }
    }
    else if (comment && text[i] == ')') {
      comment = false;
    }
  }
  
  int e = processGeneIndices(m_text);
  if (e != -1) {
    throw runtime_error("malformed genome at " + to_string(e) + " around " + m_text.substr(e, 7));
  }
  m_geneCount = m_gene_starts.size();
}

int Genome::getGeneStart(int geneNum) const {
  return m_gene_starts[geneNum];
}

int Genome::getGeneEnd(int geneNum) const {
  return m_gene_ends[geneNum];
}

char Genome::getChar(int index) const {
  return m_text[index];
}

string Genome::getSubstring(int pos, int count) const {
  return m_text.substr(pos, count);
}

int Genome::getGeneCount() const {
  return m_geneCount;
}

int Genome::processGeneIndices(string text) {
  int len = text.length();
  int i = 0;
  
  // check for invalid characters
  while (i < len) {
    if (text[i] == '0' ||
        text[i] == '1' ||
        text[i] == '2' ||
        text[i] == '3' ||
        text[i] == '4' ||
        text[i] == '5' ||
        text[i] == '6' ||
        text[i] == '7' ||
        text[i] == '8' ||
        text[i] == '9' ||
        text[i] == 'a' ||
        text[i] == 'A' ||
        text[i] == 'b' ||
        text[i] == 'B' ||
        text[i] == 'c' ||
        text[i] == 'C' ||
        text[i] == 'd' ||
        text[i] == 'D' ||
        text[i] == 'e' ||
        text[i] == 'E' ||
        text[i] == 'f' ||
        text[i] == 'F') { 
    }
    else {return i;}
    i++;
  }
  i = 0;
  while (i < len) {
    if (text[i] == '0') {
      // detect beginning of new gene
      m_gene_starts.push_back(i);
      i++;
      if (i >= len) {return i;}

      while (text[i] != '1' && i < len) {
        switch (text[i]) {
          // cannot start a gene within a gene 
        case '0':
          return i;
          break;
          // all instructions that take one word operand
        case '2':
        case '4':
        case '7':
        case '8':
        case '9':
          i++;
          if (i >= len) {return i;}

          if (text[i] == '2' || text[i] == '3') {
            i += 5;
          }
          else if (text[i] == '0' || text[i] == '1') {
            i += 1;
          }
          else {
            return i;
          }

          if (i >= len) {return i;}
          break;
          // repeat and conditional execute instructions
        case 'c':
        case 'C':
          i++;                                        
          if (i >= len) {return i;}                   
              
          if (text[i] == '2' || text[i] == '3') {     
            i += 5;                                   
          }                                           
          else if (text[i] == '0' || text[i] == '1') {
            i += 1;                                   
          }                                           
          else {                                      
            return i;                                 
          }                                           
          
          if (i >= len) {return i;}                 
                                            
          if (text[i] == '1' ||                       
              text[i] == 'c' ||                       
              text[i] == 'C' ||                       
              text[i] == 'd' ||                       
              text[i] == 'D' ||                       
              text[i] == 'e' ||                       
              text[i] == 'E' ||                       
              text[i] == 'f' ||                       
              text[i] == 'F') {                       
            return i;                                 
          }                                           
          break;                                      
        case 'd':
        case 'D':
        case 'e':
        case 'E':
        case 'f':
        case 'F':
          i++;
          if (i >= len) {return i;}

          for (int j = 0; j < 2; j++) {
            if (text[i] == '2' || text[i] == '3') {     
              i += 5;                                   
            }                                           
            else if (text[i] == '0' || text[i] == '1') {
              i += 1;                                   
            }                                           
            else {                                      
              return i;                                 
            }                                           

            if (i >= len) {return i;}
          }

          if (text[i] == '1' ||
              text[i] == 'c' ||
              text[i] == 'C' ||
              text[i] == 'd' ||
              text[i] == 'D' ||
              text[i] == 'e' ||
              text[i] == 'E' ||
              text[i] == 'f' ||
              text[i] == 'F') {
            return i;
          }
          break;
          // three operand instructions
        case '3':
        case 'b':
        case 'B':
          i++;
          if (i >= len) {return i;}

          for (int j = 0; j < 3; j++) {
            if (text[i] == '2' || text[i] == '3') {     
              i += 5;                                   
            }                                           
            else if (text[i] == '0' || text[i] == '1') {
              i += 1;                                   
            }                                           
            else {                                      
              return i;                                 
            }                                           
                          
            if (i >= len) {return i;} 
          }
          break;
          // two operand operation instruction
        case 'a':
        case 'A':
          i++;
          if (i >= len) {return i;}

          for (int j = 0; j < 2; j++) {
            if (text[i] == '2' || text[i] == '3') {     
              i += 5;                                   
            }                                           
            else if (text[i] == '0' || text[i] == '1') {
              i += 1;                                   
            }                                           
            else {                                      
              return i;                                 
            }                                           

            if (i >= len) {return i;}
          }
          break;
          // 6 digit color instructions
        case '5':
        case '6':
          i++;
          if (i >= len) {return i;}
          
          if (text[i] == '2') {     
            i += 7;                                   
          }
          else if (text[i] == '3') {
            i += 5;
          }
          else if (text[i] == '0' || text[i] == '1') {
            i += 1;                                   
          }                                           
          else {                                      
            return i;                                 
          }                                           

          if (i >= len) {return i;}
          break;
        default:
          break;
        }
      }
      // detct end of gene
      if (text[i] == '1') {
        m_gene_ends.push_back(i + 1);
      }
      // final gene must be properly closed
      else {
        return i;
      }
    }
    i++;
  }
  return -1;
}
