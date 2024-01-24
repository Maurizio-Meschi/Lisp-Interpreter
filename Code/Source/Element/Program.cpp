#include "../../Header/Visitor/Visitor.h"

void Program::accept(Visitor* v) {
    v->visitProgram(this);
}