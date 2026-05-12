#pragma once
#include <vector>
#include <string>
#include <functional>


namespace puzzles {
    //елочка программиста (максимальная гирлянда)
    int maxGarland(size_t height, std::vector<int>& tree);
    
    //пятью восемь
    uint64_t countNoThreeInRow(uint32_t n);
    
    //раз горох - два горох
    std::string combinePeas(const std::string& input);
    
    //большой остров
    int countIslands(std::vector<std::vector<int>> map); 
    
    //сарай версия O(n^4)
    int maxSquareArea(const std::vector<std::vector<int>>& map);
    
    //высота сарая
    std::vector<std::vector<int>> hangarHeights(const std::vector<std::vector<int>>& map);

    //левая граница
    std::vector<int> leftEdges(const std::vector<int>& line);   

    //правая граница
    std::vector<int> rightEdges(const std::vector<int>& line);

    //сарай oптимизированная версия 
    int maxSquareAreaOpt(const std::vector<std::vector<int>>& map);

    //обертка для сарая (создает матрицу и вызывает заданную версию расчета площади)
    int hangarAdapter(int n, int m, int t, const std::vector<std::pair<int, int>>& coords,
        std::function<int(const std::vector<std::vector<int>>&)> areaFunc);

} //namespace puzzles