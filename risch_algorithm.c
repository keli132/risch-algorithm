#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define RETURN_VALUE 0
#define DEFAULT 67 
//This program should only be for the AST creation at first. Other actual logic regarding the full algorithm should
//be put elsewhere.
//How should I go on about this? This file might mainly be the program to turn input string into ast?
/* Goals:
1. Way to parse text into tokens, creates a list which can be given to another function that
   creates the ast. Preferably would love for it to be reusable
   i.e. creating a new list every time.
2. Way to create the ast.
3. way to use the ast in a meaningfull way i.e. a way to go from node to node and simplify
   expressions.

   Also I will be very strict with the input rules so no need to think how to make it easier since it's carved in stone now.
*/

typedef enum { //Pretty self explanatory.
    TOKEN_DEFAULT,
    TOKEN_NUMBER,
    TOKEN_VARIABLE,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MULTIPLY,
    TOKEN_DIVIDE,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_SIN,
    TOKEN_COS,
    TOKEN_TAN
} token_type; 

const char* tokenTypeToString(int type) //For the ease of debugging.
{
    switch (type) {
        case TOKEN_NUMBER: return "number";
        case TOKEN_PLUS:    return "+";
        case TOKEN_MINUS:   return "-";
        case TOKEN_MULTIPLY: return "*";
        case TOKEN_DIVIDE:  return "/";
        case TOKEN_VARIABLE: return "variable";
        case TOKEN_COS:     return "cos";
        case TOKEN_SIN:     return "sin";
        case TOKEN_TAN:     return "tan";
        case TOKEN_LPAREN:  return "(";
        case TOKEN_RPAREN:  return ")";
        default:            return "UNKNOWN";
    }
}

typedef struct {
    token_type type;
    struct { //Used only for number and variable types.
        int number;
        char variable;
    } value;
    int position; //Position in tokensArray.
} token;

typedef struct { //Dynamic array for tokens.
    token *data; //List containing tokens.
    int size; //Amount of tokens.
    int capacity; //Maximum amount of tokens currently.
} tokensArray;

typedef struct node{
    token* referenceToken; //Reference to the token where information about object is. Should be faster than
                           //just copying the values to the node.
    struct node* leftNode; //Pointers to childs give stucture to the ast.
    struct node* rightNode;
    struct node* motherNode;
} node;

token *createToken(token_type type, int number, char variable) { //Allocates memory for a new token.
    token* result = malloc(sizeof(token));
    if (result != NULL) {
        result->type = type;
        switch (type) {
            case TOKEN_NUMBER: result->value.number = number;
            case TOKEN_VARIABLE: result->value.variable = variable;
        }
        return result;
    }
    printf("Something went wrong with memory allocation!\n");
}

node *createNode (token* referenceToken, node* leftNode, node* rightNode, node* motherNode) { //Allocates memory for a new node.
    node* result = malloc(sizeof(node));
    if (result != NULL) {
        result->referenceToken = referenceToken;
        result->leftNode = leftNode;
        result->rightNode = rightNode;
        result->motherNode = motherNode;
        return result;
    }
    printf("Something went wrong with memory allocation!\n");
}

void initTokenArray (tokensArray *tokens, int capacity) { //Initializing dynamic array.
    tokens->size = 0;
    tokens->capacity = capacity; 
    tokens->data = malloc(tokens->capacity * sizeof(token));
}

void addTokenToArray (tokensArray *tokens, token *newToken) { //Adding new token to array.
    if (tokens->size >= tokens->capacity) { //Adds capacity if full.
        printf("Allocating more memory for tokens...\n");
        tokens->capacity *= 2;
        tokens->data = realloc(tokens->data, tokens->capacity * sizeof(token)); 
        if (tokens->data == NULL) {
            printf("Memory allocation failed!\n");
            return;
        } 
    }
    newToken->position = tokens->size;
    tokens->data[tokens->size] = *newToken;
    tokens->size++;
}

