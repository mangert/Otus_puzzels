#include <iostream>
#include <vector>
#include <exception>
#include <string>
#include <stack>

namespace puzzles {
    namespace {       
        
        //-------- Служебные функции для задачки про горох ------//
        //проверка на четность (для НОД)
        bool isEven(uint64_t n) {
            return !(n & 1);
        }

        //наибольший общий делитель
        uint64_t GCD(uint64_t a, uint64_t b) {
            
            if (a == 0) return b;
            if (b == 0) return a;
            if (a == b) return b;

            if (isEven(a) && isEven(b)) {
                return (GCD(a >> 1, b >> 1) << 1);
            } 

            if (isEven(a) && !isEven(b)) {
                
                return GCD(a >> 1, b);
            }
              
            if (!isEven(a) && isEven(b)) {

                return GCD(a, b >> 1);
            }
            if (a > b) return GCD(a % b, b);
            else return GCD(b % a, a);

        }

        // Функция парсинга - бросает исключения при ошибках формата
        using FourNumbers = std::tuple<uint64_t, uint64_t, uint64_t, uint64_t>;
        FourNumbers parseFractions(const std::string& input) {
            
            size_t slash1 = input.find('/');
            if (slash1 == std::string::npos) {
                throw std::invalid_argument("Missing '/' in first fraction");
            }

            size_t plus = input.find('+');
            if (plus == std::string::npos) {
                throw std::invalid_argument("Missing '+' between fractions");
            }

            size_t slash2 = input.find('/', plus);
            if (slash2 == std::string::npos) {
                throw std::invalid_argument("Missing '/' in second fraction");
            }

            try {
                uint64_t  a = std::stoull(input.substr(0, slash1));
                uint64_t b = std::stoull(input.substr(slash1 + 1, plus - slash1 - 1));
                uint64_t c = std::stoull(input.substr(plus + 1, slash2 - plus - 1));
                uint64_t d = std::stoull(input.substr(slash2 + 1));

                // Проверка диапазона
                if (a < 1 || a > 10000 || b < 1 || b > 10000 ||
                    c < 1 || c > 10000 || d < 1 || d > 10000) {
                    throw std::out_of_range("Each number must be between 1 and 10000");
                }

                // Проверка, что каждая дробь меньше 1
                if (a >= b || c >= d) {
                    throw std::invalid_argument("Each fraction must be less than 1");
                }

                return FourNumbers(a, b, c, d);
            }
            catch (const std::exception& e) {
                throw std::runtime_error("Parse error: " + std::string(e.what()));
            }
        }

        //сложение дробей
        std::pair<uint64_t, uint64_t> addFractions(uint64_t a, uint64_t b, uint64_t c, uint64_t d) {
            
            uint64_t nominator = a * d + c * b;
            uint64_t denominator = b * d;

            // Проверка, что сумма не больше 1
            if (nominator > denominator) {
                throw std::invalid_argument("Sum of fractions exceeds 1");
            }
            //сокращаем дробь
            uint64_t divider = GCD(nominator, denominator);
            return std::pair<uint64_t, uint64_t>(nominator / divider, denominator / divider);
        }

        // Функция для форматирования результата в строку
        std::string formatFraction(const std::pair<uint64_t, uint64_t>& fraction) {
            return std::to_string(fraction.first) + "/" + std::to_string(fraction.second);
        }        

        //-------- Служебные функции для задачки про острова ------//
        //рекурсивный обход соседних клеток
        int floodFill(int x, int y, int n, std::vector<std::vector<int>>& map) {
            if (x < 0 || x >= n || y < 0 || y >= n) return 0;
            if (map[y][x] == 0) return 0;

            map[y][x] = 0;  // помечаем как посещённое

            return 1 + floodFill(x, y - 1, n, map)  // вверх
                + floodFill(x, y + 1, n, map)  // вниз
                + floodFill(x - 1, y, n, map)  // влево
                + floodFill(x + 1, y, n, map); // вправо
        }

        //-------- Служебные функции для задачки про сараи ------//
            
    } //anonymos namespace

    //елочка программиста (максимальная гирлянда)
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

    //пятью восемь
    uint64_t countNoThreeInRow(uint32_t n) {
        
        constexpr uint32_t MAX_N = 88;
        
        if (n == 1) return 2; // 5, 8        
        if (n == 0) return 0;
        
        if (n > MAX_N) {
            throw std::out_of_range("n must be between 1 and " + std::to_string(MAX_N));
        }

        //а теперь считаем числа фибоначчи
        uint64_t prev = 1; // F(1)
        uint64_t curr = 1; // F(2)

        for (uint32_t i = 2; i < n; ++i) {
            uint64_t next = prev + curr;
            prev = curr;
            curr = next;
        }                

        return 2 * (prev + curr);
    }

