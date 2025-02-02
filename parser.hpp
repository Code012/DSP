/*
parse.hpp

Author: 
Data: 25/01/2025
*/
//* IMPORTANT FIX
// * https://stackoverflow.com/questions/20187587/what-is-the-difference-between-dynamic-dispatch-and-late-binding-in-c
// * Dynamic dispatch is what you are looking for
//* Also methods have to be virtual inorder for them to be overwritten
// // g++ -Wall -std=c++20 -g -O0 -mconsole -o BIN/main Mathly/main.cpp Mathly/lexer.cpp
// you have to use visitor pattern to allow nodes to keep their types.

#include <unordered_map>
#include <vector>
#include "ast.hpp"
#include "lexer.hpp"
#include "token.hpp"

//* ALSO IMPLEMENT PARSER TRACING AT THE END, LOOK AT THORSTEN

const int LOWEST = 0;
const int SUM = 10;
const int PRODUCT = 20;
const int PREFIX = 30;

// left binding power
static std::map<TokenType, int> precedenceList = {
    {token::PLUS, SUM},
    {token::MINUS, SUM},
    {token::DIV, PRODUCT},
    {token::MULT, PRODUCT},
};

class ExpressionNode;
class Parser;

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
            registerPrefix(token::VAR, parseVariable);
            //registerPrefix(token::MINUS, parsePrefixExpression);

            nextToken();
            nextToken();

            
        };

        // Expression* parse() {
        //     // if (curToken.Type == token::FUNC) {
        //     //     return parseFuncExpression();
        //     // } else {
        //     return parseExpression();
            
        // }

        void nextToken() {
            curToken = peekToken;
            peekToken = lexer.nextToken();
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
            std::string msg = "no prefix aprse funtion for " + t + " found";
            errors.push_back(msg);
        }

        std::unique_ptr<ExpressionNode> parseLoop() {
            std::unique_ptr<ExpressionNode> expr;
            while(curTokenIs(token::EOL) == false) {
                expr = parseMain();
                nextToken();
            }

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

        std::unique_ptr<ExpressionNode> parseExpression(int precedence) {
            if (prefixParseFnList.count(curToken.Type)) {
                prefixParseFn prefix = prefixParseFnList.at(curToken.Type);
                std::unique_ptr<ExpressionNode> leftExpr = (this->*prefix)();   // took a while to figure out. have to dereference pointer first

                while ((peekTokenIs(token::EOL)) && (precedence < peekPrecedence())) {
                    infixParseFn infix;

                    if (infixParseFnList.count(peekToken.Type) == 0) {
                        return leftExpr;
                    } else {
                        infix = infixParseFnList.at(peekToken.Type);
                    }

                    nextToken();

                    leftExpr = (this->*infix)(std::move(leftExpr));
                }

                return leftExpr;

            } else {
                noPrefixParseFnError(curToken.Type);
                return nullptr;
            }
        }

        int peekPrecedence() {
            if (precedenceList.count(peekToken.Type)) {
                return precedenceList.at(peekToken.Type);
            } else {
                return LOWEST;
            }
        }


        std::unique_ptr<ExpressionNode> parseVariable () {
            // std::cout << curToken.Literal << std::endl;
            return std::make_unique<VariableExpressionNode>(curToken.Literal, curToken);
        }
        // ExpressionNode* parsePrefixExpression() {
        //     expr = PrefixExpressionNode();
        // }


        void registerPrefix(TokenType toktype, prefixParseFn fn) {
            prefixParseFnList.emplace(toktype, fn);
        }   

        void registerInfix(TokenType toktype, infixParseFn fn) {
            infixParseFnList.emplace(toktype, fn);
        }   


};
