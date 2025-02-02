/*
token.hpp

Author: Shahbaz
Date: 19/01/2025
*/

#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <iostream>
#include <map>
#include <cstdio>
#include <cstdint>

using TokenType = std::string;  // string for debugging purposes, will change to int later

class Token {
    public:
        TokenType Type;
        std::string Literal;
};

//research to use const or constexpr

namespace token {
const TokenType PLUS = "PLUS";
const TokenType MINUS = "MINUS";
const TokenType MULT = "MULT";
const TokenType DIV = "DIV";

const TokenType INT = "INT";

const TokenType VAR = "VAR";

const TokenType LPAREN = "LPAREN";
const TokenType RPAREN = "RPAREN";

const TokenType FUNC = "FUNC";

const TokenType ILLEGAL = "ILLEGAL";

const TokenType EOL = "EOL";
}

#endif