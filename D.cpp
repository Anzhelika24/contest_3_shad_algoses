#include <iostream>
#include <vector>

class SegTree {
private:
    std::vector<int> tree_;
    int size_ = 1;

public:
    int Size() const { return size_; }
    SegTree(int n) {
        while (size_ < n) {
            size_ *= 2;
        }
        tree_.resize((2 * size_) - 1, 0);
    }
    void Set(int left, int right, int root, int ind, int value);
    int Get(int x, int l, int elem, int lx, int rx);
};
void SegTree::Set(int lx, int rx, int root_index, int ind, int value) {
    if (rx - lx == 1) {
        tree_[root_index] += value;
        return;
    }
    int middle = (lx + rx) / 2;
    if (ind < middle) {
        Set(lx, middle, (2 * root_index) + 1, ind, value);
    } else {
        Set(middle, rx, (2 * root_index) + 2, ind, value);
    }
    tree_[root_index] =
        tree_[(2 * root_index) + 1] + tree_[(2 * root_index) + 2];
}
int SegTree::Get(int x, int l, int elem, int lx, int rx) {
    if ((tree_[x] < elem) || (rx - 1 < l)) {
        return -1;
    }
    if (x * 2 + 1 >= size_ * 2 - 1) {
        if (tree_[x] >= elem) {
            return lx + 1;
        }
        return -1;
    }
    int k;
    int m = (lx + rx) / 2;
    k = Get(2 * x + 1, l, elem, lx, m);
    if (k != -1) {
        return k;
    }
    return Get(2 * x + 2, l, elem, m, rx);
}

// int main() {
//   int n, m;
//   std::cin >> n >> m;
//   std::vector<int> a(n);
//   for (int i = 0; i < n; ++i) {
//     std::cin >> a[i];
//   }
//   SegTree t(n);
//   for (int i = 0; i < n; ++i) {
//     t.Set(0, t.Size(), 0, i, a[i]);
//   }
//   int k, pos, elem;
//   for (int i = 0; i < m; ++i) {
//     std::cin >> k >> pos >> elem;
//     if (k == 0) {
//       t.Set(0, t.Size(), 0, pos - 1, elem);
//     }
//     if (k == 1) {
//       std::cout << t.Get(0, pos - 1, elem, 0, t.Size()) << std::endl;
//     }
//   }
// }
int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int number_elements;
    int number_operations;
    int k_stat;
    std::cin >> number_elements >> number_operations >> k_stat;
    std::vector<int> array(number_elements);
    for (int i = 0; i < number_elements; ++i) {
        std::cin >> array[i];
    }

    return 0;
}