    //раз горох - два горох
    std::string combinePeas(const std::string& input) {
        
        // Шаг 1: парсинг
        auto [a, b, c, d] = parseFractions(input);

        // Шаг 2: вычисление
        auto result = addFractions(a, b, c, d);

        // Шаг 3: форматирование
        return formatFraction(result);
        
    } 

    //большой остров
    int countIslands(std::vector<std::vector<int>> map) {
        if (map.empty()) return 0;

        int n = map.size();
        int islandCount = 0;

        for (int y = 0; y < n; ++y) {
            for (int x = 0; x < n; ++x) {
                if (map[y][x] == 1) {
                    floodFill(x, y, n, map);
                    ++islandCount;
                }
            }
        }

        return islandCount;
    }

    //сарай версия O(n^4)
    int maxSquareArea(const std::vector<std::vector<int>>& map) {
        int n = map.size();
        int m = map[0].size();
        int best = 0;

        for (int top = 0; top < n; ++top) {
            for (int left = 0; left < m; ++left) {
                if (map[top][left] == 1) continue;

                int maxHeight = n - top;
                for (int right = left; right < m; ++right) {
                    if (map[top][right] == 1) break;

                    int height = 0;
                    while (top + height < n && map[top + height][right] == 0)
                        ++height;

                    maxHeight = std::min(maxHeight, height);
                    best = std::max(best, (right - left + 1) * maxHeight);
                }
            }
        }
        return best;
    }

    //высота сарая
    std::vector<std::vector<int>> hangarHeights(const std::vector<std::vector<int>>& map) {
        int n = map.size();
        int m = map[0].size();
        std::vector<std::vector<int>> h(n, std::vector<int>(m, 0));

        for (int x = 0; x < m; ++x) {
            h[0][x] = (map[0][x] == 0) ? 1 : 0;
        }

        for (int y = 1; y < n; ++y) {
            for (int x = 0; x < m; ++x) {
                if (map[y][x] == 0) {
                    h[y][x] = h[y - 1][x] + 1;
                }
                else {
                    h[y][x] = 0;
                }
            }
        }

        return h;
    }

    //левая граница
    std::vector<int> leftEdges(const std::vector<int>& line) {
        int n = line.size();
        std::vector<int> L(n, 0);
        std::stack<std::pair<int, int>> st;  // (index, height)

        for (int i = n - 1; i >= 0; --i) {
            int current = line[i];

            // Если не первый элемент и текущая высота меньше предыдущей (справа)
            if (i < n - 1 && current < line[i + 1]) {
                // Извлекаем все элементы с высотой > текущей
                while (!st.empty() && st.top().second > current) {
                    auto [idx, h] = st.top();
                    L[idx] = i + 1;   // левая граница
                    st.pop();
                }
            }
            // Всегда добавляем текущий элемент в стек
            st.push({ i, current });
        }

        // Оставшимся в стеке L уже 0 (инициализация)
        return L;
    }

    //правая граница
    std::vector<int> rightEdges(const std::vector<int>& line) {
        int n = line.size();
        std::vector<int> R(n, n-1);
        std::stack<std::pair<int, int>> st;  // (index, height)

        for (int i = 0; i != n; ++i) {
            int current = line[i];

            // Если не первый элемент и текущая высота меньше предыдущей (слева)
            if (i > 0 && current < line[i - 1]) {
                // Извлекаем все элементы с высотой > текущей
                while (!st.empty() && st.top().second > current) {
                    auto [idx, h] = st.top();
                    R[idx] = i - 1;   // правая граница
                    st.pop();
                }
            }
            // Всегда добавляем текущий элемент в стек
            st.push({ i, current });
        }                   
        // Оставшимся в стеке уже n-1 (инициализация)
        return R;
    }    

    //сарай oптимизированная версия 
    int maxSquareAreaOpt(const std::vector<std::vector<int>>& map) {
        if (map.empty() || map[0].empty()) return 0;

        auto heights = hangarHeights(map);
        int n = heights.size();     // строки
        int m = heights[0].size();  // столбцы
        int maxArea = 0;

        for (int row = 0; row < n; ++row) {
            auto lefts = leftEdges(heights[row]);   // индекс первого меньшего слева
            auto rights = rightEdges(heights[row]); // индекс первого меньшего справа

            for (int col = 0; col < m; ++col) {
                int width = rights[col] - lefts[col] + 1; 
                int area = width * heights[row][col];
                if (area > maxArea) {
                    maxArea = area;
                }
            }
        }

        return maxArea;
    }

} //puzzles