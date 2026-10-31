#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <utility>
#include <vector>

constexpr int kStartSize = 8;
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
    void Canonize();
};

class HashTable {
private:
    int size_;
    std::vector<std::vector<Triangle>> table_;
    static bool Search(const Triangle& val, const std::vector<Triangle>& chain);
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
    bool Search(const Triangle& val) { return Search(val, table_[Hash(val)]); }
    void Insert(Triangle val) {
        Insert(val, table_[Hash(val)]);
        if (count_elements_ > size_) {
            Rehash();
        }
    }
    int GetCount() const { return count_elements_; }
};

int HashTable::Hash(const Triangle& triangle) const {
    int64_t hash_code =
        ((a_ * triangle.first_side) + (b_ * triangle.second_side) +
         (c_ * triangle.third_side) + d_) %
        Save::kPrime;

    return static_cast<int>(hash_code % size_);
}
bool HashTable::Search(const Triangle& val,
                       const std::vector<Triangle>& chain) {
    return std::find(chain.begin(), chain.end(), val) != chain.end();
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
    a_ = distribution(rnd);
    b_ = distribution(rnd);
    c_ = distribution(rnd);
    d_ = distribution(rnd);
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

void Triangle::Canonize() {
    if (first_side > second_side) {
        std::swap(first_side, second_side);
    }
    if (second_side > third_side) {
        std::swap(second_side, third_side);
    }
    if (first_side > second_side) {
        std::swap(first_side, second_side);
    }

    *this /= GCD(*this);
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int triangles_number;
    std::cin >> triangles_number;
    int side_one;
    int side_two;
    int side_three;
    HashTable set;
    for (int i = 0; i < triangles_number; ++i) {
        std::cin >> side_one >> side_two >> side_three;
        Triangle triangle = {side_one, side_two, side_three};
        triangle.Canonize();
        set.Insert(triangle);
    }
    std::cout << set.GetCount();
    return 0;
}
