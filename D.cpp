#include <algorithm>
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
    void Set(int lx, int rx, int root_index, int ind, int value);
    int GetCompressedIndex(int root_index, int lx, int rx, int k_stat);
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
int SegTree::GetCompressedIndex(int root_index, int lx, int rx, int k_stat) {
    if (tree_[root_index] < k_stat) {
        return -1;
    }
    if (rx - lx == 1) {
        return lx;
    }
    int left_child = (2 * root_index) + 1;
    int middle = (lx + rx) / 2;
    if (tree_[left_child] >= k_stat) {
        return GetCompressedIndex(left_child, lx, middle, k_stat);
    }
    return GetCompressedIndex(left_child + 1, middle, rx,
                              k_stat - tree_[left_child]);
}

std::vector<int> GetSortedUniqueCoordinates(std::vector<int> array) {
    std::sort(array.begin(), array.end());
    array.erase(std::unique(array.begin(), array.end()), array.end());
    return array;
}

std::vector<int> GetCompressedArray(const std::vector<int>& coordinates,
                                    const std::vector<int>& array) {
    std::vector<int> compressed(array.size());

    for (size_t i = 0; i < array.size(); ++i) {
        compressed[i] = static_cast<int>(
            std::lower_bound(coordinates.begin(), coordinates.end(), array[i]) -
            coordinates.begin());
    }

    return compressed;
}

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
    std::vector<int> coordinates = GetSortedUniqueCoordinates(array);
    std::vector<int> compressed = GetCompressedArray(coordinates, array);

    SegTree tree_of_frequency(number_elements);
    int left = 0;
    int right = 0;
    char command;
    tree_of_frequency.Set(0, tree_of_frequency.Size(), 0, compressed[0], 1);
    for (int i = 0; i < number_operations; ++i) {
        std::cin >> command;
        if (command == 'R') {
            right++;
            tree_of_frequency.Set(0, tree_of_frequency.Size(), 0,
                                  compressed[right], 1);

        } else if (command == 'L') {
            tree_of_frequency.Set(0, tree_of_frequency.Size(), 0,
                                  compressed[left], -1);
            ++left;
        }
        int compressed_index = tree_of_frequency.GetCompressedIndex(
            0, 0, tree_of_frequency.Size(), k_stat);
        if (compressed_index == -1) {
            std::cout << -1 << '\n';
        } else {
            std::cout << coordinates[compressed_index] << '\n';
        }
    }
    return 0;
}