void removeTokenFromArray (tokensArray *tokens, int removeTokenPosition) { //Removes a token from a slot.
    if (tokens->size < removeTokenPosition + 1 || removeTokenPosition < 0) {
        printf("Cannot remove token from index: %d\n", removeTokenPosition);
        return;
    }
    tokens->size--; //Need to work on this maybe? maybe problem??
    for (int i = removeTokenPosition; i < tokens->size; i++) { //Slowly moves everything back by one index.
        printf("Removing index: %d\n", i);
        tokens->data->position--;
        tokens->data[i] = tokens->data[i+1];

    }
}

node *createAst (tokensArray *tokens);
void debugPrint (tokensArray  *tokens);
node *findMostOutParen(tokensArray *tokens);

node *createSubTree (tokensArray *tokens, int firstTokenPosition, int lastTokenPosition) { //Creates a new tree.
    tokensArray subTokens;
    initTokenArray(&subTokens, 2);

    for (int i = firstTokenPosition + 1; i <= lastTokenPosition - 1; i++) { //Copies everything to subTokens.
        addTokenToArray(&subTokens, &(tokens->data[i]));
    }
    debugPrint(&subTokens);
    findMostOutParen(&subTokens);
    return createAst(&subTokens);

}

void debugPrint (tokensArray *tokens) {
    for (int i = 0; i < tokens->size; i++) {
    printf("%d. token type: %s\n", i+1, tokenTypeToString(tokens->data[i].type));
    }
    printf("Input: ");
    for (int i = 0; i<tokens->size; i++) {
        if (tokens->data[i].type == TOKEN_NUMBER) {
            printf("%d", tokens->data[i].value.number);
            continue;
        }
        if (tokens->data[i].type == TOKEN_VARIABLE) {
            printf("%c", tokens->data[i].value.variable);
            continue;
        }
        printf("%s", tokenTypeToString(tokens->data[i].type));
    }
    printf("\nTokens size: %d\n", tokens->size);
}

token_type readFullFunction(char* input, int* i) {
    char function[20]; //Defines max function length too.
    int a = 0;
    while(input[(*i)] >= 'a' && input[(*i)] <= 'z') {
        function[a] = input[*i];
        (*i)++;
        a++;
    }
    function[a] = '\0';  

    printf("Succesfully read function: %s \n", function); //Debugging
    if (strcmp(function, "sin") == 0) {return TOKEN_SIN;}  //Need to add more funcitons eventually.
    if (strcmp(function, "cos") == 0) {return TOKEN_COS;}
    if (strcmp(function, "tan") == 0) {return TOKEN_TAN;}
    else {
        printf("Something went wrong\n");
        return TOKEN_DEFAULT;
    }
}

int readFullNumber(char* input, int* i) { //Probably easier way to do this with libaries.
    int number = input[*i] - '0';
    while (input[(*i)+1] >= '0' && input[(*i)+1] <= '9') {
        number = number*10 + (input[(*i)+1] - '0');
        (*i)++;
    }
    (*i)++;
    printf("Succesfully read number %d\n", number); //Debugging
    return number;
}

void tokenize (tokensArray* tokens, char* input) { //Parsing text into a dynamic array of tokens.
    int i = 0;
    char c;
    while (input[i] != '\0') { //Loops throught input.
        c = input[i];
        if (c >= '0' && c <= '9') { //Number
            addTokenToArray(tokens, createToken(TOKEN_NUMBER, readFullNumber(input, &i), DEFAULT));
            continue;
        }
        if (c >= 'a' && c <= 'z') { //Letter
            if (input[i+1] >= 'a' && input[i+1] <= 'z') { //Next also letter meaning function
                addTokenToArray(tokens, createToken(readFullFunction(input, &i), DEFAULT, DEFAULT));
                continue;
            }
            else { //Just a variable
                addTokenToArray(tokens, createToken(TOKEN_VARIABLE, DEFAULT, c));
                printf("Succesfully read variable: %c\n", c); //Debugging
                i++;
                continue;
            }
        } 
        switch(c) {
            case '+': addTokenToArray(tokens, createToken(TOKEN_PLUS, DEFAULT, DEFAULT)); break;
            case '-': addTokenToArray(tokens, createToken(TOKEN_MINUS, DEFAULT, DEFAULT)); break;
            case '/': addTokenToArray(tokens, createToken(TOKEN_DIVIDE, DEFAULT, DEFAULT)); break;
            case '*': addTokenToArray(tokens, createToken(TOKEN_MULTIPLY, DEFAULT, DEFAULT)); break;
            case '(': addTokenToArray(tokens, createToken(TOKEN_LPAREN, DEFAULT, DEFAULT)); break;
            case ')': addTokenToArray(tokens, createToken(TOKEN_RPAREN, DEFAULT, DEFAULT)); break;
        }
        printf("Succesfully read input: %c\n", c);
        i++;
    }
    printf("Succesfully tokenized input string!\n"); //Debugging
}

