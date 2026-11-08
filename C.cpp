#include <iostream>
#include <list>
#include <utility>
#include <vector>

struct MemoryBlock {
    int start;
    int size;
    bool is_free;
    int id;
};

struct HeapElement {
    int start;
    int size;
    int id;
};

class Heap {
private:
    std::vector<HeapElement> heap_;
    void SiftUp(int index);
    void SiftDown(int index);
    static bool IsBetter(const HeapElement& first, const HeapElement& second);

public:
    void Insert(const HeapElement& element);
    HeapElement ExtractMax();
    bool Empty() const { return heap_.empty(); }
    const HeapElement& Top() const;
};

void Heap::SiftUp(int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (!IsBetter(heap_[index], heap_[parent])) {
            break;
        }
        std::swap(heap_[index], heap_[parent]);
        index = parent;
    }
}

void Heap::SiftDown(int index) {
    int best_child;
    while (true) {
        int left_child = (2 * index) + 1;
        if (left_child >= static_cast<int>(heap_.size())) {
            break;
        }

        best_child = left_child;
        int right_child = (2 * index) + 2;
        if (right_child < static_cast<int>(heap_.size()) &&
            IsBetter(heap_[right_child], heap_[left_child])) {
            best_child = right_child;
        }

        if (!IsBetter(heap_[best_child], heap_[index])) {
            break;
        }
        std::swap(heap_[index], heap_[best_child]);
        index = best_child;
    }
}

void Heap::Insert(const HeapElement& element) {
    heap_.push_back(element);
    SiftUp(static_cast<int>(heap_.size()) - 1);
}

HeapElement Heap::ExtractMax() {
    HeapElement extracted_max = heap_[0];
    std::swap(heap_[0], heap_[heap_.size() - 1]);
    heap_.pop_back();
    SiftDown(0);
    return extracted_max;
}

bool Heap::IsBetter(const HeapElement& first, const HeapElement& second) {
    if (first.size != second.size) {
        return first.size > second.size;
    }
    return first.start < second.start;
}

const HeapElement& Heap::Top() const { return heap_[0]; }

class MemoryManager {
public:
    MemoryManager(int memory_size, int number_requests);

    int Allocate(int size, int request_index);
    void Release(int request_index);

private:
    using BlockIterator = std::list<MemoryBlock>::iterator;

    std::list<MemoryBlock> blocks_;
    Heap heap_;
    std::vector<bool> is_actual_;
    std::vector<BlockIterator> block_iterator_by_id_;
    std::vector<BlockIterator> allocation_by_request_;

    void RemoveInactiveTop();
    void RegisterFreeBlock(BlockIterator block_it);
};

MemoryManager::MemoryManager(int memory_size, int number_requests)
    : allocation_by_request_(number_requests, blocks_.end()) {
    blocks_.push_back({1, memory_size, true, 0});
    heap_.Insert({1, memory_size, 0});

    is_actual_.push_back(true);
    block_iterator_by_id_.push_back(blocks_.begin());
}

void MemoryManager::RemoveInactiveTop() {
    while (!heap_.Empty() && !is_actual_[heap_.Top().id]) {
        heap_.ExtractMax();
    }
}

void MemoryManager::RegisterFreeBlock(BlockIterator block_it) {
    int new_id = static_cast<int>(is_actual_.size());

    block_it->id = new_id;
    is_actual_.push_back(true);
    block_iterator_by_id_.push_back(block_it);

    heap_.Insert({block_it->start, block_it->size, new_id});
}

int MemoryManager::Allocate(int size, int request_index) {
    RemoveInactiveTop();

    if (heap_.Empty() || heap_.Top().size < size) {
        return -1;
    }

    HeapElement selected_block = heap_.ExtractMax();
    BlockIterator block_it = block_iterator_by_id_[selected_block.id];
    is_actual_[selected_block.id] = false;

    if (block_it->size == size) {
        block_it->is_free = false;
        allocation_by_request_[request_index] = block_it;
    } else {
        BlockIterator occupied_it =
            blocks_.insert(block_it, {block_it->start, size, false, -1});

        block_it->size -= size;
        block_it->start += size;

        allocation_by_request_[request_index] = occupied_it;
        RegisterFreeBlock(block_it);
    }

    return selected_block.start;
}

void MemoryManager::Release(int request_index) {
    BlockIterator block_it = allocation_by_request_[request_index];

    if (block_it == blocks_.end()) {
        return;
    }
    if (block_it != blocks_.begin()) {
        BlockIterator left_it = std::prev(block_it);

        if (left_it->is_free) {
            is_actual_[left_it->id] = false;

            left_it->size += block_it->size;
            blocks_.erase(block_it);

            block_it = left_it;
        }

        if (block_it != std::prev(blocks_.end())) {
            BlockIterator right_it = std::next(block_it);

            if (right_it->is_free) {
                is_actual_[right_it->id] = false;

                block_it->size += right_it->size;
                blocks_.erase(right_it);
            }
        }
        block_it->is_free = true;
    }

    int main() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr);

        int memory_size;
        int number_requests;
        std::cin >> memory_size >> number_requests;

        MemoryManager manager(memory_size, number_requests);

        for (int i = 0; i < number_requests; ++i) {
            int command;
            std::cin >> command;

            if (command > 0) {
                std::cout << manager.Allocate(command, i) << '\n';
            } else {
                manager.Release(-command - 1);
            }
        }

        return 0;
    }