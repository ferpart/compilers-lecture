#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*
* Grammar
* S -> aBc
* B -> bc | b
*/

struct Elems{
    char* parent;
    char* child;
};

struct Elems elems[3];

void graphAST(char *line){
    printf("\n%s dot representation would be the following: \n\n", line);
    printf("diagram tree {\n\n");
    printf("\t%s -> %s;\n", line, elems[0].parent);
    printf("\t%s -> %s;\n", line, elems[1].parent);
    printf("\t%s -> %s;\n", line, elems[2].parent);
    printf("\t%s -> %s;\n\n}\n", elems[1].parent, elems[1].child);   
}

bool B(char *line){
    if (line[1] == 'b' && line[2] == 'c' && strlen(line)>3){
        elems[1].child = "bc";
        return true; 
    }else if (line[1]=='b'){
        elems[1].child = "b";
        return true;
    }
    return false;
}

bool S(char *line){

    strtok(line, "\n");
    int lineLength = strlen(line)-1;
    if (line[0] == 'a' && line[lineLength] == 'c' && lineLength < 5 && B(line)){
        elems[0].parent = "a";
        elems[1].parent = "B";
        elems[2].parent = "c";
        return true;
    }
    return false;
    
}

int main(int argc, char *argv[]) {
    struct Elems elems[3];
    int size = 1024, pos;
    int c;
    char *buffer = (char *)malloc(size);

    if (argc != 2){
        printf("How to run:\n \t./sintax_analizer <file name>\n");
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
        if(S(buffer)){
            graphAST(buffer);
        }else{
            printf("sentence %s does not apply to the grammar", buffer);
        }
        printf("\n");
      } while(c != EOF); 
      fclose(f);           
    }
    free(buffer);

    return 0;
}