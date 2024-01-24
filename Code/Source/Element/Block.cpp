#include "../../Header/element/Block.h"
#include "../../Header/Visitor/Visitor.h"

void Block::accept(Visitor* v) {
    v->visitBlock(this);
}