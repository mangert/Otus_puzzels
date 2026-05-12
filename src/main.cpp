#include <iostream>
#include "short_puzzles.h"
#include <vector>
#include <test.h>

int main() {
	
	setlocale(LC_ALL, "Russian");

	//1. Задачка про горох
	std::cout << "\n1. Раз горох, два горох" << "\n";

	Test<std::string, const std::string&> test_peas(puzzles::combinePeas, "combinePeasTests", 1);
	test_peas.run();
	std::cout << "\n-------------------------------------------------\n";

	//2. Задачка про елочку
	std::cout << "\n2. Елочка программиста" << "\n";	
	Test<int, size_t, std::vector<int>&> test_xmasTree(puzzles::maxGarland, "xmasTreeTests", 1);
	test_xmasTree.run();
	std::cout << "\n-------------------------------------------------\n";

	//3. Пятью восемь
	std::cout << "\n3. Пятью восемь" << "\n";	
	Test<uint64_t, uint32_t> test_countNoThree(puzzles::countNoThreeInRow, "FiveEightTests", 1);
	test_countNoThree.run();
	std::cout << "\n-------------------------------------------------\n";

	//4. Большой остров 
	std::cout << "\n4. Большой остров" << "\n";
	auto islandsWrapper = [](int, std::vector<std::vector<int>> map) {
		return puzzles::countIslands(map);
		};
	Test<int, int, std::vector<std::vector<int>>> test_islands(
		islandsWrapper,
		"Island",
		1  // показываем только первую строку (размер)
	);
	test_islands.run();
	std::cout << "\n-------------------------------------------------\n";

	// Сарай O(N^4) — тупой перебор	
	auto hangarBrute = [](int n, int m, int t, const std::vector<std::pair<int, int>>& coords) {
		return puzzles::hangarAdapter(n, m, t, coords, puzzles::maxSquareArea);
		};
	std::cout << "\n5. Сарай (тупая версия)" << "\n";
	Test<int, int, int, int, std::vector<std::pair<int, int>>> test_hangarBrute(
		hangarBrute,
		"Hangar",
		2  // показываем первые 2 аргумента: N M и T
	);
	test_hangarBrute.run();
	std::cout << "\n-------------------------------------------------\n";
	// Сарай оптимизированная версия
	auto hangarOpt = [](int n, int m, int t, const std::vector<std::pair<int, int>>& coords) {
		return puzzles::hangarAdapter(n, m, t, coords, puzzles::maxSquareAreaOpt);
		};
	std::cout << "\n6. Сарай (оптимизированная)" << "\n";
	Test<int, int, int, int, std::vector<std::pair<int, int>>> test_hangarOpt(
		hangarOpt,
		"Hangar",
		2
	);
	test_hangarOpt.run();
	
	return 0;
}