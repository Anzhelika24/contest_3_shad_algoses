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

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int memory_size;
    int number_requests;
    std::cin >> memory_size >> number_requests;

    std::list<MemoryBlock> blocks;
    blocks.push_back(MemoryBlock{
        1,
        memory_size,
        true,
        0,
    });

    Heap heap;
    heap.Insert({1, memory_size, 0});

    std::vector<bool> is_actual(1, true);

    std::vector<std::list<MemoryBlock>::iterator> block_iterator_by_id;
    block_iterator_by_id.push_back(blocks.begin());

    std::vector<std::list<MemoryBlock>::iterator> allocation_by_request(
        number_requests, blocks.end());

    int new_id = static_cast<int>(is_actual.size());

    int command;
    for (int i = 0; i < number_requests; ++i) {
        std::cin >> command;
        if (command > 0) {
            while (!heap.Empty() && !is_actual[heap.Top().id]) {
                heap.ExtractMax();
            }

            if (heap.Empty() || heap.Top().size < command) {
                std::cout << -1 << '\n';
                continue;
            }

            auto selected_block = heap.ExtractMax();
            auto block_it = block_iterator_by_id[selected_block.id];
            if (block_it->size == command) {
                block_it->is_free = false;
                is_actual[block_it->id] = false;
                allocation_by_request[i] = block_it;
            } else {
                block_it->size -= command;
                block_it->start += command;
                auto new_it =
                    blocks.insert(block_it, MemoryBlock{
                                                block_it->start - command,
                                                command,
                                                false,
                                                -1,
                                            });
            }
        }
    }

    return 0;
}