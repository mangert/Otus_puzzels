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
	int area = puzzles::maxSquareAreaOpt(map);
	std::cout << "------------" << area << std::endl;

	auto v = puzzles::hangarHeights(map);
	for (size_t i = 0; i != v.size(); ++i) {
		for (size_t j = 0; j != v.size(); ++j) {
			std::cout << v[i][j] << "  ";

		}
		std::cout << std::endl;
	}
	std::vector<int> line = {1, 2, 3, 2, 4, 3, 2, 3, 1, 1};
	auto res = puzzles::leftEdges(line);
	for (auto& item : res) {
		std::cout << item << " ";
	}
	std::cout << std::endl;

	res = puzzles::rightEdges(line);
	for (auto& item : res) {
		std::cout << item << " ";
	}
	std::cout << std::endl;
}