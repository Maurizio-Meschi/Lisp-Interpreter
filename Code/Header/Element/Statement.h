#ifndef STATEMENT_H
#define STATEMENT_H

#include "NumExpr.h"
#include "BoolExpr.h"

class Block;
class Virtual;

class Statement {
public:
	virtual ~Statement() {};
	virtual void accept(Visitor* v) = 0;
};

// Concrete class for the print statement
class StmtPrint : public Statement {
public:
	StmtPrint(NumExpr* e) : expr{ e } { }
	~StmtPrint() = default;

	StmtPrint(const StmtPrint& other) = default;
	StmtPrint& operator=(const StmtPrint& other) = default;

	NumExpr* getExpr() const{
		return expr;
	}

	void accept(Visitor* v) override;
private:
	NumExpr* expr;
};

// Concrete class for the set statement
class StmtSet : public Statement {
public:
	StmtSet(Variable* v, NumExpr* e) : var{ v }, expr{ e } { }
	~StmtSet() = default;

	StmtSet(const StmtSet& other) = default;
	StmtSet& operator=(const StmtSet& other) = default;

	Variable* getVar() const {
		return var;
	}
	NumExpr* getExpr() const {
		return expr;
	}
	void accept(Visitor* v) override;
private:
	Variable* var;
	NumExpr* expr;
};

// Concrete class for the input statement
class StmtInput : public Statement {
public:
	StmtInput(Variable* v) : var{ v } { }
	~StmtInput() = default;

	StmtInput(const StmtInput& other) = default;
	StmtInput& operator=(const StmtInput& other) = default;

	Variable* getVar() const {
		return var;
	}
	void accept(Visitor* v) override;
private:
	Variable* var;
};

// Concrete class for the while statement
class StmtWhile : public Statement {
public:
	StmtWhile(BoolExpr* b, Block* block) : bool_expr{ b }, stmt_block{ block } { }
	~StmtWhile() = default;

	StmtWhile(const StmtWhile& other) = default;
	StmtWhile& operator=(const StmtWhile& other) = default;

	BoolExpr* getBoolExpr() const {
		return bool_expr;
	}
	Block* getStmtBlock() const {
		return stmt_block;
	}

	void accept(Visitor* v) override;
private:
	BoolExpr* bool_expr;
	Block* stmt_block;
};

// Concrete class for the if statement
class StmtIf : public Statement {
public:
	StmtIf(Block* l, Block* r, BoolExpr* e) : left{ l }, right{ r }, expr{ e } { }
	~StmtIf() = default;

	StmtIf(const StmtIf& other) = default;
	StmtIf& operator=(const StmtIf& other) = default;

	Block* getRight() const {
		return right;
	}
	Block* getLeft() const{
		return left;
	}
	BoolExpr* getExpr() const {
		return expr;
	}

	void accept(Visitor* v) override;
private:
	Block* left;
	Block* right;
	BoolExpr* expr;
};
#endif
