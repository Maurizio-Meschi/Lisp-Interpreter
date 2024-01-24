#include "../../Header/Element/BoolExpr.h"
#include "../../Header/Visitor/Visitor.h"

void RelBoolOperator::accept(Visitor* v) {
    v->visitRelBool(this);
}

void ConstBool::accept(Visitor* v) {
    v->visitConstBool(this);
}

void BoolOperator::accept(Visitor* v) {
    v->visitBool(this);
}
