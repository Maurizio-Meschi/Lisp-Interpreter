#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <vector>
#include <fstream>

#include "Token.h"

class Tokenizer {
public:
	Tokenizer() = default;
	Tokenizer(const Tokenizer& t) = default;
	~Tokenizer() = default;
	std::vector<Token> operator()(std::ifstream& InFile) {
		std::vector<Token> InTokens;
		tokenizeInputFile(InFile, InTokens);
		return InTokens;
	}
private:
	void tokenizeInputFile(std::ifstream& InFile, std::vector<Token>& InTokens);

	void error(const std::stringstream& tmp, int line, char ch) const;
};

#endif
