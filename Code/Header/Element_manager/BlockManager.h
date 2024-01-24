#ifndef BLOCKMANAGER_H
#define BLOCKMANAGER_H

#include <vector>
#include "../Element/Block.h"

class BlockManager {
public:
    BlockManager() = default;
    ~BlockManager() {
        clearMemory();
    }

    BlockManager(const BlockManager& other) = delete;
    BlockManager& operator=(const BlockManager& other) = delete;

    // Factory method for creating block objects
    Block* makeBlock(std::vector<Statement*> list) {
        Block* b = new Block(list);
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
	std::vector<Block*> allocated;
};

#endif
