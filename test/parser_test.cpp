/*
parser_test.cpp

*/


// to run: g++ -Wall -std=c++20 -g -O0 -Isimpletest -mconsole -o BIN/parser_test Mathly/test/parser_test.cpp

#include <vector>

#include "..\lexer.hpp"
#include "..\parser.hpp"
#include "..\..\simpletest\simpletest.h"


// void checkParserErrors(Parser p);

    DEFINE_TEST(TestVariableExpression) {
        std::string input = "area$";

        Lexer lexer = Lexer(input);
        Parser parser = Parser(lexer);
        std::unique_ptr<ExpressionNode> parsedExpr = parser.parseLoop();
        VariableExpressionNode* convertedparsedExpr = dynamic_cast<VariableExpressionNode*>(parsedExpr.get());  // dynamic casting to access class members
        // checkParserErrors(parser);

        // Test if the VariableExpressionNode AST node has a value of "area"
        TEST( convertedparsedExpr->Value == "area" );

        // Test if the Tok.Literal attribute is "area"
        TEST( parsedExpr->TokenLiteral() == "area" );
    }

void checkParserErrors(Parser p) {

}

int main() {
    
    bool allTestsPassed = true;

    // Execute all tests
    allTestsPassed &= TestFixture::ExecuteAllTests(TestFixture::Verbose);

    return allTestsPassed ? 0 : 1;
}