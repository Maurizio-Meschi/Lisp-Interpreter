#include <sstream>

#include "../../Header/Parse/Parser.h"
#include "../../Header/Token/Token.h"
#include "../../Header/Exceptions/Exceptions.h"
#include "../../Header/Element_manager/BlockManager.h"

Block* ProgramParse::recursiveBlockParse(std::vector<Token>::const_iterator& itr) {
	if (check_parentesis == 1 && itr == streamEnd)
		throw ParseError("Overflow in token stream.");

	if (itr->tag == Token::LP) {
		check_parentesis = 1;
		safe_next(itr);
		Block* block = recursiveBlockParse(itr);
		return block;
	}
	// Handling the 'BLOCK' token
	else if (itr->tag == Token::BLOCK && check_parentesis == 1) {
		// After reading the block following a parenthesis, the control variable is restored
		check_parentesis = 0;
		safe_next(itr);
		// Before performing a check, 
		// it is necessary to verify that the token stream is not finished; 
		// otherwise, the 'itr->tag' operation will crash the program
		check_overflow(itr);
		// After reading the BLOCK token, I expect an open parenthesis
		if (itr->tag != Token::LP)
			throw ParseError("Expected a 'statement_list'");
		// Reading all the statements in the block
		while (itr->tag == Token::LP) {
			Statement* tmp = recursiveStmtParse(itr);
			allocated_stmt.push_back(tmp);
			check_overflow(itr);
		}
		// After reading the block, I expect a closed parenthesis
		if (itr->tag != Token::RP)
			throw ParseError("Mismatched parenthesis. BLOCK instruction always ends with ')'.");
		safe_next(itr);
		return block_em.makeBlock(allocated_stmt);
	}
	// Handling the statements
	else if (itr->tag >= Token::SET && itr->tag <= Token::WHILE && check_parentesis == 1) {
		Statement* tmp = recursiveStmtParse(itr);
		allocated_stmt.push_back(tmp);
		return block_em.makeBlock(allocated_stmt);
	}
	else {
		std::stringstream tmp{};
		tmp << "Misplaced token '" << itr->word << "'";
		throw ParseError(tmp.str());
		return nullptr;
	}
}

