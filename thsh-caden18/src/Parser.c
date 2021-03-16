#include "Parser.h"
#include "Node.h"

Node* parse(Scanner *scanner)
{
    const char *error = "Error!";
    Node *node = ErrorNode_new(error);
    return node;
}
