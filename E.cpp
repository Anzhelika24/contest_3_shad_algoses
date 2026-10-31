#include <cstddef>
#include <iostream>
#include <numeric>
#include <random>
#include <utility>
#include <vector>

constexpr size_t kStartSize = 8;
namespace Save {
constexpr int64_t kPrime = 2147483647;

std::mt19937 rnd(std::random_device{}());

std::uniform_int_distribution<int64_t> distribution(0, kPrime - 1);
}  // namespace Save

using Save::distribution;
using Save::rnd;

struct Triangle {
    int first_side;
    int second_side;
    int third_side;

    bool operator==(const Triangle& other) const {
        return (first_side == other.first_side) &&
               (second_side == other.second_side) &&
               (third_side == other.third_side);
    }
    Triangle& operator/=(int divisor) {
        first_side /= divisor;
        second_side /= divisor;
        third_side /= divisor;
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

    int64_t a_, b_, c_, d_;
    int count_elements_ = 0;
    void Rehash();
    int Hash(const Triangle& triangle) const;

public:
    HashTable()
        : size_(kStartSize),
          table_(kStartSize),
          a_(distribution(rnd)),
          b_(distribution(rnd)),
          c_(distribution(rnd)),
          d_(distribution(rnd)) {}
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

int HashTable::Hash(const Triangle& triangle) const {
    int64_t hash_code =
        ((a_ * triangle.first_side) + (b_ * triangle.second_side) +
         (c_ * triangle.third_side) + d_) %
        Save::kPrime;

    return static_cast<int>(hash_code % size_);
}

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
        for (const Triangle& canon_triangle : table_[i]) {
            Insert(canon_triangle, new_table[Hash(canon_triangle)], true);
        }
    }
    std::swap(table_, new_table);
}

int GCD(const Triangle& triangle) {
    return std::gcd(triangle.first_side,
                    std::gcd(triangle.second_side, triangle.third_side));
}

void CanonizeTriangle(Triangle* triangle) {
    if (triangle->first_side > triangle->second_side) {
        std::swap(triangle->first_side, triangle->second_side);
    }
    if (triangle->second_side > triangle->third_side) {
        std::swap(triangle->second_side, triangle->third_side);
    }
    if (triangle->first_side > triangle->second_side) {
        std::swap(triangle->first_side, triangle->second_side);
    }

    *triangle /= GCD(*triangle);
}

int main() { return 0; }
