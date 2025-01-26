/*
lexer.cpp
Author: Shahbaz
Date 20/01/2025
*/

#ifndef LEXER_HPP
#define LEXER_HPP

#include <iostream>
#include <map>
#include <cstdio>
#include <cstdint>
#include "token.hpp"




class Lexer {
    public:
        std::string input;
        size_t position {0};
        size_t peekPosition {0};
        char ch;
    
        Lexer(std::string& input);

        const Token nextToken();
        void skipWhitespace();
        void readChar();
        char peekChar() const;
        std::string readIdentifier();
        std::string readNumber();
        const Token newToken(TokenType, char ch);
};



#endif