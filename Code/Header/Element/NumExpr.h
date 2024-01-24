#ifndef NUMEXPR_H
#define	NUMEXPR_H

#include <string>

class Visitor;

class NumExpr {
public:
	virtual ~NumExpr() {};
	virtual void accept(Visitor* v) = 0;
};

// Concrete class for numbers
class Number : public NumExpr {
public:
	Number(int64_t v) : value{ v } { }
	~Number() = default;

	Number(const Number& other) = default;
	Number& operator=(const Number& other) = default;

	int64_t getValue() const{
		return value;
	}

	void accept(Visitor* v) override;
private:
	int64_t value;
};

// Concrete class for variables
class Variable : public NumExpr {
public:
    Variable(std::string n) : name{ n } { }
	~Variable() = default;
	
	Variable(const Variable& other) = default;
	Variable& operator=(const Variable& other) = default;

   std::string getName() const {
        return name;
   }

   void accept(Visitor* v) override;
private:
    std::string name;
};

// Concrete class for operators
class Operator : public NumExpr {
public:
    enum OpCode { ADD, SUB, MUL, DIV };

    Operator(OpCode o, NumExpr* lop, NumExpr* rop) :
        op{ o }, left{ lop }, right{ rop } { }
    ~Operator() = default;
    Operator(const Operator& other) = default;
    Operator& operator=(const Operator& other) = default;

    OpCode getOp() const {
        return op;
    }
    NumExpr* getLeft() const {
        return left;
    }
    NumExpr* getRight() const {
        return right;
    }

    void accept(Visitor* v) override;

    static std::string opCodeToString(OpCode op) {
        switch (op) {
        case ADD: return "ADD";
        case SUB: return "SUB";
        case MUL: return "MUL";
        case DIV: return "DIV";
        }
        return NULL;
    }

private:
    OpCode op;
    NumExpr* left;
    NumExpr* right;

};

#endif
