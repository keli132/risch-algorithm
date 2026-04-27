#include <stdio.h>
#include <stdlib.h>
#define RETURN_VALUE 0

//How should I go on about this?
/* Goals:
1. Way to parse text into tokens, creates a list which can be given to another function that
   creates the ast. Preferably would love for it to be reusable
   i.e. creating a new list every time.
2. Way to create the ast.
3. way to use the ast in a meaningfull way i.e. a way to go from node to node and simplify
   expressions.
*/

typedef enum {
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

typedef struct {
    token_type type;
    union { //Used only for number and variable types.
        int number;
        char variable;
    } value;
} token;

typedef struct { //Dynamic array for tokens.
    token *data;
    int size;
    int capacity;
} tokensArray;

void initTokenArray (tokensArray *tokens) { //initializing dynamic array.
    tokens->size = 0;
    tokens->capacity = 2; 
    tokens->data = malloc(tokens->capacity * sizeof(token));
}

void pushTokenToArray (tokensArray *tokens, token newToken) { //Adding new token to array.
    if (tokens->size == tokens->capacity) { //Add capacity if full.
        tokens->capacity *= 2;
        tokens->data = realloc(tokens->data, tokens->capacity * sizeof(token)); 
    }
    tokens->data[tokens->size + 1] = newToken;
}

token* tokenizer (char* input) { //Parsing text into a dynamic array of tokens.

}

int main (int *argc, char argv[]) {

    return RETURN_VALUE;
}


