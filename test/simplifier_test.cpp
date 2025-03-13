#include <iostream>
#include "..\simplifier.hpp"
// g++ -Wall -std=c++20 -g -O0 -Isimpletest -mconsole -o BIN/simp_test Mathly/test/simplifier_test.cpp Mathly/ast.cpp Mathly/visitors.cpp Mathly/simplifier.cpp
// Helper function to create a number node
std::unique_ptr<ExpressionNode> makeNumber(double value) {
    Token tok{token::INT, std::to_string(value)};
    return std::make_unique<NumberExpressionNode>(tok, value, InfixKind::NUM);
}

// Helper function to create a variable node
std::unique_ptr<ExpressionNode> makeVariable(const std::string& name) {
    Token tok{token::VAR, name};
    return std::make_unique<VariableExpressionNode>(name, tok, InfixKind::VAR);
}

// Helper function to create a product node
std::unique_ptr<ExpressionNode> makeProduct(std::vector<std::unique_ptr<ExpressionNode>> operands) {
    Token tok{token::MULT, "*"};
    return std::make_unique<NaryExpressionNode>(tok, '*', InfixKind::MULTIPLY, std::move(operands));
}

std::unique_ptr<ExpressionNode> makeSum(std::vector<std::unique_ptr<ExpressionNode>> operands) {
    Token tok{token::PLUS, "+"};
    return std::make_unique<NaryExpressionNode>(tok, '+', InfixKind::PLUS, std::move(operands));
}

std::unique_ptr<InfixExpressionNode> makeDivide(double numerator, double denominator) {
    Token fracTok{token::DIV, "/"};
    auto num_node = makeNumber(numerator);
    auto denom_node = makeNumber(denominator);

    
    return std::make_unique<InfixExpressionNode>(
        fracTok,
        '/',
        InfixKind::DIVIDE, // Special kind for fractions, Defnition 2.26 , p.g. 38
        std::move(num_node),
        std::move(denom_node)
    );
}

std::unique_ptr<InfixExpressionNode> makeDivide(const char* numerator, const char* denominator) {
    Token fracTok{token::DIV, "/"};
    auto num_node = makeVariable(numerator);
    auto denom_node = makeVariable(denominator);

    
    return std::make_unique<InfixExpressionNode>(
        fracTok,
        '/',
        InfixKind::DIVIDE, // Special kind for fractions, Defnition 2.26 , p.g. 38
        std::move(num_node),
        std::move(denom_node)
    );
}

int main() {
    // // Test case 1: Simple product a * 2 * b
    // std::vector<std::unique_ptr<ExpressionNode>> operands1;
    // std::vector<std::unique_ptr<ExpressionNode>> operands200;
    // std::vector<std::unique_ptr<ExpressionNode>> operands100;
    // operands1.push_back(makeVariable("a"));

    // operands1.push_back(makeVariable("b"));
    // auto expr1 = makeProduct(std::move(operands1));

    // operands200.push_back(makeVariable("d"));
    // operands200.push_back(makeVariable("c"));
    // auto expr200 = makeProduct(std::move(operands200));

    

    // operands100.push_back(std::move(expr200));
    // operands100.push_back(std::move(expr1));
    
    // auto expr100 = makeProduct(std::move(operands100));
    
    // std::cout << "Original: " << expr100->String() << std::endl;
    // auto simplified1 = automatic_simplify(std::move(expr100));
    // std::cout << "Simplified: " << simplified1->String() << std::endl;
    
    // // Test case 2: Product with 1: a * 1 * b
    // std::vector<std::unique_ptr<ExpressionNode>> operands2;
    // operands2.push_back(makeVariable("b"));
    // operands2.push_back(makeNumber(1));
    // operands2.push_back(makeVariable("a"));
    // auto expr2 = makeProduct(std::move(operands2));
    
    // std::cout << "Original: " << expr2->String() << std::endl;
    // auto simplified2 = automatic_simplify(std::move(expr2));
    // std::cout << "Simplified: " << simplified2->String() << std::endl;
    
    // // Test case 3: Product with 0: a * 0 * b
    // std::vector<std::unique_ptr<ExpressionNode>> operands3;
    // operands3.push_back(makeVariable("a"));
    // operands3.push_back(makeNumber(0));
    // operands3.push_back(makeVariable("b"));
    // auto expr3 = makeProduct(std::move(operands3));
    
    // std::cout << "Original: " << expr3->String() << std::endl;
    // auto simplified3 = automatic_simplify(std::move(expr3));
    // std::cout << "Simplified: " << simplified3->String() << std::endl;
    
    // Test case 4: Nested products: (2 / 4) * (12 * 3)
 
    // auto div1 = makeDivide(7, 13);

    // auto div2 = makeDivide(5, 9);
    

    // auto div3 = makeDivide(39, 15);
    // auto div4 = makeDivide(12, 8);
    // auto div5 = makeDivide("x", "2");
    // auto div6 = makeDivide(1, 1);
    
    
    // std::vector<std::unique_ptr<ExpressionNode>> operands4;
    // operands4.push_back(std::move(div5));
    // operands4.push_back(std::move(div1));
    // operands4.push_back(makeNumber(2));
    // operands4.push_back(std::move(div2));
    // operands4.push_back(std::move(div6));
    // operands4.push_back(std::move(div3));
    // operands4.push_back(std::move(div4));
    // operands4.push_back(makeNumber(16));


    // auto expr4 = makeProduct(std::move(operands4));
    
    // std::cout << "Original: " << expr4->String() << std::endl;
    // auto simplified4 = automatic_simplify(std::move(expr4));
    // std::cout << "Simplified: " << simplified4->String() << std::endl;
    
// ====================

    auto div1p = makeDivide(7, 13);

    auto div2p = makeDivide(5, 9);
    

    auto div3p = makeDivide(39, 15);
    auto div4p = makeDivide(12, 8);
    auto div5p = makeDivide("x", "2");
    auto div6p = makeDivide(1, 1);
    
    
    std::vector<std::unique_ptr<ExpressionNode>> operands4p;
    std::vector<std::unique_ptr<ExpressionNode>> operands5p;

    operands5p.push_back(makeNumber(20));
    operands5p.push_back(makeNumber(10));
    operands5p.push_back(std::move(div1p));


    auto expr5p = makeProduct(std::move(operands5p));

    operands4p.push_back(makeNumber(2));
    operands4p.push_back(makeVariable("x"));
    operands4p.push_back(makeVariable("x"));
    operands4p.push_back(std::move(expr5p));
    // operands4p.push_back(std::move(div1p));
    operands4p.push_back(std::move(div2p));
    operands4p.push_back(std::move(div3p));
    operands4p.push_back(std::move(div4p));
    operands4p.push_back(std::move(div6p));


    auto expr4p = makeSum(std::move(operands4p));
    
    std::cout << "Original: " << expr4p->String() << std::endl;
    auto simplified4p = automatic_simplify(std::move(expr4p));
    std::cout << "Simplified: " << simplified4p->String() << std::endl;

    // Test case 1: Simple product a
    auto expr5 = makeVariable("a");
    
    std::cout << "Original: " << expr5->String() << std::endl;
    auto simplified5 = automatic_simplify(std::move(expr5));
    std::cout << "Simplified: " << simplified5->String() << std::endl;

    // TEST RNE
    auto rne1 = makeDivide(4, 2);
    std::cout << "Original: " << rne1->String() << std::endl;
    auto simplified_rne_1 = automatic_simplify(std::move(rne1));
    std::cout << "Simplified: " << simplified_rne_1->String() << std::endl;



    

    return 0;
}