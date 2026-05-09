#include <iostream>
#include "short_puzzels.cpp"
#include <vector>

int main() {
	
	auto x = puzzles::GCD(63, 14);
	std::cout << "--------" << x << std::endl;

	std::string source = "4/24+2/16";
	auto y = puzzles::combinePeas(source);
	std::cout << "---------------" << y << std::endl;

	std::vector<std::vector<int>> map = { {0, 1, 1, 0}, {0, 1, 0, 0}, {1, 0, 1, 1}, {0, 1, 1, 0} };
	for (size_t i = 0; i != map.size(); ++i) {
		for (size_t j = 0; j != map.size(); ++j) {
			std::cout << map[i][j] << "  ";

		}
		std::cout << std::endl;
	}
	int island = puzzles::island(map);
	std::cout << "------------" << island << std::endl;
}