/*
18/2/25
*/

#include "ast.hpp"

class NumberExpressionNode;
class VariableExpressionNode;
class PrefixExpressionNode;
class InfixExpressionNode;

// Base Visitor (Mutable for Simplification stage where ast will be transformed)
struct ExprMutableVisitor {
    ExprMutableVisitor() = default;
    virtual ~ExprMutableVisitor() = default;

    virtual void visit(NumberExpressionNode&) = 0;
    virtual void visit(VariableExpressionNode&) = 0;
    virtual void visit(PrefixExpressionNode&) = 0;
    virtual void visit(InfixExpressionNode&) = 0;
};

struct ExprVisitor {
    ExprVisitor() = default;
    virtual ~ExprVisitor() = default;

    virtual void visit(const NumberExpressionNode&) = 0;
    virtual void visit(const VariableExpressionNode&) = 0;
    virtual void visit(const PrefixExpressionNode&) = 0;
    virtual void visit(const InfixExpressionNode&) = 0;
};