#include <algorithm>
#include <sstream>

#include "../../Header/Visitor/Visitor.h"
#include "../../Header/Exceptions/Exceptions.h"

void EvaluationVisitor::visitOperator(Operator* opNode) {
    NumExpr* left = opNode->getLeft();
    left->accept(this);
    NumExpr* right = opNode->getRight();
    right->accept(this);
    // Taking the values from the accumulator
    int64_t rval = accumulator.back(); accumulator.pop_back();
    int64_t lval = accumulator.back(); accumulator.pop_back();
    switch (opNode->getOp()) {
    case Operator::ADD:
        accumulator.push_back(lval + rval); return;
    case Operator::SUB:
        accumulator.push_back(lval - rval); return;
    case Operator::MUL:
        accumulator.push_back(lval * rval); return;
    case Operator::DIV:
        // I handle division by 0
        if (rval == 0)
            throw EvaluationError("Division by zero");
        accumulator.push_back(lval / rval); return;
    default:
        return;
    }
}
void EvaluationVisitor::visitNumber(Number* numNode) {
    accumulator.push_back(numNode->getValue());
}
void EvaluationVisitor::visitVariable(Variable* varNode) {
    // Checking that the variable has already been defined
    if (varTable.FindVariable(varNode->getName()) == false) {
        std::stringstream tmp{};
        tmp << "Undefined variable " << varNode->getName();
        throw EvaluationError(tmp.str());
    }
    accumulator.push_back(varTable.getVariableValue(varNode->getName()));
}

void EvaluationVisitor::visitRelBool(RelBoolOperator* relBoolNode) {
    NumExpr* left = relBoolNode->getLeftOp();
    left->accept(this);
    NumExpr* right = relBoolNode->getrightOp();
    right->accept(this);
    // Taking the values from the accumulator
    int64_t rval = accumulator.back(); accumulator.pop_back();
    int64_t lval = accumulator.back(); accumulator.pop_back();
    switch (relBoolNode->getBoolOp()) {
    case RelBoolOperator::GT:
        accumulator.push_back(lval > rval); return;
    case RelBoolOperator::LT:
        accumulator.push_back(lval < rval); return;
    case RelBoolOperator::EQ:
        accumulator.push_back(lval == rval); return;
    default:
        return;
    }
}
void EvaluationVisitor::visitConstBool(ConstBool* constBoolNode) {
    accumulator.push_back(constBoolNode->getValue());
}
void EvaluationVisitor::visitBool(BoolOperator* BoolNode) {
    BoolExpr* left;
    BoolExpr* right;
    int64_t lval;
    int64_t rval;
    switch (BoolNode->getBoolOp()) {
    case BoolOperator::NOT:
        left = BoolNode->getLeftOp();
        left->accept(this);
        lval = accumulator.back(); accumulator.pop_back();
        if (lval)
            accumulator.push_back(ConstBool::FALSE);
        else
            accumulator.push_back(ConstBool::TRUE);
        return;
    case BoolOperator::AND:
        // Check the left operator first
        // If the value is false I don't check the other one
        left = BoolNode->getLeftOp();
        left->accept(this);
        lval = accumulator.back(); accumulator.pop_back();
        if (lval == ConstBool::FALSE)
            accumulator.push_back(ConstBool::FALSE);
        else {
            right = BoolNode->getrightOp();
            right->accept(this);
            rval = accumulator.back(); accumulator.pop_back();
            if (rval == ConstBool::FALSE)
                accumulator.push_back(ConstBool::FALSE);
            else
                accumulator.push_back(ConstBool::TRUE);
        }
        return;
    case BoolOperator::OR:
        // Check the left operator first. 
        // If the value is true I don't check the other one
        BoolExpr* left = BoolNode->getLeftOp();
        left->accept(this);
        lval = accumulator.back(); accumulator.pop_back();
        if (lval)
            accumulator.push_back(ConstBool::TRUE);
        else {
            BoolExpr* right = BoolNode->getrightOp();
            right->accept(this);
            rval = accumulator.back(); accumulator.pop_back();
            if (rval)
                accumulator.push_back(ConstBool::TRUE);
            else
                accumulator.push_back(ConstBool::FALSE);
        }
        return;
    }
}

void EvaluationVisitor::visitPrint(StmtPrint* printNode) {
    NumExpr* expr = printNode->getExpr();
    expr->accept(this);
    int64_t val = accumulator.back();
    std::cout << val << std::endl;
}
void EvaluationVisitor::visitSet(StmtSet* setNode) {
    Variable* var = setNode->getVar();
    NumExpr* expr = setNode->getExpr();
    expr->accept(this);
    int64_t value = accumulator.back(); accumulator.pop_back();
    varTable.push(var->getName(), value);

}
void EvaluationVisitor::visitInput(StmtInput* inputNode) {
    Variable* var = inputNode->getVar();
    std::string value{};
    std::cout << "Type an integer: ";
    std::cin >> value;
    // Checking that a valid integer was read
    // otherwise I return the error
    auto itr = value.begin();
    // Checking that if the number starts with 0, there are no other values following.
    if (*itr == '0' && value.size() > 1)
        throw EvaluationError("Invalid number");
    // I consider the value '-0' to be correct, but it must not be followed by another value
    if (*itr == '-') {
        itr++;
        if (*itr == '0' && value.size() > 2)
            throw EvaluationError("Invalid number");
    }
    // Checking if the first value is a number or a '-'
    // If it's true, the iterator is incremented, and all subsequent values are checked;
    // otherwise, the 'std::all_of' function will detect the error
    if(*itr == '-' || isdigit(*itr))
        ++itr;
    if (!std::all_of(itr, value.end(), ::isdigit)) {
        throw EvaluationError("No integer was entered");
    }
    int64_t val = std::stoll(value);
    varTable.push(var->getName(), val);
}
void EvaluationVisitor::visitWhile(StmtWhile* whileNode) {
    BoolExpr* Bexpr = whileNode->getBoolExpr();
    Bexpr->accept(this);
    int64_t val = accumulator.back(); accumulator.pop_back();
    Block* block = whileNode->getStmtBlock();
    while (val != ConstBool::FALSE) {
        block->accept(this);
        Bexpr = whileNode->getBoolExpr();
        Bexpr->accept(this);
        val = accumulator.back();
        accumulator.pop_back();
    }
}
void EvaluationVisitor::visitIf(StmtIf* ifNode) {
    BoolExpr* Bexpr = ifNode->getExpr();
    Bexpr->accept(this);
    int64_t val = accumulator.back(); accumulator.pop_back();
    if (val) {
        Block* block1 = ifNode->getLeft();
        block1->accept(this);
    }
    else {
        Block* block2 = ifNode->getRight();
        block2->accept(this);
    }
}

void EvaluationVisitor::visitBlock(Block* blockNode) {
    auto itr = blockNode->getBegin();
    while (itr != blockNode->getEnd()) {
        Statement* stmt = blockNode->getStmt(itr);
        stmt->accept(this);
        ++itr;
    }
}
void EvaluationVisitor::visitProgram(Program* varNode) {
    Block* block = varNode->getBlock();
    block->accept(this);
}