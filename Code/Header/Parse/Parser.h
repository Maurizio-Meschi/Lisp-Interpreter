#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include <iostream>

#include "../Exceptions/Exceptions.h"
#include "../Token/Token.h"
#include "../Element/NumExpr.h"
#include "../Element_manager/NumExpreManager.h"
#include "../Element/BoolExpr.h"
#include "../Element_manager/BoolExprManager.h"
#include "../Element/Program.h"
#include "../Element_manager/StmtManager.h"
#include "../Element_manager/BlockManager.h"



class ProgramParse {
static constexpr int DEFAULT_CHECK_PARENTESIS = 0;

public:
    ProgramParse(NumExpreManager& n_manager, BoolExpreManager& b_manager, StmtManager& s_manager,
        BlockManager& block_manager) :
        num_em{ n_manager }, bool_em{ b_manager }, stmt_em{ s_manager }, 
        block_em{ block_manager }, check_parentesis{ DEFAULT_CHECK_PARENTESIS } { }

    ProgramParse(const ProgramParse& t) = default;
    ~ProgramParse() = default;

    Program* operator()(const std::vector<Token>& tokenStream) {
        // Checking that the program is not empty
        if (tokenStream.empty())
            throw ParseError("Empty program");
        auto tokenItr = tokenStream.begin();
        streamEnd = tokenStream.end();
        Block* block = recursiveBlockParse(tokenItr);
        // Checking that I have reached the end of the token stream."
        if (tokenItr != streamEnd) {
            throw ParseError("Invalid program");
        }
        Program* program = new Program(block);
        return program;
    }

private:
    // Defining an iterator to check if all tokens have been read
    std::vector<Token>::const_iterator streamEnd;

    // Array to store the statements
    std::vector<Statement*> allocated_stmt;

    // Integer to check that instructions are preceded by an open parenthesis.
    int check_parentesis;

    // References to the "managers"
    NumExpreManager& num_em;
    BoolExpreManager& bool_em;
    StmtManager& stmt_em;
    BlockManager& block_em;

    // Recursive descent parser in the block structure
    Block* recursiveBlockParse(std::vector<Token>::const_iterator& tokenItr);
    // Recursive descent parser in the statement structure
    Statement* recursiveStmtParse(std::vector<Token>::const_iterator& tokenItr);
    //Recursive descent parser in the boolean expression structure
    BoolExpr* recursiveBoolParse(std::vector<Token>::const_iterator& tokenItr);
    // Recursive descent parser in the numeric expression structure
    NumExpr* recursiveNumParse(std::vector<Token>::const_iterator& tokenItr);

    // Safe advancement of an iterator
    void safe_next(std::vector<Token>::const_iterator& itr) {
        if (itr == streamEnd) {
            throw ParseError("Unexpected end of input");
        }
        ++itr;
    }

    // Check to verify the presence of an overflow
    void check_overflow(std::vector<Token>::const_iterator& itr) {
        if (itr == streamEnd)
            throw ParseError("Overflow in token stream.");
    }
};

#endif