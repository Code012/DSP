/*
ast.cpp

Author: Shahbaz
Date 26/01/2025
*/
// See: https://lesleylai.info/en/ast-in-cpp-part-1-variant/ for visitor apttern 

#ifndef AST_HPP
#define AST_HPP

#include <iostream>
#include <vector>
#include <memory>
#include <sstream>
#include "token.hpp"
#include "visitors.hpp"

using u64 = uint64_t;

// class Program {
//     public:
//         std::vector<ExpressionNode> Expressions;
// };

//TODO: Implement visitor pattern when on the evaluation stage to decouple eval/simplification logic from ast
// Base class for all expressions
class ExpressionNode {
    public: 
    
        virtual ~ExpressionNode() = default;
        
        virtual std::string TokenLiteral() = 0;
        virtual std::string String() = 0;

        virtual void accept(ExprVisitor& visitor) const  = 0;
        virtual void accept(ExprMutableVisitor& visitor) = 0;
};


class NumberExpressionNode : public ExpressionNode {
    public:
        Token Tok;   
        u64 Value;

        NumberExpressionNode(Token tok, u64 Val) : Tok(tok), Value(Val) {}
        
        std::string TokenLiteral() override { return Tok.Literal; };
        std::string String() override { return Tok.Literal; };

        void accept(ExprVisitor& visitor) const override {visitor.visit(*this);}
        void accept(ExprMutableVisitor& visitor) override {visitor.visit(*this);}


};

class VariableExpressionNode : public ExpressionNode {
    public: 
        std::string Value;
        Token Tok;    // token::VAR

        VariableExpressionNode(const std::string &Name, Token &tok) : Value(Name), Tok(tok)  {}
        
        virtual std::string TokenLiteral() override { return Tok.Literal; };
        virtual std::string String() override { return Value; };

        void accept(ExprVisitor& visitor) const override {visitor.visit(*this);}
        void accept(ExprMutableVisitor& visitor) override {visitor.visit(*this);}
};


class PrefixExpressionNode : public ExpressionNode {
    public:
        char Operator;
        Token Tok;        // prefix token, e.g. - for negative numbers    
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
        
        PrefixExpressionNode(char Op, Token &tok, std::unique_ptr<ExpressionNode> Right = nullptr) 
                : Operator(Op), Tok(tok), Right(std::move(Right)) {}

        void accept(ExprVisitor& visitor) const override {visitor.visit(*this);}
        void accept(ExprMutableVisitor& visitor) override {visitor.visit(*this);}
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

         InfixExpressionNode(Token &tok, char Op,  std::unique_ptr<ExpressionNode> Left, std::unique_ptr<ExpressionNode> Right = nullptr) 
                : Tok(tok), Operator(Op), Left(std::move(Left)), Right(std::move(Right)) {}
        
        void accept(ExprVisitor& visitor) const override {visitor.visit(*this);}
        void accept(ExprMutableVisitor& visitor) override {visitor.visit(*this);}
};

#endif