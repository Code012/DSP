/*
parse.hpp

Author: 
Data: 25/01/2025
*/
// for writing parser: https://llvm.org/docs/tutorial/MyFirstLanguageFrontend/LangImpl02.html
//                      https://matklad.github.io/2020/04/13/simple-but-powerful-pratt-parsing.html
//                      https://journal.stuffwithstuff.com/2011/03/19/pratt-parsers-expression-parsing-made-easy/ 
//  https://www.youtube.com/watch?v=2l1Si4gSb9A helped in intuitvely understanding pratt parsing
//TODO: include copyright
// // g++ -Wall -std=c++20 -g -O0 -mconsole -o BIN/main Mathly/main.cpp Mathly/lexer.cpp

#include <unordered_map>
#include <vector>
#include <cstdint>
#include "ast.hpp"
#include "lexer.hpp"
#include "token.hpp"

const int LOWEST = 0;
const int ADDITIVE = 10;
const int MULTIPLICATIVE = 20;
const int UNARY = 30;
const int IMPLICIT_MULT = 21;

// left binding power
static std::map<TokenType, int> precedenceList = {
    {token::PLUS, ADDITIVE},
    {token::MINUS, ADDITIVE},
    {token::DIV, MULTIPLICATIVE},
    {token::MULT, MULTIPLICATIVE},
    {token::IMPLICIT_MULT, IMPLICIT_MULT}
};


class ExpressionNode;
class Parser;
uint64_t str_to_u64(std::string new_str);

//function pointer types for LEDs and NUDs
using prefixParseFn = std::unique_ptr<ExpressionNode> (Parser::*)(void); 
using infixParseFn = std::unique_ptr<ExpressionNode> (Parser::*)(std::unique_ptr<ExpressionNode> );

class Parser {
    public:
        Lexer lexer;
        std::vector<std::string> errors;


        Token curToken;
        Token peekToken;

        
        

        std::unordered_map<TokenType, prefixParseFn> prefixParseFnList;
        std::unordered_map<TokenType, infixParseFn> infixParseFnList;

        Parser(Lexer& l) : lexer(l) {
            lexer.lex();

            registerPrefix(token::VAR, parseVariable);
            registerPrefix(token::INT, parseNumber);
            registerPrefix(token::MINUS, parsePrefixExpression);
            registerPrefix(token::LPAREN, parseGroupedExpression);
            // registerPrefix(token::VAR_NUM, parseVarNumPair);
            // registerPrefix(token::NUM_VAR, parseVarNumPair);
            

            
            registerInfix(token::PLUS, parseNaryExpression);
            registerInfix(token::MULT, parseNaryExpression);
            registerInfix(token::IMPLICIT_MULT, parseNaryExpression);
            registerInfix(token::MINUS, parseInfixExpression);
            registerInfix(token::DIV, parseInfixExpression);


            nextToken();
            nextToken();

            
        };

        void nextToken() {
            curToken = peekToken;
            peekToken = lexer.getNextToken();
        }

        bool curTokenIs(TokenType t) {
            return curToken.Type == t;
        }

        bool peekTokenIs(TokenType t) {
            return peekToken.Type == t;
        }

        bool match(TokenType t) {
            if (peekTokenIs(t)) {
                nextToken();
                return true;
            } else {
                peekError(t);
                return false;
            }
        }

        std::vector<std::string> getErrors() {
            return errors;
        };

        void peekError(TokenType t) {
            std::string msg = "expected next token to be " + t + ", got " + peekToken.Type + " instead";
        };

        void noPrefixParseFnError(TokenType t) {
            std::string msg = "no prefix parse funtion for " + t + " found";
            errors.push_back(msg);
        }

        std::unique_ptr<ExpressionNode> parseLoop() {
            std::unique_ptr<ExpressionNode> expr;
            // while(curTokenIs(token::EOL) == false) {
                expr = parseMain();
                // nextToken();
            // }

            return expr;
        }

        std::unique_ptr<ExpressionNode> parseMain() {
            // if (curToken.Type == token::FUNC) { // for future
            //     return parseFunc();
            // } else {
            //     return HandleExpression();
            // }

            return HandleExpression();
        }

        // std::unique_ptr<ExpressionNode> parseFunc() {
        //     return;
        // }

        std::unique_ptr<ExpressionNode> HandleExpression() {
            auto expr = parseExpression(LOWEST);

            return expr;
            
        };

