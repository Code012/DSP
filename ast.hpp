/*
ast.cpp

Author: Shahbaz
Date 26/01/2025
*/
// See: https://lesleylai.info/en/ast-in-cpp-part-1-variant/ for visitor apttern 

#ifndef AST_HPP
#define AST_HPP



class ExpressionNode;
class NumberExpressionNode;
class VariableExpressionNode;
class PrefixExpressionNode;
class InfixExpressionNode;
class NaryExpressionNode;
enum class InfixKind;

#include <iostream>
#include <vector>
#include <memory>
#include <sstream>
#include "token.hpp"
#include "visitors.hpp"




using u64 = uint64_t;

enum class InfixKind {PLUS, MULTIPLY, POWER, DIFFERENCE, FRACTION, NUM, VAR, PRE_MINUS};


// Base class for all expressions
class ExpressionNode {
    public: 
    
        virtual ~ExpressionNode() = default;
        
        virtual std::string TokenLiteral() = 0;
        virtual std::string String() = 0;
        virtual InfixKind getKind() = 0;

        virtual void accept(ExprVisitor& visitor) const  = 0;
        virtual void accept(ExprMutableVisitor& visitor) = 0;
};


class NumberExpressionNode : public ExpressionNode {
    public:
        Token Tok;   
        u64 Value;
        InfixKind Kind;

        NumberExpressionNode(Token tok, u64 Val, InfixKind Kind);
        
        std::string TokenLiteral() override;
        std::string String() override;
        InfixKind getKind() override;

        void accept(ExprVisitor& visitor) const override;
        void accept(ExprMutableVisitor& visitor) override;


};

class VariableExpressionNode : public ExpressionNode {
    public: 
        std::string Value;
        Token Tok;    // token::VAR
        InfixKind Kind;

        VariableExpressionNode(const std::string &Name, Token &tok, InfixKind Kind);
        
        virtual std::string TokenLiteral() override;
        virtual std::string String() override;
        InfixKind getKind() override;

        void accept(ExprVisitor& visitor) const override;
        void accept(ExprMutableVisitor& visitor) override;
};


class PrefixExpressionNode : public ExpressionNode {    //* RENAME TO UNARY
    public:
        char Operator;
        Token Tok;        // prefix token, e.g. - for negative numbers    
        InfixKind Kind;
        std::unique_ptr<ExpressionNode> Right;

        PrefixExpressionNode(char Op, Token &tok, InfixKind Kind, std::unique_ptr<ExpressionNode> Right = nullptr);

        std::string TokenLiteral() override;
        InfixKind getKind() override;
        std::string String() override;
        
        

        void accept(ExprVisitor& visitor) const override;
        void accept(ExprMutableVisitor& visitor) override;
};

class InfixExpressionNode : public ExpressionNode { //* RENAME TO BINARY WHEN YOU REDO EVERYTHING FOR DISSERATTION
    public:
        Token Tok;        // operator token, e.g. +, *, -, /
        char Operator;
        InfixKind Kind;
        std::unique_ptr<ExpressionNode> Left, Right;
        

        std::string TokenLiteral() override;
        InfixKind getKind() override;
        std::string String() override;

         InfixExpressionNode(Token &tok, char Op, InfixKind Kind, std::unique_ptr<ExpressionNode> Left, std::unique_ptr<ExpressionNode> Right = nullptr);
        
        void accept(ExprVisitor& visitor) const override;
        void accept(ExprMutableVisitor& visitor) override;
};


class NaryExpressionNode : public ExpressionNode {
    public:
        Token Tok;      // +, *
        char Operator;
        InfixKind Kind;
        std::vector<std::unique_ptr<ExpressionNode>> Operands;

        NaryExpressionNode(Token &tok, char Op, InfixKind Kind, std::vector<std::unique_ptr<ExpressionNode>> ops);

        std::string TokenLiteral() override;
        InfixKind getKind() override;
        std::string String() override;

        void accept(ExprVisitor& visitor) const override;
        void accept(ExprMutableVisitor& visitor) override;
};



#endif