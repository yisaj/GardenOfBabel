You can play around with a live version of this project at [gardenofbabel.com](http://gardenofbabel.com/).

This program implements plant "genomes" as a string of hex characters and provides methods to render them as visual elements. Essentially, each genome is just a list of instructions that set the shape, size, and color of each cell that make up the plant. As a simple hex string these genomes can be generated and manipulated in some interesting ways.

## Programs
#### render [-w]
Reads a plant genome from stdin and sends the resulting SVG format image to stdout. With the optional argument [-w] the SVG image is output in a web-friendly format that allows automatic resizing.

#### mutate [-f]
Reads a plant genome from stdin and sends a mutated copy of the genome to stdout. The program scans the input genome with a MUTATION_CHANCE probability of changing each hex character to a random other character. With the optional argument [-f] the program will return after the first try, without the guarantee of outputting a valid genome.

#### cross
Reads two plant genomes separated by the sentinel character "$" from stdin and sends a child genome to stdout. The parent genomes must have an equal number of genes and the child genome is constructed by randomly between the parents at each gene index.

## Genome Syntax
Gene instructions are single hex characters that take between zero to three arguments. Genome strings are case-insensitive, but non-hex characters are considered invalid. Comments are contained within parentheses "()" and are stripped out before a genome is parsed. Similarly, whitespace has no effect on any output. Hex characters outside of gene delimiters are valid, but have no effect, essentially representing junk DNA.

## Instruction Reference
#### 0:  
Declare the beginning of a gene.  
#### 1:  
Declare the end of a gene.  
#### 2[arg1]: 
Jump to gene with number [arg1].
#### 3[arg1][arg2][arg3]:
Spawn child cell at position [arg1] with direction [arg2] starting at gene [arg3].
#### 4[arg1]:
Set cell shape to [arg1].
#### 5[arg1]:
Set cell outline color to [arg1].
#### 6[arg1]:
Set cell fill color to [arg1].
#### 7[arg1]:
Set cell height to [arg1].
#### 8[arg1]:
Set cell width to [arg1].
#### 9[arg1]:
Set cell direction to [arg1].
#### A[arg1][arg2]:
Set accumulator [arg1] to the value of [arg2].
#### B[arg1][arg2][arg3]:
Perform operation [arg2] on accumulator [arg1] with operand [arg3].
#### C[arg1]:
Repeat next instruction [arg1] times.
#### D[arg1][arg2]:
Execute next instruction if [arg2] is less than accumulator [arg1].
#### E[arg1][arg2]:
Execute next instruction if [arg2] is equal to accumulator [arg1].
#### F[arg1][arg2]:
Execute next instruction if [arg2] is greater than accumulator [arg1].

## Arguments
Arguments can be passed in three ways: as the value of one of the accumulators, as a direct value, or as the value of one the cell's properties.

#### 0:
The current value of accumulator 0.
#### 1:
The current value of accumulator 1.
#### 2XXXX:
The 4-digit hexadecimal value XXXX.
#### 2XXXXXX:
The 6-digit hexadecimal value XXXXXX, used exclusively for instructions 5 and 6, in order to allow standard 6-digit color codes.
#### 3XXXX:
The value of cell property XXXX.

## Cell Properties
#### 0000:
Cell number. Each cell has a cell number one greater than its parent.
#### 0001:
Cell shape.
#### 0002:
Cell outline color.
#### 0003:
Cell fill color.
#### 0004:
Cell height.
#### 0005:
Cell width.
#### 0006:
Cell direction.

## Cell Accumulators and Calculation
Every cell has two accumulators 0 and 1 that help facillitate more complex behavior. Numerical values can be saved to an accumulator and simple arithmetic operations can be performed on these values. Calculations are performed by instruction B when passed the following values for [arg2].

#### 0000:
Add [arg3] to accumulator [arg1].
#### 0001:
Subtract [arg3] from accumulator [arg1].
#### 0002:
Multiply accumulator [arg1] by [arg3].
#### 0003:
Divide accumulator [arg1] by [arg3].
#### 0004:
Modulo accumulator [arg1] by [arg3].
