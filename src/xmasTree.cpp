#include <iostream>
#include <vector>
#include <exception>
#include <string>

namespace puzzles {

    int maxGarland(size_t height, std::vector<int>& tree) {
        
        if (tree.empty()) return 0;

        size_t expected = height * (height + 1) / 2;
        
        if (tree.size() != expected) {
            throw std::invalid_argument(
                "Invalid tree: height=" + std::to_string(height) +
                ", vector size=" + std::to_string(tree.size()) +
                ", expected=" + std::to_string(expected)
            );
        }

        if (height == 1) return tree[0];

        size_t start_idx = tree.size() - 2 * height + 1;  // начало предпоследнего уровня

        for (size_t lvl = height - 1; lvl > 0; --lvl) {
            for (size_t i = 0; i < lvl; ++i) {
                size_t cur = start_idx + i;
                int left = tree[cur + lvl];
                int right = tree[cur + lvl + 1];
                tree[cur] += std::max(left, right);
            }
            start_idx -= lvl - 1;  // переход на уровень выше
        }

        return tree[0];
    }

} //puzzles