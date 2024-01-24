#include <sstream>
#include <string>
#include <iostream>
#include <algorithm>

#include "../../Header/Token/Tokenizer.h"
#include "../../Header/Exceptions/Exceptions.h"

// A function to send an error message
void Tokenizer::error(const std::stringstream& tmp, int line, char ch) const {
	std::stringstream error_tmp;

	error_tmp << "line "
		     << line << ": '"
		     << tmp.str() << ch
		     << "'  not recognized";

	throw LexicalError(error_tmp.str());
}

void Tokenizer::tokenizeInputFile(std::ifstream& InFile,
	std::vector<Token>& InTokens) {

	auto begin = Token::idWord;
	auto end = begin + Token::KEYWORD;

	char read;
	int line = 1;
	read = InFile.get();

	while (!InFile.eof()) {
		std::stringstream tmp{};

		// Spaces are ignored
		if (std::isspace(read)) {
			read = InFile.get();
			continue;
		}

		/*
		If a character other than:
        - a number (positive or negative)
        - an alphabetic character
        - parentheses
        is read, then there is definitely an error, as it does not belong to the grammar
		*/
		if (!isdigit(read) && !isalpha(read) &&
			read != '(' && read != ')' && read != '-') {
			error(tmp, line, read);
		}

		while (std::isalpha(read)) {
			tmp << read;
			read = InFile.get();
			// if an character string has been consumed and the next character is a number, 
			// there is an error because variables or keywords do not contain numbers
			if (std::isdigit(read)) {
				error(tmp, line, read);
			}
		}

		// Checking that the string tmp is not empty.
		if (tmp.rdbuf()->in_avail() != 0) {
			// Checking that a keyword was read
			auto itr = std::find(begin, end, tmp.str());
			int id = std::distance(begin, itr);
			// If the result is positive, I create the associated token
			if (itr != end)
				InTokens.push_back(Token{ id, *itr });
			// Otherwise, I create a variable
			else
				InTokens.push_back(Token{ Token::VAR, tmp.str() });
		}

		// Creating the LP/RP tokens.
		if (read == '(') {
			InTokens.push_back(Token{ Token::LP, Token::idWord[Token::LP] });
			read = InFile.get();
		}
		else if (read == ')') {
			InTokens.push_back(Token{ Token::RP, Token::idWord[Token::RP] });
			read = InFile.get();
		}

		if (std::isdigit(read) || read == '-') {
			// If a minus sign is read, I must ensure that it is followed by a number
			if (read == '-') {
				tmp << read;
				read = InFile.get();
				if (!std::isdigit(read)) {
					error(tmp, line, read);
				}
			}
			// If a zero is read, I must ensure that I don't read any other numbers
			if (read == '0') {
				tmp << read;
				read = InFile.get();
				if (std::isdigit(read)) {
					error(tmp, line, read);
				}
			}
			// Reading the entire numeric string
			while (std::isdigit(read)) {
				tmp << read;
				read = InFile.get();
			}
			// After a number, I cannot have a character
			// I handle this error
			if (std::isalpha(read)) {
				error(tmp, line, read);
			}
			InTokens.push_back(Token{ Token::NUM, tmp.str() });
		}
		// Keep the line in memory for information about the position of a potential error.
		if (read == '\n')
			++line;
	}
}