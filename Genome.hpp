#ifndef GENOME_H
#define GENOME_H

#include <string>
#include <vector>

using namespace std;

class Genome {
 public:
  Genome(string text);
  int getGeneCount() const;
  int getGeneStart(int geneNum) const;
  int getGeneEnd(int geneNum) const;
  char getChar(int index) const;
  string getSubstring(int pos, int count) const;
 private:
  string m_text;
  int m_geneCount;
  vector<int> m_gene_starts;
  vector<int> m_gene_ends;

  int processGeneIndices(string text);
};

#endif
