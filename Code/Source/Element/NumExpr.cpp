#include "../../Header/Element/NumExpr.h"
#include "../../Header/Visitor/Visitor.h"

void Number::accept(Visitor* v) {
    v->visitNumber(this);
}

void Operator::accept(Visitor* v) {
    v->visitOperator(this);
}

void Variable::accept(Visitor* v) {
    v->visitVariable(this);
}
