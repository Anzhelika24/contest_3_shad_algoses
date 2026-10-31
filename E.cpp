#include <iostream>
#include <list>
#include <random>
namespace save {
std::mt19937 rnd(1000);
}
using save::rnd;

struct Triangle {
    int first_side;
    int second_side;
    int third_side;
};

class HashTable {
private:
    int size_;
    std::vector<std::list<int>> table_;
    bool Search(int val, std::list<int>& chain);
    void Insert(int val, std::list<int>& chain, bool rehash = false);

    long long a_, b_;
    int count_elements_ = 0;
    void Rehash();
    int Hash(int val) { return ((a_ * val) + b_) % size_; }

public:
    HashTable()
        : size_(8),
          table_(std::vector<std::list<int>>(8)),
          a_(rnd()),
          b_(rnd()) {}
    ~HashTable() {}
    bool Search(int val) { return Search(val, table_[Hash(val)]); }
    void Insert(int val) {
        Insert(val, table_[Hash(val)]);
        if (count_elements_ > size_) {
            Rehash();
        }
    }
    void Discard(int val) { Discard(val, table_[Hash(val)]); }
};

bool HashTable::Search(int val, std::list<int>& chain) {
    for (int i : chain) {
        if (i == val) {
            return true;
        }
    }
    return false;
}

void HashTable::Insert(int val, std::list<int>& chain, bool rehash) {
    if (!Search(val, chain)) {
        chain.push_front(val);
        if (!rehash) {
            count_elements_ += 1;
        }
    }
}

void HashTable::Rehash() {
    std::vector<std::list<int>> new_table(2 * size_);
    size_ *= 2;
    a_ = rnd();
    b_ = rnd();
    for (int i = 0; i < size_ / 2; ++i) {
        for (int j : table_[i]) {
            Insert(j, new_table[Hash(j)], true);
        }
    }
    table_ = new_table;
}
