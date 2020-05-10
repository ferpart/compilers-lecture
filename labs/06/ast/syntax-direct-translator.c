#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "stackString.h"

/*
* Grammar
* E -> int|(E)|E+E|E-E|E*E
*
* We can reduce that to
*
* E     -> T E_1
* E_1   -> + T E_1 | ɛ
* T     -> R T_1
* T_1   -> - R T_1 | ɛ
* R     -> F R_1
* R_1   -> * F R_1 | ɛ
* F     -> ( E ) | int
*
*/


int i;
char elem;
char *buffer;

struct stack_t *stack;

bool E();
bool E_1();
bool T();
bool T_1();
bool R();
bool R_1();
bool F();

bool match(char s){
  if (elem == s){
    i++;
    elem = buffer[i];
    return true;
  }
  return false;
}

bool matchInt(){
  if(isdigit(elem)){
    i++;
    elem = buffer[i];
    if(isdigit(elem)){
      return false;
    }
    return true;
  }
  return false;
}

// E -> T E_1
bool E(){
  if (T() && E_1()) {
    push(stack, "E -> T E_1\n");
    return true;
  }
  return false;
}

//E_1 -> + T E_1 | ɛ
bool E_1(){
  if(match('+') && T() && E_1()) {
    push(stack, "E_1 -> + T E_1\n");
    return true;
  }
  return true;
}

//T -> R T_1
bool T(){
  if(R() && T_1()){
    push(stack, "T -> R T_1\n");
    return true;
  }
  return false;
}

//T_1 -> - R T_1 | ɛ
bool T_1(){
  if(match('-') && R() && T_1()){
    push(stack, "T_1 -> - R T_1\n");
    return true;
  }
  return true;
}

//R -> F R_1
bool R(){
  if(F() && R_1()){
    push(stack, "R -> F R_1\n");
    return true;
  }
  return false;
}

//R_1 -> * F R_1 | ɛ
bool R_1(){
  if(match('*') && F() && R_1){
    push(stack, "R_1 -> F R_1\n");
    return true;
  }
  return true;

}

//F -> ( E ) | int
bool F(){
  if(match('(') && E() && match(')')){
    push(stack, "F -> ( E )\n");
    return true;
  }
  if(matchInt()){
    push(stack, "F -> int\n");
    return true;
  }
  return false;
}

void trim(char *s){
    const char* d = s;
    do {
        while (*d == ' ') {
            ++d;
        }
    } while (*s++ = *d++);
}

/* int main() 
{ 
    struct Stack* stack = createStack(100); 
  
    push(stack, 10); 
    push(stack, 20); 
    push(stack, 30); 
  
    printf("%d popped from stack\n", pop(stack)); 
  
    return 0; 
}  */

int main(int argc, char const *argv[])
{
  int size = 1024, pos;
  int c;
  buffer = (char *)malloc(size);

  if (argc != 2){
      printf("How to run:\n \t./sintax-direct-translator <file name>\n");
      return 1;
  }

  FILE *f = fopen(argv[1], "r");
  if(f) {
    do { // read all lines in file
      pos = 0;
      do{ // read one line
        c = fgetc(f);
        if(c != EOF) buffer[pos++] = (char)c;
        if(pos >= size - 1) { // increase buffer length - leave room for 0
          size *=2;
          buffer = (char*)realloc(buffer, size);
        }
      }while(c != EOF && c != '\n');
      buffer[pos] = 0;
      // line is now in buffer
      i = 0;
      trim(buffer);
      elem = buffer[i];
      stack = newStack();
      do{
        if (buffer[strlen(buffer)-1] == '\n'){
          buffer[strlen(buffer)-1] = 0;
        }      
        if(E()){
          printf("%s: input valid\n", buffer);
          printf("Path taken:\n");
          while (!isEmpty(stack))
          {
            printf("%s", top(stack));
            pop(stack);
          }
          printf("\n\n");
          break;
        }
        else{
          printf("%s: input not valid\n\n", buffer);
          break;
        }
      } while (elem != EOF && elem != '\n');

    } while(c != EOF); 
      fclose(f);           
    }
    free(buffer);
    
  return 0;
}
