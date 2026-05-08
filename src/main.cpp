#include <iostream>
#include "xmasTree.cpp"
#include <vector>

int main() {
	
	std::cout << "Hello" << std::endl;
	std::vector<int> tree(10);
	for (size_t i = 0; i != 6; ++i) {
		tree[i] = i+1;
	}
	tree[6] = 9;
	tree[7] = 8;
	tree[8] = 0;
	tree[9] = 3;
	int x = puzzles::maxGarland(4, tree);
	std::cout << "--- "<< x << std::endl;
	return 0;
}