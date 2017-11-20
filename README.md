Instruction reference:  
0 - start gene  
1 - end gene  
2 - jump to gene (calls it like a function)  
3 - spawn new child cell  
    - position on parent  
    - initial direction  
    - starting gene  
4 - set shape  
    - line / triangle / rhombus / pentagon / ellipse  
5 - set line color  
6 - set fill color  
7 - set height  
8 - set width  
9 - set direction  
A - set accumulator  
    - accumulator num  
    - argument  
B - operate on accumulator by argument  
    - accumulator num  
    - add / subtract / multiply / divide / modulo  
    - argument  
C - repeat next instruction  
    - times to repeat  
    - instruction (2 - A)  
D - execute next instruction if argument is less than accumulator  
    - accumulator num  
    - argument  
E - execute next instruction if argument equals accumulator  
F - execute next instruction if argument is more than accumulator  
  
In an argument:  
0 - accumulator 0 value  
1 - accumulator 1 value  
2 - immediate value  
3 - cell value  
    - cellNumber / shape / lineColor / fillColor / height / width / direction  
	