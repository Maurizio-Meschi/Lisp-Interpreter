#ifndef BOLLEXPRMANAGER_H
#define BOLLEXPRMANAGER_H

#include <vector>

#include "../Element/BoolExpr.h"
#include "../Element/NumExpr.h"

class BoolExpreManager {
public:
    BoolExpreManager() = default;
    ~BoolExpreManager() {
        clearMemory();
    }

    BoolExpreManager(const BoolExpreManager& other) = delete;
    BoolExpreManager& operator=(const BoolExpreManager& other) = delete;

    // Factory method for creating Bool Expression objects
    BoolExpr* makeRelBoolOp(RelBoolOperator::RelOpCode op, NumExpr* lop, NumExpr* rop) {
       BoolExpr* r = new RelBoolOperator(op, lop, rop);
       allocated.push_back(r);
       return r;
    }
    BoolExpr* makeConstBoolOp(int value) {
        BoolExpr* c = new ConstBool(value);
        allocated.push_back(c);
        return c;
    }

    BoolExpr* makeBoolOp(BoolOperator::OpCodeBool op, BoolExpr* expr_bool1, BoolExpr* expr_bool2) {
        BoolExpr* b = new BoolOperator(op, expr_bool1, expr_bool2);
        allocated.push_back(b);
        return b;
    }

    void clearMemory() {
        auto i = allocated.begin();
        for (; i != allocated.end(); ++i) {
            delete(*i);
        }
        allocated.resize(0);
    }

private:
    std::vector<BoolExpr*> allocated;
};

#endif