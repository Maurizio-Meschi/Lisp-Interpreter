#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "Header/Token/Token.h"
#include "Header/Token/Tokenizer.h"
#include "Header/Element/NumExpr.h"
#include "Header/Element/BoolExpr.h"
#include "Header/Element_manager/NumExpreManager.h"
#include "Header/Element_manager/BoolExprManager.h"
#include "Header/Parse/Parser.h"
#include "Header/Visitor/Visitor.h"
#include "Header/Variable/VariableTable.h"

int main(int argc, char* argv[])
{
    // Checking to have enough parameters.
    if (argc < 2) {
        std::cerr << "(ERROR: Unspecified file!" << std::endl;
        std::cerr << "Try: " << argv[0] << " <file_name> )" << std::endl;
        return EXIT_FAILURE;
    }
    std::ifstream inputFile;
    // The name provided by the user may not correspond to an existing file
    // I handle this possibility
    try {
        inputFile.open(argv[1]);
        if (inputFile.fail())
            throw std::runtime_error(argv[1]);
    }
    catch (std::exception& exc) {
        std::cerr << "(ERROR: unable to open file '";
        std::cerr << exc.what() << "' )" << std::endl;
        return EXIT_FAILURE;
    }
    
    // Transforming the read file into a sequence of tokens
    Tokenizer tokenize;
    std::vector<Token> inputTokens;

    try {
        inputTokens = std::move(tokenize(inputFile));
        inputFile.close();
    }
    catch (LexicalError& le) {
        std::cerr << "(ERROR in lexical analysis: ";
        std::cerr << le.what() << " )" << std::endl;
        return EXIT_FAILURE;
    }
    catch (std::exception& exc) {
        std::cerr << "(ERROR: Can't read from " << argv[1] << " )" << std::endl;
        std::cerr << exc.what() << std::endl;
        return EXIT_FAILURE;
    }

    // Creating the node managers
    NumExpreManager n_manager;
    BoolExpreManager b_manager;
    StmtManager s_manager;
    BlockManager block_manager;

    // Creating the function object for parsing
    ProgramParse parse{ n_manager, b_manager, s_manager, block_manager };

    // Creating the variable table.
    VariableTable table;
    
    try {
        Program* program = parse(inputTokens);
        EvaluationVisitor* v = new EvaluationVisitor(table);
        program->accept(v);
    }
    catch (ParseError& pe) {
        std::cerr << "(ERROR in parser: ";
        std::cerr << pe.what() << " )" << std::endl;
        return EXIT_FAILURE;
    }
    catch (EvaluationError& ev) {
        std::cerr << "(ERROR in evaluator: ";
        std::cerr << ev.what() << " )" << std::endl;
        return EXIT_FAILURE;
    }
    catch (std::exception& exc) {
        std::cerr << "(ERROR: ";
        std::cerr << exc.what() << " )" << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}