/*

---My ideas---
Next thing I need is a function that converts the gotten tokenArray into an abstract syntaxt tree, and to do that
it needs rules i.e. operations have children and I need to figure out something with ().

Let's brainstorm. 
x*(x+1) should become:
    *
  x   +
    x   1  
Which is simple in theory but requires some phases in between. First the function should have a way how to 
loop and find the order of operations e.g:
x+1+3*x could become
        +                       +                       *
    x      +        or      +      *     but not     +    x
        1      *          x   1  3   x            +    3
             3    x                             x   1
since then the correct order of operations would be lost. => highest order of operation operators should be lower
than ones of lesser order? Also the last example would just be x*(3+1+x). I think I should keep the highest order
operations the lowest since they only affect two nodes instead of everything under. 
Communativity is also something to take into account. 

So how should the algorithm look like? It should create the exact ast as in with x*(x+1) from where other functions
could simplify the trees in that case it would eventually become x*x+x. Functions are weird with this

x*sin(2*x+3+y) would be 
            *
         x     sin
                +
             +    y
          *    3
        2   x
           
functions are very easy to be problematic, easiest idea would be to create an entirely new tree for 
what is inside it. Also I notice it should be possible to make every tree in the form of a diagonal like so.

x*x+(2/y)*x+4 

        +
     4     +
        *     *
      x   x  x   /
               2   y
Taking a common factor we get x*(x+(2/y))+4
            +
          4    *
             x   +
               x   /
                 2   y
Which shows (I suppose) we could always transform the trees into a form like that line with only two elements per level

So how does such a function actually work? Something that just purely makes a version of a tree from the tokens 
which can be modified and simplified later. 
It should scan the dynamic array to first find all the the most outside parenthesis () and then treat everything 
inside one as a placeholder node before it starts to recursively call itself on the resulting sub array. 
After it correctly finds and makes the outside parenthesis placeholder nodes it can start to look into order of
operations.


Once we have the addresses for the outermost parenthesis we should try to create a subtree?
*/

node *findMostOutParen (tokensArray *tokens) {
    int leftParen_count = 0; 
    int rightParen_count = 0;
    token* leftParen = NULL;
    token* rightParen;
    for(int i = 0; i < tokens->size; i++) { //Loops throught tokens.
        if (tokens->data[i].type == TOKEN_LPAREN) { //First encounter will be outmost parenthesis.
            leftParen_count++;
            if (leftParen == NULL) {
                leftParen = &tokens->data[i];
            }
        }
        if (tokens->data[i].type == TOKEN_RPAREN) {
            rightParen_count++;
            rightParen = &tokens->data[i];
        }
        if (leftParen_count == rightParen_count && leftParen_count != 0) {
            printf("Outmost parenthesis from %d to %d\n", leftParen->position, rightParen->position); //Debug
            return createSubTree(tokens, leftParen->position, rightParen->position);
            leftParen_count = rightParen_count = 0; //Resets the counter.
            leftParen = NULL;

        }
    }

    //Then actual ast creation logic.

}

node *createAst (tokensArray *tokens) {
    findMostOutParen (tokens);

}


int main (int *argc, char argv[]) {
    tokensArray tokens; //Initializing 
    initTokenArray(&tokens, 2);

    //debugging
    char text[] = "x*(x+1)+2";
    tokenize(&tokens, text);

    debugPrint(&tokens);
    findMostOutParen(&tokens);
    

    return RETURN_VALUE;
}