        /*
        First parse the NUD,
        While we haven't reached the end and the bp of the next token is > bp of current token
            Continue parsing left hand side
        */
        std::unique_ptr<ExpressionNode> parseExpression(int precedence) {


            // If the relevant prefix function (NUD) exists for the token, then
            // then get the function pointer to it and call it and store in leftExpr
            std::unique_ptr<ExpressionNode> leftExpr;


            if (prefixParseFnList.count(curToken.Type)) { 

                prefixParseFn prefix = prefixParseFnList.at(curToken.Type); // NUD
                leftExpr = (this->*prefix)();   // took a while to figure out. have to dereference pointer first

            } else {

                noPrefixParseFnError(curToken.Type);
                return nullptr;

            }

            // Check for implicit multiplciation of coefficient-variable pairs
            if ((curTokenIs(token::INT) && peekTokenIs(token::VAR)) ||  // 2x, 12x, 1232x
               ( (curTokenIs(token::INT) || curTokenIs(token::VAR)) && peekTokenIs(token::LPAREN))) {   // 2(), x()
                std::string ch = "*";
                peekToken = insertToken(ch, token::IMPLICIT_MULT);
            }


            // While we haven't reached the end, or the next token bp > current token bp
            // Continue parsing the left hand side
            while ((!peekTokenIs(token::EOL)) && (precedence < peekPrecedence())) {
                infixParseFn infix; //LED
                
                if (infixParseFnList.count(peekToken.Type)) {
                    infix = infixParseFnList.at(peekToken.Type);

                } else {
                    return leftExpr;
                }

                nextToken();



                leftExpr = (this->*infix)(std::move(leftExpr));
            }

            return leftExpr;

            
        }

        Token insertToken(std::string& ch, TokenType type) {
            lexer.insertToken(ch, type);
            return Token{type, ch};
        }
       

        int peekPrecedence() {


            if (precedenceList.count(peekToken.Type)) {
                return precedenceList.at(peekToken.Type);
            } else {
                return LOWEST;
            }
        }

        int currentPrecedence() {
            if (precedenceList.count(curToken.Type)) {
                return precedenceList.at(curToken.Type);
            } else {
                return LOWEST;
            }
        }


        std::unique_ptr<ExpressionNode> parseVariable () {
            // std::cout << curToken.Literal << std::endl;
            return std::make_unique<VariableExpressionNode>(curToken.Literal, curToken, InfixKind::VAR);
        }
        // ExpressionNode* parsePrefixExpression() {
        //     expr = PrefixExpressionNode();
        // }

        std::unique_ptr<ExpressionNode> parseNumber() {

            //TODO: perhaps some error handling for conversion
            return std::make_unique<NumberExpressionNode>(curToken, std::stoi(curToken.Literal), InfixKind::NUM);
        }

        std::unique_ptr<ExpressionNode> parsePrefixExpression() { 
            // operand first
            std::unique_ptr<PrefixExpressionNode> expr = std::make_unique<PrefixExpressionNode>(*(curToken.Literal.c_str()), curToken, InfixKind::PRE_MINUS);
            nextToken(); // consume prefix operand

            // now number
            expr->Right = parseExpression(UNARY);

            return expr;
        }

        std::unique_ptr<ExpressionNode> parseInfixExpression(std::unique_ptr<ExpressionNode> left) {
            //first left and operand

            InfixKind kind;
            if (curToken.Type == token::MINUS) { kind = InfixKind::DIFFERENCE; }
            else if (curToken.Type == token::DIV) { kind = InfixKind::DIVIDE; }
                

            std::unique_ptr<InfixExpressionNode> expr = std::make_unique<InfixExpressionNode>(curToken, *(curToken.Literal.c_str()), kind, std::move(left), nullptr);

            int precedence = currentPrecedence(); // get lbp
            nextToken();
            expr->Right = parseExpression(precedence);

            return expr;
        }

        std::unique_ptr<ExpressionNode> parseNaryExpression(std::unique_ptr<ExpressionNode> left) {
            //first left and operand

            InfixKind kind;
            if (curToken.Type == token::PLUS) { kind = InfixKind::PLUS;} 
            else if (curToken.Type == token::MULT || curToken.Type == token::IMPLICIT_MULT ) { kind = InfixKind::MULTIPLY; }
                
            std::vector<std::unique_ptr<ExpressionNode>> operands;
            operands.push_back(std::move(left));

            std::unique_ptr<NaryExpressionNode> expr = std::make_unique<NaryExpressionNode>(curToken, *(curToken.Literal.c_str()), kind, std::move(operands));

            int precedence = currentPrecedence(); // get lbp
            nextToken();
            expr->Operands.push_back(parseExpression(precedence));

            return expr;
        }

        std::unique_ptr<ExpressionNode> parseGroupedExpression() {
            nextToken();

            auto expr = parseExpression(LOWEST);

            if (!match(token::RPAREN)) {
                return nullptr;
            }

            return expr;
        }



        void registerPrefix(TokenType toktype, prefixParseFn fn) {
            prefixParseFnList.emplace(toktype, fn);
        }   

        void registerInfix(TokenType toktype, infixParseFn fn) {
            infixParseFnList.emplace(toktype, fn);
        }   


};

// Helper Functions

// https://stackoverflow.com/questions/42356939/c-convert-string-to-uint64-t?noredirect=1&lq=1
// Avizipi, 0xt 12 2021, accessed feb 5 2025
uint64_t str_to_u64(std::string new_str) {
    uint64_t val = 0;
    for (auto ch : new_str) {
        if (not isNumber(ch)) return 0;
        val = 10 * val + (ch - '0');
    }
    return val;
}
