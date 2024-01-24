#ifndef STMTMANAGER_H
#define STMTMANAGER_H

#include <vector>
#include "../Element/Statement.h"
#include "../Element/Block.h"


class StmtManager {
public:
	StmtManager() = default;
	~StmtManager() {
		clearMemory();
	}

	StmtManager(const StmtManager& other) = delete;
	StmtManager& operator=(const StmtManager& other) = delete;

	// Factory method for creating Statement objects
	Statement* makeStmtPrint(NumExpr* expr) {
		Statement* p = new StmtPrint(expr);
		allocated.push_back(p);
		return p;
	}
	Statement* makeStmtSet(Variable* var, NumExpr* expr) {
		Statement* s = new StmtSet(var, expr);
		allocated.push_back(s);
		return s;
	}
	Statement* makeStmtInput(Variable* var) {
		Statement* i = new StmtInput(var);
		allocated.push_back(i);
		return i;
	}
	Statement* makeStmtWhile(BoolExpr* Bexpr, Block* b) {
		Statement* w = new StmtWhile(Bexpr, b);
		allocated.push_back(w);
		return w;
	}
	Statement* makeStmtIf(Block* l, Block* r, BoolExpr* Bexpr) {
		Statement* IF = new StmtIf(l, r, Bexpr);
		allocated.push_back(IF);
		return IF;
	}

	void clearMemory() {
		auto i = allocated.begin();
		for (; i != allocated.end(); ++i) {
			delete(*i);
		}
		allocated.resize(0);
	}
private:
	std::vector<Statement*> allocated;
};

#endif