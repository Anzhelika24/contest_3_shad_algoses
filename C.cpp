#include <iostream>
#include <list>

struct Block {
    int start;
    int size;
    bool is_free;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int memory_size;
    std::cin >> memory_size;
    std::list<Block> blocks;
    blocks.push_back(Block{
        1,
        memory_size,
        true,
    });
    return 0;
}