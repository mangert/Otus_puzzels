#include <iostream>
#include <vector>
#include <exception>
#include <string>

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
        void dfs(const int& n, int x, int y, std::vector<std::vector<int>>& map, int& size) {
            
            if (x < 0 || x >= n) return;
            if (y < 0 || y >= n) return;
            if (map[x][y] == 0) return;
            map[x][y] = 0;
            ++size;
            dfs(n, x, y - 1, map, size);
            dfs(n, x, y + 1, map, size);
            dfs(n, x - 1, y, map, size);
            dfs(n, x + 1, y, map, size);
        }

    
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
    int island(std::vector<std::vector<int>> map) {
        
        int max_island = 0;
        for (size_t y = 0; y != map.size(); ++y) {
            for (size_t x = 0; x != map.size(); ++x) {
                int island_ = 0;
                dfs(map.size(), x, y, map, island_);
                if (island_ > max_island)
                    max_island = island_;
            }
        }
        return max_island;
    }

} //puzzles