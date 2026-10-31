#include <iostream>
#include <list>
#include <random>
namespace save {
std::mt19937 rnd(1000);
}
using save::rnd;
class HashTable {
private:
    bool Search(int val, std::list<int>& chain) {
        for (int i : chain) {
            if (i == val) {
                return true;
            }
        }
        return false;
    }
    void Insert(int val, std::list<int>& chain) {
        if (!Search(val, chain)) {
            chain.push_front(val);
            count_elements_ += 1;
        }
    }
    void Discard(int val, std::list<int>& chain) {
        if (Search(val, chain)) {
            count_elements_ -= 1;
            chain.remove(val);
        }
    }
    int size_;
    std::vector<std::list<int>> table_;
    long long a_, b_;
    int count_elements_ = 0;
    void Rehash() {
        std::vector<std::list<int>> new_table(2 * size_);
        size_ *= 2;
        a_ = rnd();
        b_ = rnd();
        for (int i = 0; i < size_ / 2; ++i) {
            for (int j : table_[i]) {
                Insert(j, new_table[Hash(j)]);
            }
        }
        table_ = new_table;
    }
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

int main() {
    int n, val, i;
    char c;
    std::cin >> n;
    HashTable table;
    for (i = 0; i < n; ++i) {
        std::cin >> c >> val;
        if (c == '+') {
            table.Insert(val);
        } else if (c == '-') {
            table.Discard(val);
        } else {
            if (table.Search(val)) {
                std::cout << "YES\n";
            } else {
                std::cout << "NO\n";
            }
        }
    }
}
