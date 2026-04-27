#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define RETURN_VALUE 0
#define DEFAULT 67 

//How should I go on about this? This file might mainly be the program to turn input string into ast?
/* Goals:
1. Way to parse text into tokens, creates a list which can be given to another function that
   creates the ast. Preferably would love for it to be reusable
   i.e. creating a new list every time.
2. Way to create the ast.
3. way to use the ast in a meaningfull way i.e. a way to go from node to node and simplify
   expressions.

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

const char* tokenTypeToString(int type)
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
} token;

typedef struct { //Dynamic array for tokens.
    token *data; //List containing tokens.
    int size; //Amount of tokens.
    int capacity; //Maximum amount of tokens currently.
} tokensArray;

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

void initTokenArray (tokensArray *tokens) { //Initializing dynamic array.
    tokens->size = 0;
    tokens->capacity = 2; 
    tokens->data = malloc(tokens->capacity * sizeof(token));
}

void addTokenToArray (tokensArray *tokens, token *newToken) { //Adding new token to array.
    if (tokens->size == tokens->capacity) { //Add capacity if full.
        printf("Allocating more memory for tokens...\n");
        tokens->capacity *= 2;
        tokens->data = realloc(tokens->data, tokens->capacity * sizeof(token)); 
        if (tokens->data == NULL) {
            printf("Memory allocation failed!\n");
            return;
        } 
    }
    tokens->data[tokens->size] = *newToken;
    tokens->size++;
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
        printf("Something fucked up\n");
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
        printf("Succesfully tokenized input: %c\n", c);
        i++;
        continue;
    }
    printf("Succesfully tokenized input string!\n"); //Debugging
}

int main (int *argc, char argv[]) {
    tokensArray tokens; //Initializing 
    initTokenArray(&tokens);

    //debugging
    char text[] = "23x1111)(sin/sin))(tan))(/(y4";
    tokenize(&tokens, text);

    for(int i = 0; i<tokens.size; i++) {
        printf("%d. token type: %s\n", i+1, tokenTypeToString(tokens.data[i].type));
    }

    return RETURN_VALUE;
}


