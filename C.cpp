#include <iostream>
#include <list>

struct MemoryBlock {
    int start;
    int size;
    bool is_free;
};

struct HeapElement {
    int start;
    int size;
    int id;
    static bool IsBetter(const HeapElement& first, const HeapElement& second);
};

class Heap {
private:
    std::vector<HeapElement> heap_;
    void SiftUp(int index);
    void SiftDown(int index);
    static bool IsBetter(const HeapElement& first, const HeapElement& second);

public:
    void Insert(const HeapElement& element);
    HeapElement ExtractMin();
    bool Empty() const { return heap_.empty(); }
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
    int smallest_child;
    while (true) {
        int left_child = (2 * index) + 1;
        if (left_child >= static_cast<int>(heap_.size())) {
            break;
        }

        smallest_child = left_child;
        int right_child = (2 * index) + 2;
        if (right_child < static_cast<int>(heap_.size()) &&
            IsBetter(heap_[right_child], heap_[left_child])) {
            smallest_child = right_child;
        }

        if (heap_[index].value <= heap_[smallest_child].value) {
            break;
        }
        std::swap(heap_[index], heap_[smallest_child]);
        index = smallest_child;
    }
}

void Heap::Insert(const HeapElement& element) {
    heap_.push_back(element);
    SiftUp(static_cast<int>(heap_.size()) - 1);
}

HeapElement Heap::ExtractMin() {
    HeapElement extracted_min = heap_[0];
    std::swap(heap_[0], heap_[heap_.size() - 1]);
    heap_.pop_back();
    SiftDown(0);
    return extracted_min;
}

bool HeapElement::IsBetter(const HeapElement& first,
                           const HeapElement& second) {
    if (first.size != second.size) {
        return first.size > second.size;
    }
    return first.start > second.start;
}

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
    });
    return 0;
}