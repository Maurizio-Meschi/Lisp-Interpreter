#ifndef VISITOR_H
#define VISITOR_H

#include <iostream>
#include <string>
#include <vector>

#include "../Element/NumExpr.h"
#include "../Element/BoolExpr.h"
#include "../Element/Statement.h"
#include "../Element/Block.h"
#include "../Element/Program.h"
#include "../Variable/VariableTable.h"

class Visitor {
public:
    virtual void visitOperator(Operator* opNode) = 0;
    virtual void visitNumber(Number* numNode) = 0;
    virtual void visitVariable(Variable* varNode) = 0;
    
    virtual void visitRelBool(RelBoolOperator* relBoolNode) = 0;
    virtual void visitConstBool(ConstBool* constBoolNode) = 0;
    virtual void visitBool(BoolOperator* BoolNode) = 0;

    virtual void visitPrint(StmtPrint* printNode) = 0;
    virtual void visitSet(StmtSet* setNode) = 0;
    virtual void visitInput(StmtInput* inputNode) = 0;
    virtual void visitWhile(StmtWhile* whileNode) = 0;
    virtual void visitIf(StmtIf* ifNode) = 0;
    
    virtual void visitBlock(Block* blockNode) = 0;
    virtual void visitProgram(Program* varNode) = 0;
};

// Concrete class for instruction evaluation
class EvaluationVisitor : public Visitor {
public:
    EvaluationVisitor(VariableTable& table) : accumulator{ }, varTable{ table } { }
    ~EvaluationVisitor() = default;

    EvaluationVisitor(const EvaluationVisitor& other) = default;
    EvaluationVisitor& operator=(const EvaluationVisitor& other) = default;

    void visitOperator(Operator* opNode) override;
    void visitNumber(Number* numNode) override;
    void visitVariable(Variable* varNode) override;

    void visitRelBool(RelBoolOperator* relBoolNode) override;
    void visitConstBool(ConstBool* constBoolNode) override;
    void visitBool(BoolOperator* BoolNode) override;

    void visitPrint(StmtPrint* printNode) override;
    void visitSet(StmtSet* setNode) override;
    void visitInput(StmtInput* inputNode) override;
    void visitWhile(StmtWhile* whileNode) override;
    void visitIf(StmtIf* ifNode) override;

    void visitBlock(Block* blockNode) override;
    void visitProgram(Program* varNode) override;
private:
    std::vector<int64_t> accumulator;
    VariableTable& varTable;
};

// Concrete class to print instructions
// NOTE: the class was used to carry out tests
// to verify the correct functioning of the parse
class PrintVisitor : public Visitor {
public:
    void visitOperator(Operator* opNode) override {
        std::cout << "(";
        std::cout << opNode->opCodeToString(opNode->getOp());
        std::cout << " ";
        opNode->getLeft()->accept(this);
        std::cout << " ";
        opNode->getRight()->accept(this);
        std::cout << ")" << std::endl;
    }
    void visitNumber(Number* numNode) override {
        std::cout << numNode->getValue();
    }
    void visitVariable(Variable* varNode) override {
        std::cout << varNode->getName();
    }

    void visitRelBool(RelBoolOperator* relBoolNode) override {
        std::cout << "(";
        std::cout << relBoolNode->RelopCodeToString(relBoolNode->getBoolOp());
        std::cout << " ";
        relBoolNode->getLeftOp()->accept(this);
        std::cout << " ";
        relBoolNode->getrightOp()->accept(this);
        std::cout << ")" << std::endl;
    }

    void visitConstBool(ConstBool* constBoolNode) override {
        int value = constBoolNode->getValue();
        if (value == ConstBool::TRUE)
            std::cout << "TRUE ";
        else
            std::cout << "FALSE ";
    }

    void visitBool(BoolOperator* BoolNode) override {
        std::cout << "(";
        std::cout << BoolNode->opCodeToString(BoolNode->getBoolOp());
        std::cout << " ";
        if (BoolNode->getBoolOp() == BoolOperator::NOT) {
            BoolNode->getLeftOp()->accept(this);
            std::cout << ")" << std::endl;
        }
        else {
            BoolNode->getLeftOp()->accept(this);
            BoolNode->getrightOp()->accept(this);
            std::cout << ")" << std::endl;
        }
    }

    void visitPrint(StmtPrint* printNode) override {
        std::cout << "(PRINT ";
        printNode->getExpr()->accept(this);
        std::cout << ")" << std::endl;
    }

    void visitSet(StmtSet* setNode) override {
        std::cout << "(SET ";
        setNode->getVar()->accept(this);
        std::cout << " ";
        setNode->getExpr()->accept(this);
        std::cout << ")" << std::endl;
    }

    void visitInput(StmtInput* inputNode) override {
        std::cout << "(INPUT ";
        inputNode->getVar()->accept(this);
        std::cout << ")" << std::endl;
    }

    void visitWhile(StmtWhile* whileNode) override {
        std::cout << "(WHILE ";
        whileNode->getBoolExpr()->accept(this);
        whileNode->getStmtBlock()->accept(this);
        std::cout << ")" << std::endl;
    }

    void visitIf(StmtIf* ifNode) override {
        std::cout << "(IF ";
        ifNode->getExpr()->accept(this);
        ifNode->getLeft()->accept(this);
        ifNode->getRight()->accept(this);
        std::cout << ")" << std::endl;
    }

    void visitBlock(Block* blockNode) override {
        if (blockNode->getSize() > 1) {
            std::cout << "(BLOCK" << std::endl;
            auto itr = blockNode->getBegin();
            while (itr != blockNode->getEnd()) {
                blockNode->getStmt(itr)->accept(this);
                ++itr;
            }
            std::cout << ")" << std::endl;
        }
        else
            blockNode->getStmt(blockNode->getBegin())->accept(this);
    }

    void visitProgram(Program* programNode) override {
        programNode->getBlock()->accept(this);
    }
};

#endif
