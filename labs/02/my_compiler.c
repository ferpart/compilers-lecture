#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define nbytes 2048

// A structure to represent a stack 
struct StackNode { 
	char data; 
	struct StackNode* next; 
}; 

struct StackNode* newNode(char data) 
{ 
	struct StackNode* stackNode = (struct StackNode*)malloc(sizeof(struct StackNode)); 
	stackNode->data = data; 
	stackNode->next = NULL; 
	return stackNode; 
} 

int isEmpty(struct StackNode* root) 
{ 
	return !root; 
} 

void push(struct StackNode** root, char data) 
{ 
	struct StackNode* stackNode = newNode(data); 
	stackNode->next = *root; 
	*root = stackNode; 
} 

char pop(struct StackNode** root) 
{ 
	if (isEmpty(*root)) 
		return CHAR_MIN; 
	struct StackNode* temp = *root; 
	*root = (*root)->next; 
	char popped = temp->data; 
	free(temp); 

	return popped; 
} 

char peek(struct StackNode* root) 
{ 
	if (isEmpty(root)) 
		return CHAR_MIN; 
	return root->data; 
}

void displayer(struct StackNode* syntaxStack, struct StackNode* syntaxExtraStack,
               int commentflag, int multilineCommentFlag, int singleQuoteFlag, 
               int doubleQuoteFlag)
{
    while(!isEmpty(syntaxStack)){
        switch (pop(&syntaxStack)){
            case('('):
                printf("There is a missing: )\n");
                break;
            case('{'):
                printf("There is a missing: }\n");
                break;
            case('['):
                printf("There is a missing: ]\n");
                break;
        }
    }
    while(!isEmpty(syntaxStack)){
                switch (pop(&syntaxStack)){
            case(')'):
                printf("There is a missing: (\n");
                break;
            case('}'):
                printf("There is a missing: {\n");
                break;
            case(']'):
                printf("There is a missing: [\n");
                break;
        }
    }

    if (commentflag)
        printf("Unclosed Comment\n");

    if (multilineCommentFlag)
        printf("There is a missing: */\n");

    if(singleQuoteFlag)
        printf("There is a missing: '\n");

    if(doubleQuoteFlag)
        printf("There is a missing: \"\n");
}

void fileReader(int fd){

    char *openerElements = "({[";
    char *closerElements = ")}]";

    struct StackNode* syntaxStack = NULL;
    struct StackNode* syntaxExtraStack = NULL;
    
    int commentFlag = 0;
    int multiLineCommentFlag = 0;
    int singleQuoteFlag = 0;
    int doubleQuoteFlag = 0;
    char *buffer;
    buffer = (char*) calloc(nbytes, sizeof(char));
    while (read(fd, buffer, nbytes-1) > 0){
        for (int i=0; buffer[i]!='\0'; i++){

            if(buffer[i] == '/' && buffer[i+1] == '/' && !multiLineCommentFlag &&
                !singleQuoteFlag && !doubleQuoteFlag && !commentFlag)
                commentFlag = 1;
            
            else if(buffer[i] == '\n' && !multiLineCommentFlag && !singleQuoteFlag 
                && !doubleQuoteFlag && commentFlag)
                commentFlag = 0;
            
            else if(buffer[i] == '/' && buffer[i+1] == '*' && !commentFlag && 
                !singleQuoteFlag && !doubleQuoteFlag && !multiLineCommentFlag)
                multiLineCommentFlag = 1;
            
            else if(buffer[i] == '/' && buffer[i-1] == '*' && !commentFlag && 
                !singleQuoteFlag && !doubleQuoteFlag && multiLineCommentFlag)
                multiLineCommentFlag = 0;

            else if(buffer[i] == '\'' && !commentFlag && !multiLineCommentFlag &&
                !doubleQuoteFlag && !singleQuoteFlag)
                singleQuoteFlag = 1;

            else if(buffer[i] == '\'' && !commentFlag && !multiLineCommentFlag &&
                !doubleQuoteFlag && singleQuoteFlag)
                singleQuoteFlag = 0;

            else if(buffer[i] == '"' && !commentFlag && !multiLineCommentFlag &&
                !singleQuoteFlag && !doubleQuoteFlag)
                doubleQuoteFlag = 1;

            else if(buffer[i] == '"' && !commentFlag && !multiLineCommentFlag &&
                !singleQuoteFlag && doubleQuoteFlag)
                doubleQuoteFlag = 0; 

            if (!commentFlag && !multiLineCommentFlag && !singleQuoteFlag && !
                doubleQuoteFlag){
                for(int j=0; j < 3; j++){
                    if(buffer[i] == openerElements[j]){
                        push(&syntaxStack, buffer[i]);
                        break;

                    }else if (buffer[i] == closerElements[j]){
                        int pushed = 0;
                        for (int k = 0; k < 3; k++){
                            if(buffer[i] == closerElements[k] && peek(syntaxStack) 
                                == openerElements[k]){
                                pop(&syntaxStack);
                                pushed = 1;
                                break;
                            }
                        }

                        if(!pushed)
                            push(&syntaxExtraStack, buffer[i]);
                        pushed=0;
                        break;
                    }
                }
            }

        }

        memset(buffer, '\0', nbytes);
        if(commentFlag || multiLineCommentFlag || singleQuoteFlag, doubleQuoteFlag,
            !isEmpty(syntaxStack) || !isEmpty(syntaxExtraStack))
        {
            displayer(syntaxStack, syntaxExtraStack, commentFlag, multiLineCommentFlag,
                    singleQuoteFlag, doubleQuoteFlag);
        }else{
            printf("There are no errors\n");
        }
    }
    close(fd);
    
}

int main(int argc, char *argv[])
{
    int fd;
    if (argc == 2){
        fd = open(argv[1], O_RDONLY);

        if (fd == -1){
            printf("File not found, errno =%d\n", errno);
            return 1;
        }

        fileReader(fd);


        return 0;
    }
    printf("No arguments given \n");
    return 1;
}
