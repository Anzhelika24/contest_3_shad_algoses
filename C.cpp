#include <iostream>
#include <list>
#include <utility>
#include <vector>

struct MemoryBlock {
    int start;
    int size;
    bool is_free;
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
    std::cout << "start " << '\n';
    int memory_size;
    int number_requests;
    std::cin >> memory_size >> number_requests;
    std::list<MemoryBlock> blocks;
    blocks.push_back(MemoryBlock{
        1,
        memory_size,
        true,
    });

    Heap heap;

    heap.Insert({10, 5, 0});
    heap.Insert({2, 8, 1});
    heap.Insert({20, 8, 2});
    std::cout << "start " << '\n';

    while (!heap.Empty()) {
        auto extractedmax = heap.ExtractMax();
        std::cout << extractedmax.start << " " << extractedmax.size << "\n";
    }
    return 0;
}