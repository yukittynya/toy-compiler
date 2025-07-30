#include "parser.h"

#include <stdlib.h>

Parser createParser(TokenArray *arr) {
    Parser parser;

    parser.tokens = arr;

    return parser; 
}
