#ifndef BOOLEXPR_H
#define BOOLEXPR_H

#include <string>
#include "NumExpr.h"

class NumExpr;

class BoolExpr {
public:
	virtual ~BoolExpr() {};
	virtual void accept(Visitor* v) = 0;
};

// Concrete class for relational boolean operators
class RelBoolOperator : public BoolExpr {
public:
	enum RelOpCode { GT, LT, EQ };

	RelBoolOperator(RelOpCode op, NumExpr* l, NumExpr* r) :
		Rel_BoolOp{ op }, Rel_left{ l }, Rel_right{ r } { }
	~RelBoolOperator() = default;

	RelBoolOperator(const RelBoolOperator& other) = default;
	RelBoolOperator& operator=(const RelBoolOperator& other) = default;

	RelOpCode getBoolOp() const {
		return Rel_BoolOp;
	}
	NumExpr* getLeftOp() const {
		return Rel_left;
	}
	NumExpr* getrightOp() const {
		return Rel_right;
	}

	void accept(Visitor* v) override;

	static std::string RelopCodeToString(RelOpCode op) {
		switch (op) {
		case GT: return "GT";
		case LT: return "LT";
		case EQ: return "EQ";
		}
		return NULL;
	}
	
private:
	RelOpCode Rel_BoolOp;
	NumExpr* Rel_left;
	NumExpr* Rel_right;
};


// Concrete class for constant boolean operators
class ConstBool : public BoolExpr {
public:
	static constexpr int FALSE = 0;
	static constexpr int TRUE = 1;

	ConstBool(int v) : value{ v } { }
	~ConstBool() = default;

	ConstBool(const ConstBool& other) = default;
	ConstBool& operator=(const ConstBool& other) = default;

	int getValue() const {
		return value;
	}

	void accept(Visitor* v) override;
private:
	int value;
};

// Concrete class for boolean operators
class BoolOperator : public BoolExpr {
public:
	enum OpCodeBool { AND, OR, NOT };

	BoolOperator(OpCodeBool op, BoolExpr* l, BoolExpr* r) :
		BoolOp{ op }, left{ l }, right{ r } { }
	~BoolOperator() = default;

	BoolOperator(const BoolOperator& other) = default;
	BoolOperator& operator=(const BoolOperator& other) = default;

	OpCodeBool getBoolOp() const {
		return BoolOp;
	}
	BoolExpr* getLeftOp() const {
		return left;
	}
	BoolExpr* getrightOp() const {
		return right;
	}

	void accept(Visitor* v) override;

	static std::string opCodeToString(OpCodeBool op) {
		switch (op) {
		case AND: return "AND";
		case OR:  return "OR";
		case NOT: return "NOT";
		}
		return NULL;
	}

private:
	OpCodeBool BoolOp;
	BoolExpr* left;
	BoolExpr* right;
};

#endif
