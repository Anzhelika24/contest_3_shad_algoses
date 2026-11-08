#ifndef FIXED_SET_H_
#define FIXED_SET_H_

#include <algorithm>
#include <cstdint>
#include <random>
#include <vector>

class FixedSet {
public:
    FixedSet() = default;

    void Initialize(const std::vector<int>& numbers);
    bool Contains(int number) const;

private:
    static constexpr int64_t kPrime = 2147483647LL;
    static constexpr int64_t kShift = 1000000000LL;
    struct Hash {
        int64_t multiplier = 1;
        int64_t increment = 0;
        int Index(int key, int length) const {
            int64_t shifted_key = static_cast<int64_t>(key) + kShift;
            int64_t mixed = ((multiplier * shifted_key) + increment) % kPrime;
            return static_cast<int>(mixed % length);
        }
    };

    struct Slot {
        int value = 0;
        bool used = false;
    };

    struct Bucket {
        int offset = 0;
        int length = 0;
        Hash hash;
    };

    Hash NewHash() {
        int64_t aa = 0;
        while (aa == 0) {
            aa = distribution_(rnd_);
        }

        int64_t bb = distribution_(rnd_);
        return {aa, bb};
    }
    void ChooseOuterHash(const std::vector<int>& numbers,
                         std::vector<int>* counts);

    Hash outer_hash_;
    std::vector<Bucket> buckets_;
    std::vector<Slot> slots_;
    std::mt19937 rnd_{std::random_device{}()};
    std::uniform_int_distribution<int64_t> distribution_{0, kPrime - 1};
};

inline void FixedSet::ChooseOuterHash(const std::vector<int>& numbers,
                                      std::vector<int>* counts) {
    const int kCount = static_cast<int>(numbers.size());
    while (true) {
        outer_hash_ = NewHash();
        std::fill(counts->begin(), counts->end(), 0);
        int64_t space = 0;
        for (int value : numbers) {
            int& bucket_size = (*counts)[outer_hash_.Index(value, kCount)];
            space += (2LL * bucket_size) + 1;
            ++bucket_size;
            if (space > 4LL * kCount) {
                break;
            }
        }
        if (space <= 4LL * kCount) {
            return;
        }
    }
}

inline void FixedSet::Initialize(const std::vector<int>& numbers) {
    std::vector<Bucket>().swap(buckets_);
    std::vector<Slot>().swap(slots_);
    const int kCount = static_cast<int>(numbers.size());
    if (kCount == 0) {
        return;
    }
    std::vector<int> counts(kCount);
    ChooseOuterHash(numbers, &counts);
    std::vector<int> begin(kCount + 1, 0);
    for (int i = 0; i < kCount; ++i) {
        begin[i + 1] = begin[i] + counts[i];
    }
    std::vector<int> next = begin;
    std::vector<int> grouped(kCount);
    for (int value : numbers) {
        int group = outer_hash_.Index(value, kCount);
        grouped[next[group]++] = value;
    }

    buckets_.resize(kCount);
    int total_cells = 0;
    for (int i = 0; i < kCount; ++i) {
        int size = counts[i];
        buckets_[i].offset = total_cells;
        buckets_[i].length = size * size;
        total_cells += size * size;
    }
    slots_.resize(total_cells);

    for (int i = 0; i < kCount; ++i) {
        Bucket& bucket = buckets_[i];
        if (bucket.length == 0) {
            continue;
        }
        if (counts[i] == 1) {
            slots_[bucket.offset] = {grouped[begin[i]], true};
            continue;
        }
        while (true) {
            bucket.hash = NewHash();
            auto first = slots_.begin() + bucket.offset;
            std::fill(first, first + bucket.length, Slot{});
            bool collision = false;
            for (int pos = begin[i]; pos < begin[i + 1]; ++pos) {
                int value = grouped[pos];
                Slot& slot = slots_[bucket.offset +
                                    bucket.hash.Index(value, bucket.length)];
                if (slot.used) {
                    collision = true;
                    break;
                }
                slot = {value, true};
            }
            if (!collision) {
                break;
            }
        }
    }
}

inline bool FixedSet::Contains(int number) const {
    if (buckets_.empty()) {
        return false;
    }
    const Bucket& bucket =
        buckets_[outer_hash_.Index(number, static_cast<int>(buckets_.size()))];
    if (bucket.length == 0) {
        return false;
    }
    const Slot& slot =
        slots_[bucket.offset + bucket.hash.Index(number, bucket.length)];
    return slot.used && slot.value == number;
}

#endif  // FIXED_SET_H_