#ifndef NUMEXPREMANAGER_H
#define NUMEXPREMANAGER_H

#include <vector>

#include "../Element/NumExpr.h"

class NumExpreManager {
public:
    NumExpreManager() = default;
    ~NumExpreManager() {
        clearMemory();
    }

    NumExpreManager(const NumExpreManager& other) = delete;
    NumExpreManager& operator=(const NumExpreManager& other) = delete;

    // Factory method for creating Expression objects
    NumExpr* makeNumber(int64_t value) {
        NumExpr* n = new Number(value);
        num_allocated.push_back(n);
        return n;
    }
    NumExpr* makeOperator(Operator::OpCode op, NumExpr* l, NumExpr* r) {
        NumExpr* o = new Operator(op, l, r);
        num_allocated.push_back(o);
        return o;
    }
    
    NumExpr* makeVariable(std::string var) {
        NumExpr* v = new Variable(var);
        num_allocated.push_back(v);
        return v;
    }

    void clearMemory() {
        auto i = num_allocated.begin();
        for (; i != num_allocated.end(); ++i) {
            delete(*i);
        }
        num_allocated.resize(0);
    }

private:
    std::vector<NumExpr*> num_allocated;
};

#endif
