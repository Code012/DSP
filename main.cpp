#include "lexer.hpp"
// #include "token.hpp"
#include <fstream>


const std::string PROMPT = ">> ";

int main() {

    

    bool running = true;
    std::string input;

    while (running == true) {

        // // Open an output file
        // std::ofstream outputFile("output.txt");
        // if (!outputFile.is_open()) {
        //     std::cerr << "Failed to open file for writing." << std::endl;
        //     return 1;
        // }

        std::cout << PROMPT;
        std::getline(std::cin, input);
        input.append("$");

        if (input == "exit$") {
            running = false;
            break;
        }   
    
        // if (input.back() != '$') {
        //     std::cerr << "End in '$'" << std::endl;
        //     continue;
        // }

        // if (input.empty()) {
        //     std::cout << "No input";
        //     running = false;
        //     break;
        // }

        Lexer lexer = Lexer(input);
        Token tok;

        do {
            tok = lexer.nextToken();
            std::cout << "Token Type: " << tok.Type << ", ";

            int i = 7 - tok.Type.length();
            while (i != 0) {
                i--;
                std::cout << " ";
            }
            std::cout <<  "Literal: " << tok.Literal <<  '\n';

        } while (tok.Type != token::EOL);

        // outputFile.flush();

        // outputFile.close();
        // std::cout << "Tokens written to output.txt" << std::endl;
    }
    
    return 0; 
}
// g++ -Wall -std=c++20 -g -O0 -mconsole -o BIN/main Mathly/main.cpp Mathly/lexer.cpp

