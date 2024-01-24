#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <sstream>

struct Token{
	// Token definition
	static constexpr int SET   = 0;
	static constexpr int PRINT = 1;
	static constexpr int INPUT = 2;
	static constexpr int IF    = 3;
	static constexpr int WHILE = 4;
	static constexpr int ADD   = 5;
	static constexpr int SUB   = 6;
	static constexpr int MUL   = 7;
	static constexpr int DIV   = 8;
	static constexpr int GT    = 9;
	static constexpr int LT    = 10;
	static constexpr int EQ    = 11;
	static constexpr int AND   = 12;
	static constexpr int OR    = 13;
	static constexpr int NOT   = 14;
	static constexpr int TRUE  = 15;
	static constexpr int FALSE = 16;
	static constexpr int BLOCK = 17;
	static constexpr int NUM   = 18;
	static constexpr int VAR   = 19;
	static constexpr int LP    = 20;
	static constexpr int RP    = 21;

	// Defining a variable to handle keywords
	static constexpr int KEYWORD = 18;

	static constexpr const char* idWord[]{
		"SET", "PRINT", "INPUT", "IF", "WHILE", "ADD", "SUB", "MUL", "DIV", "GT", "LT", "EQ", "AND", "OR", "NOT", "TRUE", "FALSE", "BLOCK", "NUM", "VAR", "(", ")"
	};

	Token(int t, const char* w) : tag { t }, word{ w } { }

	Token(int t, std::string w) : tag { t }, word{ w } { }

	int tag;
	std::string word;
};

std::ostream& operator<<(std::ostream& os, const Token& t);

#endif