/*
ast.cpp

Author: Shahbaz
Date 26/01/2025
*/

#ifndef AST_HPP
#define AST_HPP

#include <iostream>
#include <vector>
#include <memory>
#include <sstream>
#include "token.hpp"


// class Program {
//     public:
//         std::vector<ExpressionNode> Expressions;
// };

// Base class for all expressions
class ExpressionNode {
    public: 
        virtual std::string TokenLiteral() = 0;
        virtual std::string String() = 0;

        virtual ~ExpressionNode() = default;
};


class NumberExpressionNode : public ExpressionNode {
    public:
        Token Tok;   
        double Value;

        std::string TokenLiteral() override { return Tok.Literal; };
        std::string String() override { return Tok.Literal; };

        NumberExpressionNode(Token tok, double Val) : Tok(tok), Value(Val) {}
};

class VariableExpressionNode : public ExpressionNode {
    public: 
        std::string Value;
        Token Tok;    // token::VAR

        virtual std::string TokenLiteral() override { return Tok.Literal; };
        virtual std::string String() override { return Value; };

        VariableExpressionNode(const std::string &Name, Token &tok) : Value(Name), Tok(tok)  {}
};


class PrefixExpressionNode : public ExpressionNode {
    public:
        Token Tok;        // prefix token, e.g. - for negative numbers    
        char Operator;
        std::unique_ptr<ExpressionNode> Right;

        std::string TokenLiteral() override { return Tok.Literal; };
        std::string String() override { 
            std::ostringstream oss;

            oss << "(";
            oss << Operator;
            oss << Right->String();
            oss << ")";

            std::string result = oss.str();

            return result;
         };

         PrefixExpressionNode(char Op, Token &tok, std::unique_ptr<ExpressionNode> Right) 
                : Tok(tok), Operator(Op), Right(std::move(Right)) {}
};

class InfixExpressionNode : public ExpressionNode {
    public:
        Token Tok;        // operator token, e.g. +, -, *, /
        char Operator;
        std::unique_ptr<ExpressionNode> Left, Right;

        std::string TokenLiteral() override { return Tok.Literal; };
        std::string String() override { 
            std::ostringstream oss;

            oss << "(";
            oss << Left->String();
            oss << " " << Operator << " ";
            oss << Right->String();
            oss << ")";

            std::string result = oss.str();

            return result;
         };

         InfixExpressionNode(char Op, Token &tok, std::unique_ptr<ExpressionNode> Left, std::unique_ptr<ExpressionNode> Right) 
                : Tok(tok), Operator(Op), Left(std::move(Left)), Right(std::move(Right)) {}
};

#endif