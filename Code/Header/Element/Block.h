#ifndef BLOCK_H
#define BLOCK_H

#include <vector>
#include "Statement.h"

class Visitor;

class Block{
public:
	Block(std::vector<Statement*> list) : stmt_list{ list } {}
	~Block() = default;

	Block(const Block& other) = default;
	Block& operator=(const Block& other) = default;

	std::vector<Statement*>::const_iterator getBegin() const {
		return stmt_list.begin();
	}
	std::vector<Statement*>::const_iterator getEnd() const {
		return stmt_list.end();
	}

	Statement* getStmt(std::vector<Statement*>::const_iterator itr) const {
		return *itr;
	}
	size_t getSize() const {
		return stmt_list.size();
	}

	void accept(Visitor* v);
private:
	std::vector<Statement*> stmt_list;
};

#endif
