#include <iostream>
#include <numeric>
#include <random>
#include <vector>

namespace save {
std::mt19937 rnd(1000);
}
using save::rnd;

struct Triangle {
    int first_side;
    int second_side;
    int third_side;
    Triangle& operator=(Triangle other) {
        std::swap(*this, other);
        return *this;
    }
};

class HashTable {
private:
    int size_;
    std::vector<std::vector<Triangle>> table_;
    bool Search(const Triangle& val, const std::vector<Triangle>& chain) const;
    void Insert(const Triangle& val, std::vector<Triangle>& chain,
                bool rehash = false);

    long long a_, b_;
    int count_elements_ = 0;
    void Rehash();
    int Hash(Triangle val) { return ((a_ * val) + b_) % size_; }

public:
    HashTable()
        : size_(8),
          table_(std::vector<std::vector<Triangle>>(8)),
          a_(rnd()),
          b_(rnd()) {}
    ~HashTable() {}
    bool Search(const Triangle& val) const {
        return Search(val, table_[Hash(val)]);
    }
    void Insert(Triangle val) {
        Insert(val, table_[Hash(val)]);
        if (count_elements_ > size_) {
            Rehash();
        }
    }
};

bool HashTable::Search(const Triangle& val,
                       const std::vector<Triangle>& chain) const {
    for (const Triangle& element : chain) {
        if (element == val) {
            return true;
        }
    }
    return false;
}

void HashTable::Insert(const Triangle& val, std::vector<Triangle>& chain,
                       bool rehash) {
    if (!Search(val, chain)) {
        chain.push_back(val);
        if (!rehash) {
            count_elements_ += 1;
        }
    }
}

void HashTable::Rehash() {
    std::vector<std::vector<Triangle>> new_table(2 * size_);
    size_ *= 2;
    a_ = rnd();
    b_ = rnd();
    for (int i = 0; i < size_ / 2; ++i) {
        for (Triangle j : table_[i]) {
            Insert(j, new_table[Hash(j)]);
        }
    }
    std::swap(table_, new_table);
}

int GCD(const Triangle& triangle) {
    return std::gcd(triangle.first_side,
                    std::gcd(triangle.second_side, triangle.third_side));
}
void CanonizeTriangle(Triangle triangle) {}