Statement* ProgramParse::recursiveStmtParse(std::vector<Token>::const_iterator& itr) {
	check_overflow(itr);
	if (itr->tag == Token::LP) {
		check_parentesis = 1;
		safe_next(itr);
		return recursiveStmtParse(itr);
	}
	// Handling the 'SET' token
	else if (itr->tag == Token::SET && check_parentesis == 1) {
		check_parentesis = 0;
		safe_next(itr);
		check_overflow(itr);
		// The next token must be a variable: I perform a check
		if (itr->tag != Token::VAR) {
			std::stringstream tmp{};
			tmp << "Misplaced token '" << itr->word << "'";
			throw ParseError(tmp.str());
		}
		// Reading a variable
		NumExpr* var = recursiveNumParse(itr);
		// Reading a num expr
		NumExpr* expr = recursiveNumParse(itr);
		check_overflow(itr);
		// Checking to have a closed parenthesis
		if (itr->tag != Token::RP)
			throw ParseError("Mismatched parenthesis. SET instruction always ends with ')'");
		safe_next(itr);
		Statement* set = stmt_em.makeStmtSet(dynamic_cast<Variable*>(var), expr);
		return set;
	}
	// Handling the 'PRINT' token
	else if (itr->tag == Token::PRINT && check_parentesis == 1) {
		check_parentesis = 0;
		safe_next(itr);
		// Reading a num expr
		NumExpr* expr = recursiveNumParse(itr);
		check_overflow(itr);
		// Checking to have a closed parenthesis
		if (itr->tag != Token::RP)
			throw ParseError("Mismatched parenthesis. PRINT instruction always ends with ')'");
		safe_next(itr);
		Statement* print = stmt_em.makeStmtPrint(expr);
		return print;
	}
	//  Handling the 'INPUT' token
	else if (itr->tag == Token::INPUT && check_parentesis == 1) {
		check_parentesis = 0;
		safe_next(itr);
		check_overflow(itr);
		// The next token must be a variable: I perform a check
		if (itr->tag != Token::VAR) {
			std::stringstream tmp{};
			tmp << "Misplaced token '" << itr->word << "'";
			throw ParseError(tmp.str());
		}
		// Reading a variable
		NumExpr* var = recursiveNumParse(itr);
		check_overflow(itr);
		// Checking to have a closed parenthesis
		if (itr->tag != Token::RP || itr == streamEnd)
			throw ParseError("Mismatched parenthesis. INPUT instruction always ends with ')'");
	    safe_next(itr);
		Statement* input = stmt_em.makeStmtInput(dynamic_cast<Variable*>(var));
		return input;
	}
	// Handling the 'WHILE' token
	else if (itr->tag == Token::WHILE && check_parentesis == 1) {
		check_parentesis = 0;
		safe_next(itr);
		// Reading a bool expr
		BoolExpr* Bexpr = recursiveBoolParse(itr);

		// I temporarily save the vector and proceed to read a block
		std::vector<Statement*> tmp;
		tmp = allocated_stmt;
		allocated_stmt.clear();
		Block* block = recursiveBlockParse(itr);
		// Restoring the vector associated with the previous block
		allocated_stmt = tmp;
		check_overflow(itr);
		// Checking to have a closed parenthesis
		if (itr->tag != Token::RP)
			throw ParseError("Mismatched parenthesis. WHILE instruction always ends with ')'");
		safe_next(itr);
		Statement* While = stmt_em.makeStmtWhile(Bexpr, block);
		return While;
	}
	// Handling the 'IF' token
	else if (itr->tag == Token::IF && check_parentesis == 1) {
		check_parentesis = 0;
		safe_next(itr);
		// Reading a bool expr
		BoolExpr* Bexpr = recursiveBoolParse(itr);
		// I temporarily save the vector and proceed to read a block
		std::vector<Statement*> tmp;
		tmp = allocated_stmt;
		allocated_stmt.clear();
		Block* left = recursiveBlockParse(itr);

		// Emptying the vector and reading a new block
		allocated_stmt.clear();
		Block* right = recursiveBlockParse(itr);

		// Restoring the vector associated with the initial block
		allocated_stmt = tmp;
		check_overflow(itr);
		// Checking to have a closed parenthesis
		if (itr->tag != Token::RP)
			throw ParseError("Mismatched parenthesis. IF instruction always ends with ')'");
		safe_next(itr);
		Statement* If = stmt_em.makeStmtIf(left, right, Bexpr);
		return If;
	}
	else {
		std::stringstream tmp{};
		tmp << "Misplaced token '" << itr->word << "'";
		throw ParseError(tmp.str());
		return nullptr;
	}
}

NumExpr* ProgramParse::recursiveNumParse(std::vector<Token>::const_iterator& itr) {
	check_overflow(itr);

	if (itr->tag == Token::LP) {
		check_parentesis = 1;
		safe_next(itr);
		return recursiveNumParse(itr);
	}
	// Handling the tokens 'ADD, SUB, MUL, DIV'
	else if (itr->tag >= Token::ADD && itr->tag <= Token::DIV && check_parentesis == 1) {
		check_parentesis = 0;
		Operator::OpCode op;
		switch (itr->tag) {
		case Token::ADD: op = Operator::ADD;  break;
		case Token::SUB: op = Operator::SUB; break;
		case Token::MUL: op = Operator::MUL; break;
		case Token::DIV: op = Operator::DIV;   break;
		default:
			throw ParseError("Unrecognized operator");
		}
		safe_next(itr);
		// Reading the first expr
		NumExpr* left  = recursiveNumParse(itr);
		// Reading the second expr
		NumExpr* right = recursiveNumParse(itr);
		check_overflow(itr);
		// Checking to have a closed parenthesis
		if (itr->tag != Token::RP) {
			std::stringstream tmp{};

			tmp << "Mismatched parenthesis. " 
				<< Operator::opCodeToString(op) 
				<< " instruction always ends with ')'";

			throw ParseError(tmp.str());
		}
		safe_next(itr);
		return num_em.makeOperator(op, left, right);
	}
	// Handling the numbers
	else if (itr->tag == Token::NUM) {
		std::stringstream tmp{};
		tmp << itr->word;
		int64_t value;
		tmp >> value;
		NumExpr* expr = num_em.makeNumber(value);
		safe_next(itr);
		return expr;
	}
	// Handling the variables
	else if (itr->tag == Token::VAR) {
		NumExpr* var = num_em.makeVariable(itr->word);
		safe_next(itr);
		return var;
	}
	else {
		std::stringstream tmp{};
		tmp << "Misplaced token '" << itr->word << "'";
		throw ParseError(tmp.str());
		return nullptr;
	}
}

