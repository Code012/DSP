#ifndef SIMPLIFIER_HPP
#define SIMPLIFIER_HPP

#include "ast.hpp"
#include "visitors.hpp"
#include <memory>
#include <algorithm>

class SimplifyVisitor : public ExprMutableVisitor {
    public:
        SimplifyVisitor() = default;

        void visit(NumberExpressionNode& node) override;
        void visit(VariableExpressionNode& node) override;
        void visit(PrefixExpressionNode& node) override;
        void visit(InfixExpressionNode& node) override;
        void visit(NaryExpressionNode& node) override;

        
        std::unique_ptr<ExpressionNode> getResult() { return std::move(result); }
        std::unique_ptr<ExpressionNode> expand_tree(std::unique_ptr<ExpressionNode>& expr);
        std::unique_ptr<ExpressionNode> clone_expr(std::unique_ptr<ExpressionNode>& expr);
        void rearrange_left(std::unique_ptr<ExpressionNode>& left, std::unique_ptr<ExpressionNode>& right);
        void rearrange_right(std::unique_ptr<ExpressionNode>& left, std::unique_ptr<ExpressionNode>& right);
        void solve_x(std::unique_ptr<ExpressionNode>& left, std::unique_ptr<ExpressionNode>& right);

        ~SimplifyVisitor() = default;

    private:
        std::unique_ptr<ExpressionNode> result;

        // keeping the functions names and case the same as in the book so I know whats from the book and not
        std::unique_ptr<ExpressionNode> automatic_simplify(std::unique_ptr<ExpressionNode> expr); //pg 92
        std::unique_ptr<ExpressionNode> simplify_product(std::unique_ptr<NaryExpressionNode> product); // p.g.97
        std::vector<std::unique_ptr<ExpressionNode>> simplify_product_rec(std::vector<std::unique_ptr<ExpressionNode>>& operands); //p.g. 98
        std::vector<std::unique_ptr<ExpressionNode>> merge_products( std::vector<std::unique_ptr<ExpressionNode>>& p, std::vector<std::unique_ptr<ExpressionNode>>& q); // p.g. 102

        std::unique_ptr<ExpressionNode> simplify_sum(std::unique_ptr<NaryExpressionNode> sum);
        std::vector<std::unique_ptr<ExpressionNode>> simplify_sum_rec(std::vector<std::unique_ptr<ExpressionNode>>& operands);
        std::vector<std::unique_ptr<ExpressionNode>> merge_sums( std::vector<std::unique_ptr<ExpressionNode>>& p, std::vector<std::unique_ptr<ExpressionNode>>& q); // p.g. 102

        void extractBaseAndCoefficient(ExpressionNode* expr, std::unique_ptr<ExpressionNode>& base_out, std::unique_ptr<ExpressionNode>& coef_out);
        bool isPlusNodeChild(const std::vector<std::unique_ptr<ExpressionNode>>& operands);

        bool isUndefined(const ExpressionNode* node) const;

        bool isZero(const ExpressionNode*) const;

        bool isOne(const ExpressionNode* node) const;
        std::unique_ptr<NaryExpressionNode> createProduct(std::vector<std::unique_ptr<ExpressionNode>> operands);
        std::unique_ptr<NaryExpressionNode> createSum(std::vector<std::unique_ptr<ExpressionNode>> operands) const;
        std::unique_ptr<InfixExpressionNode> createQuotient(std::unique_ptr<ExpressionNode> left, std::unique_ptr<ExpressionNode> right);
        std::unique_ptr<InfixExpressionNode> createDifference(std::unique_ptr<ExpressionNode> left, std::unique_ptr<ExpressionNode> right);
        std::unique_ptr<NumberExpressionNode> createNumber(int value); 
        std::unique_ptr<VariableExpressionNode> createVariable(const std::string& value) const;


        bool isProduct(const ExpressionNode* node) const;
        bool isSum(const ExpressionNode* node) const;


        std::vector<std::unique_ptr<ExpressionNode>> getNaryOperands(NaryExpressionNode* expr);

        // New methods for RNE
        std::unique_ptr<ExpressionNode> simplify_rational_number(std::unique_ptr<ExpressionNode> fraction);
        std::unique_ptr<ExpressionNode> simplify_rne(std::unique_ptr<ExpressionNode> expr);
        std::unique_ptr<ExpressionNode> simplify_rne_rec(std::unique_ptr<ExpressionNode> expr);
        std::unique_ptr<ExpressionNode> evaluate_product(std::unique_ptr<ExpressionNode> left, std::unique_ptr<ExpressionNode> right);
        std::unique_ptr<ExpressionNode> evaluate_quotient(std::unique_ptr<ExpressionNode> left, std::unique_ptr<ExpressionNode> right);
        std::unique_ptr<ExpressionNode> evaluate_sum(std::unique_ptr<ExpressionNode> left, std::unique_ptr<ExpressionNode> right);
        std::unique_ptr<ExpressionNode> evaluate_difference(std::unique_ptr<ExpressionNode> left, std::unique_ptr<ExpressionNode> right);
        std::unique_ptr<InfixExpressionNode> createFraction(int numerator, int denominator);
        bool isInteger(const ExpressionNode* node);
        int getintValue(const ExpressionNode* node) const;
        int getNumerator(const ExpressionNode* node) const;
        int getDenominator(const ExpressionNode* node) const;
        int gcd(int a, int b);


        // order relation, decides order of simplified expressions (commutative transformation)
        bool operandLessThan(const ExpressionNode* u, const ExpressionNode* v) const; //p.g.84

        std::vector<std::unique_ptr<ExpressionNode>> rest(std::vector<std::unique_ptr<ExpressionNode>>& a);


        

};


// entry point function that calls visitor
std::unique_ptr<ExpressionNode> automatic_simplify(std::unique_ptr<ExpressionNode> expr);

#endif