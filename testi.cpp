#include <stdio.h>
#include <string.h>
#define RETURN_VALUE 0
#define MAX_TOKENS 100
#define MAX_NODES 100
#define MAX_FUNCTION_SIZE 20

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

struct Token { //for text parsing
    token_type type;
    union {
        int number; //used if type is a number.
        char variable; //used if type is a variable.
    } value;
};

struct Node { //for ast
    token_type type; //yea im just reusing these why not
    Token* reference_token; //address to used token
    Node* left_node; //left and right child nodes
    Node* right_node;
    Node* mother_node; //might be usefull, no idea tho
};

const char* tokenTypeToString(token_type type)
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

token_type readFunction(int* i, char* Input) { //in case tan is not just t a and n
    char function[MAX_FUNCTION_SIZE];
    int a = 0;
    while(Input[(*i)] >= 'a' && Input[(*i)] <= 'z') {
        function[a] = Input[*i];
        (*i)++;
        a++;
    }
    function[a] = '\0';
    printf("Succesfully read function: %s \n", function); //debugging
    if (strcmp(function, "sin") == 0) {return TOKEN_SIN;}  //need to add more funcitons eventually
    if (strcmp(function, "cos") == 0) {return TOKEN_COS;}
    if (strcmp(function, "tan") == 0) {return TOKEN_TAN;}
    else {
        printf("Something fucked up\n");
        return TOKEN_DEFAULT;
    }
}

int readFullNumber(int* i, char* Input) { //in case 56 is not just 5 and 6 
    int number = Input[*i] - '0';
    while (Input[(*i)+1] >= '0' && Input[(*i)+1] <= '9') {
        number = number*10 + (Input[(*i)+1] - '0');
        (*i)++;
    }
    (*i)++;
    return number;
}

Token Tokens[MAX_TOKENS]; //array for tokens
int Tokens_count = 0; //number of tokens inside array after tokenizer()
// i should make tokenizer return a pointer to tokens and create a new one each time in case i need to re-call it fuck important !!!
//IMPORTANT ^^^^!!!!
void tokenizer(char* Input, Token* Tokens, int* Tokens_count) { //obvious name retard
    int i = 0;
    while (Input[i] != '\0') { //loops through input
        char c = Input[i]; //makes life easier
        if (c >= '0' && c <= '9') { //if number
            Tokens[*Tokens_count].type = TOKEN_NUMBER;
            Tokens[*Tokens_count].value.number = readFullNumber(&i, Input);
            printf("Succesfully read number: %d \n", Tokens[*Tokens_count].value.number); //debugging
            (*Tokens_count)++;
        }
        if (c >= 'a' && c <= 'z') { //if letter, subcases for functions and variables:
            if (Input[i+1] >= 'a' && Input[i+1] <= 'z') { //if function
                Tokens[*Tokens_count].type = readFunction(&i, Input);
                (*Tokens_count)++;
            }
            else {
                Tokens[*Tokens_count].type = TOKEN_VARIABLE; //if variable
                Tokens[*Tokens_count].value.variable = c;
                printf("Succesfully read variable: %c \n", Tokens[*Tokens_count].value.variable);
                (*Tokens_count)++;
                i++;
            }
        }
        if (c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')' || c == '^') {
            switch (c) {
            case '+': Tokens[*Tokens_count].type = TOKEN_PLUS; break;
            case '-': Tokens[*Tokens_count].type = TOKEN_MINUS; break;
            case '*': Tokens[*Tokens_count].type = TOKEN_MULTIPLY; break;
            case '/': Tokens[*Tokens_count].type = TOKEN_DIVIDE; break;
            case '(': Tokens[*Tokens_count].type = TOKEN_LPAREN; break;
            case ')': Tokens[*Tokens_count].type = TOKEN_RPAREN; break;
            default: printf("woof\n"); break; //debugging
        }
            (*Tokens_count)++;
            i++;
        }
    }   
    printf("Succesfully tokenized input string!\n");
}


Node* aster (Token* Tokens, int* Tokens_count) { //should be obvious by now 
    //should return an address to a list of nodes, takes in everything about the tokens
    /* how should this even work uhhhh. create a list like if input tokens are x*(x+1) (in order) then resulting ast tree should look like
          *
        x   +
           x  1
    and the nodes can be inside the resulting list however they want, the order and stucture comes from each node having a pointer to 
    important nodes e.g. the mother node and child nodes. THE PURPOSE OF THIS FUNCTION IS ONLY TO CREATE SUCH AN AST NOT SIMPLIFY 
    EXPRESSIONS, my idea is to be able to use this with tokenizer recursively if needed. 
    */
    Node Nodes[MAX_NODES]; //array for nodes
    int node_count = 0; //for keeping count

    //checks if token on index i is a bracket and creates a child node off first operation
    //and child nodes out of that, might need to also handle cases where (2*x+4-y) i.e.
    //multiple operations inside () D:
    Node* parenthesisNode(int* node_count, Token* Tokens, Node* Nodes) {
        
        
        return 
    }

    for (int i=0; i <= *Tokens_count; i++) {
        if (Tokens[i].type >= 3 && Tokens[i].type <= 5) { //checks if token is operation 
            Nodes[node_count].reference_token = &Tokens[i]; //makes a node out of said operation
            Nodes[node_count].type = Tokens[i].type; //copies the type
            node_count++; 
            //next i need to check the left token which can only be ) or a number or a variable and make it into a child node
            //no i need to make a check if () or just number/variable and if () then make a big ass node
            if (Token[i+1].type == TOKEN_LPAREN) {
                parenthesisNode()
            }
            
        }   
    }

    return Nodes;
}

void printTokens(Token* Tokens, int* Tokens_count) {//prints everything, should make it return as a string too maybe
    for (int j=0; j < *Tokens_count; j++) {
        if (Tokens[j].type == TOKEN_NUMBER) {
            printf("%d", Tokens[j].value.number);
            continue;
        }
        if (Tokens[j].type == TOKEN_VARIABLE) {
            printf("%c", Tokens[j].value.variable);
            continue;
        }
        printf("%s", tokenTypeToString(Tokens[j].type));
    }
    printf("\n");
}



int main(int argc, char const *argv[]) { //main loop

    char Input_example[] = "x*(x+1)"; //testing
    tokenizer(Input_example, Tokens, &Tokens_count);

    for(int i=0; i < Tokens_count; i++) { //debugging
        printf("%dth token type: %s \n", (i+1), tokenTypeToString(Tokens[i].type));
        if (Tokens[i].type == TOKEN_NUMBER) {
            printf("%dth token value: %d \n", (i+1), Tokens[i].value.number);
        }
        if (Tokens[i].type == TOKEN_VARIABLE) {
            printf("%dth token value: %c \n", (i+1), Tokens[i].value.variable);
        }
    }
    printTokens(Tokens, &Tokens_count); //debugging for now



    return RETURN_VALUE;
}