BoolExpr* ProgramParse::recursiveBoolParse(std::vector<Token>::const_iterator& itr) {
	check_overflow(itr);

	if (itr->tag == Token::LP) {
		check_parentesis = 1;
		safe_next(itr);
		return recursiveBoolParse(itr);
	}
	// Handling the tokens "GT, LT, EQ"
	else if (itr->tag >= Token::GT && itr->tag <= Token::EQ && check_parentesis == 1) {
		check_parentesis = 0;
		RelBoolOperator::RelOpCode op;
		switch (itr->tag) {
		case Token::GT: op = RelBoolOperator::GT; break;
		case Token::LT: op = RelBoolOperator::LT; break;
		case Token::EQ: op = RelBoolOperator::EQ; break;
		default:
			throw ParseError("Unrecognized operator");
		}
		safe_next(itr);
		// Reading the first expr
		NumExpr* left = recursiveNumParse(itr);
		// Reading the second expr
		NumExpr* right = recursiveNumParse(itr);
		check_overflow(itr);
		// Checking to have a closed parenthesis
		if (itr->tag != Token::RP) {
			std::stringstream tmp{};

			tmp << "Mismatched parenthesis. " 
				<< RelBoolOperator::RelopCodeToString(op) 
				<< " instruction always ends with ')'.";

			throw ParseError(tmp.str());
		}
		safe_next(itr);
		return bool_em.makeRelBoolOp(op, left, right);
	}
	// Handling the tokens "AND, OR, NOT"
	else if (itr->tag >= Token::AND && itr->tag <= Token::NOT && check_parentesis == 1) {
		check_parentesis = 0;
		BoolOperator::OpCodeBool op;
		switch (itr->tag) {
		case Token::AND: op = BoolOperator::AND; break;
		case Token::OR:  op = BoolOperator::OR;  break;
		case Token::NOT: op = BoolOperator::NOT; break;
		default:
			throw ParseError("Unrecognized operator");
		}
		// handling the NOT separately as it requires only one bool expr
		if (itr->tag == Token::NOT) {
			safe_next(itr);
			BoolExpr* left = recursiveBoolParse(itr);
			check_overflow(itr);
			// Checking to have a closed parenthesis
			if (itr->tag != Token::RP)
				throw ParseError("Mismatched parenthesis. NOT instruction always ends with ')'.");
			safe_next(itr);
			return bool_em.makeBoolOp(op, left, nullptr);
		}
		else {
			safe_next(itr);
			BoolExpr* left = recursiveBoolParse(itr);
			BoolExpr* right = recursiveBoolParse(itr);
			check_overflow(itr);
			// Checking to have a closed parenthesis
			if (itr->tag != Token::RP) {
				std::stringstream tmp{};

				tmp << "Mismatched parenthesis. " 
					<< BoolOperator::opCodeToString(op) 
					<< " instruction always ends with ')'.";

				throw ParseError(tmp.str());
			}
			safe_next(itr);
			return bool_em.makeBoolOp(op, left, right);
		}
		
	}
	// Handling the 'TRUE' token
	else if (itr->tag == Token::TRUE) {
		BoolExpr* constBool = bool_em.makeConstBoolOp(ConstBool::TRUE);
		safe_next(itr);
		return constBool;
	}
	// Handling the 'FALSE' token
	else if (itr->tag == Token::FALSE) {
		BoolExpr* constBool = bool_em.makeConstBoolOp(ConstBool::FALSE);
		safe_next(itr);
		return constBool;
	}
	else {
		std::stringstream tmp{};
		tmp << "Misplaced token '" << itr->word << "'";
		throw ParseError(tmp.str());
		return nullptr;
	}
}