/*
18/2/25
*/

#ifndef VISITOR_CPP
#define VISITOR_CPP

class ExprVisitor;
class ExprMutableVisitor;
class AssociativeTransformation;

#include "ast.hpp"




// Base Visitor (Mutable for Simplification stage where ast will be transformed)
struct ExprMutableVisitor {
    ExprMutableVisitor() = default;
    virtual ~ExprMutableVisitor() = default;

    virtual void visit(NumberExpressionNode&) = 0;
    virtual void visit(VariableExpressionNode&) = 0;
    virtual void visit(PrefixExpressionNode&) = 0;
    virtual void visit(InfixExpressionNode&) = 0;
    virtual void visit(NaryExpressionNode&) = 0;

};

struct ExprVisitor {
    ExprVisitor() = default;
    virtual ~ExprVisitor() = default;

    virtual void visit(const NumberExpressionNode&) = 0;
    virtual void visit(const VariableExpressionNode&) = 0;
    virtual void visit(const PrefixExpressionNode&) = 0;
    virtual void visit(const InfixExpressionNode &) = 0;
    virtual void visit(const NaryExpressionNode &) = 0;

};

// -------------CAS FUNCTIONS--------------------

// Flattening nested operators, ((((2 + 2) + 3) * 4 ) * 4 ) -> ((2 + 2 + 3) * 4 * 4)
class AssociativeTransformationVisitor : public ExprMutableVisitor {
    public:
        void visit(NumberExpressionNode& node) override;
        void visit(VariableExpressionNode& node) override;
        void visit(PrefixExpressionNode& node) override;
        void visit(InfixExpressionNode& node) override;
        void visit(NaryExpressionNode& node) override;
    
    private: 
        void flattenOperands(std::unique_ptr<ExpressionNode>& node, InfixKind kind, std::vector<std::unique_ptr<ExpressionNode>>& flattenedOperands);
           
};



#endif        