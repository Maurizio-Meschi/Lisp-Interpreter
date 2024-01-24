#ifndef PROGRAM_H
#define PROGRAM_H

#include "Block.h"

class Program {
public:
	Program(Block* b) : block{ b } { }
	~Program() = default;

	Program(const Program& other) = default;
	Program& operator=(const Program& other) = default;

	Block* getBlock() const {
		return block;
	}

	void accept(Visitor* v);
private:
	Block* block;
};

#endif
