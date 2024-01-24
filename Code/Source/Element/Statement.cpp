#include "../../Header/Element/Statement.h"
#include "../../Header/Visitor/Visitor.h"

void StmtPrint::accept(Visitor* v) {
	v->visitPrint(this);
}

void StmtSet::accept(Visitor* v) {
	v->visitSet(this);
}

void StmtInput::accept(Visitor* v) {
	v->visitInput(this);
}

void StmtWhile::accept(Visitor* v) {
	v->visitWhile(this);
}

void StmtIf::accept(Visitor* v) {
	v->visitIf(this);
}