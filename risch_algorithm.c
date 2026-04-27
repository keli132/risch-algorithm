#include <stdio.h>
#include <stdlib.h>
#define RETURN_VALUE 0
#define DEFAULT 67 

//How should I go on about this?
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

typedef struct token {
    token_type type;
    struct { //Used only for number and variable types.
        int number;
        char variable;
    } value;
} token;

typedef struct { //Dynamic array for tokens.
    token *data;
    int size;
    int capacity;
} tokensArray;

token* createToken(token_type type, int number, char variable) { //Allocates memory for a new token.
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
        printf("woof\n");
    }
    printf("meow\n");
    tokens->data[tokens->size] = *newToken;
    tokens->size++;
}

token* tokenizer (char* input) { //Parsing text into a dynamic array of tokens.

}

int main (int *argc, char argv[]) {
    tokensArray tokens; //Initializing 
    initTokenArray(&tokens);

    //debugging
    addTokenToArray(&tokens, createToken(TOKEN_PLUS, DEFAULT, DEFAULT));
    addTokenToArray(&tokens, createToken(TOKEN_PLUS, DEFAULT, DEFAULT));
    addTokenToArray(&tokens, createToken(TOKEN_PLUS, DEFAULT, DEFAULT));
    addTokenToArray(&tokens, createToken(TOKEN_NUMBER, 2, DEFAULT));
    printf("%d\n", tokens.data[3].value.number);
    printf("%d\n", tokens.size);
    
    return RETURN_VALUE;
}


