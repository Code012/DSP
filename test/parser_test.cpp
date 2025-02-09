/*
parser_test.cpp

*/


// to run: g++ -Wall -std=c++20 -g -O0 -Isimpletest -mconsole -o BIN/parser_test Mathly/test/parser_test.cpp

#include <vector>

#include "..\lexer.hpp"
#include "..\parser.hpp"
#include "..\..\simpletest\simpletest.h"


int checkParserErrors(const Parser& p);
bool testNumber(std::unique_ptr<ExpressionNode>& exp, double num, const char* tokLiteral);

DEFINE_TEST(TestNumberExpression) {
    std::string input = "1928378412$";

    Lexer lexer = Lexer(input);
    Parser parser = Parser(lexer);
    std::unique_ptr<ExpressionNode> parsedExpr = parser.parseLoop();
    NumberExpressionNode* convertedparsedExpr = dynamic_cast<NumberExpressionNode*>(parsedExpr.get());  // dynamic casting to access class members
    int errorCode = checkParserErrors(parser);
    TEST_EQ(errorCode, 0);
    
    
    // Test if the VariableExpressionNode AST node has a value of "area"
    TEST( convertedparsedExpr->Value == 1928378412 );

    // Test if the Tok.Literal attribute is "area"
    TEST( parsedExpr->TokenLiteral() == "1928378412" );
}

DEFINE_TEST(TestVariableExpression) {
    std::string input = "area$";

    Lexer lexer = Lexer(input);
    Parser parser = Parser(lexer);
    std::unique_ptr<ExpressionNode> parsedExpr = parser.parseLoop();
    VariableExpressionNode* convertedparsedExpr = dynamic_cast<VariableExpressionNode*>(parsedExpr.get());  // dynamic casting to access class members
    int errorCode = checkParserErrors(parser);
    TEST_EQ(errorCode, 0);
    

    // Test if the VariableExpressionNode AST node has a value of "area"
    TEST( convertedparsedExpr->Value == "area" );

    // Test if the Tok.Literal attribute is "area"
    TEST( parsedExpr->TokenLiteral() == "area" );
}

DEFINE_TEST(TestPrefixExpression) {
    std::string input = "-20$";

    Lexer lexer = Lexer(input);
    Parser parser = Parser(lexer);
    std::unique_ptr<ExpressionNode> parsedExpr = parser.parseLoop();
    PrefixExpressionNode* convertedparsedExpr = dynamic_cast<PrefixExpressionNode*>(parsedExpr.get());  // dynamic casting to access class members
    int errorCode = checkParserErrors(parser);
    TEST_EQ(errorCode, 0);
    
    // Test "negative" prefix operator
    TEST( convertedparsedExpr->Operator == '-' );
    
    // Test number
    TEST( testNumber(convertedparsedExpr->Right, 20, "20") );

    // Test tokenLiteral correctly parsed and stored, which is the prefix operator
    TEST( parsedExpr->TokenLiteral() == "-" );
}

    DEFINE_TEST(TestParsingInfixExpressions) {

        struct testInfix_s {
            std::string input;
            double leftValue;
            std::string op;
            double rightValue;
        };

        std::vector<testInfix_s> testTable;

        testTable.push_back(testInfix_s{"5 + 5$", 5, "+", 5});
        testTable.push_back(testInfix_s{"5 - 5$", 5, "-", 5});
        testTable.push_back(testInfix_s{"5 * 5$", 5, "*", 5});
        testTable.push_back(testInfix_s{"5 / 5$", 5, "/", 5});
        

        for (size_t i=0; i < testTable.size(); i++) {
            const testInfix_s test = testTable[i];
            std::string input = test.input;

            Lexer lexer = Lexer(input);
            Parser parser = Parser(lexer);
            std::unique_ptr<ExpressionNode> parsedExpr = parser.parseLoop();
            InfixExpressionNode* convertedparsedExpr = dynamic_cast<InfixExpressionNode*>(parsedExpr.get());  // dynamic casting to access class members
            int errorCode = checkParserErrors(parser);
            TEST_EQ(errorCode, 0);

            
            // Test left number
            TEST( testNumber(convertedparsedExpr->Left, test.leftValue, "5") );

            // Test operator
            TEST_EQ( convertedparsedExpr->Operator, *(test.op.c_str()) );

            // Test right number
            TEST( testNumber(convertedparsedExpr->Right, test.rightValue, "5") );          
        }  



    }

bool testNumber(std::unique_ptr<ExpressionNode>& exp, double num, const char* tokLiteral) {
    NumberExpressionNode* numbExp = dynamic_cast<NumberExpressionNode*>(exp.get());

    if (numbExp->Value == num && numbExp->TokenLiteral() == tokLiteral)
        return true;
    
    return false;
}

int checkParserErrors(const Parser& p) {
    std::vector<std::string> errors = p.errors;

    if (errors.size() == 0)
        return 0;
    
    std::cerr << "\n\nparser has " << errors.size() << " error[s]" << std::endl;

    for (const auto& i : errors) {
        std::cerr << "\nparser error: " << i;
    }
    std::cerr << std::endl;

    return 1;

}

int main() {
    
    bool allTestsPassed = true;

    // Execute all tests
    allTestsPassed &= TestFixture::ExecuteAllTests(TestFixture::Verbose);

    return allTestsPassed ? 0 : 1;
}