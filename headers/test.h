#pragma once
#include <iostream>
#include <functional>
#include <filesystem>
#include <fstream>
#include <chrono>
#include <vector>
#include <future>

template<typename ResultType, typename... Args>
class Test {
public:
    using TestFunction = std::function<ResultType(Args...)>;

    Test(TestFunction run, const std::string& test_folder, size_t maxArgsToPrint)
        : test_run(run), folder(test_folder), maxArgsToPrint(maxArgsToPrint) {
    }

    void run() {
        namespace fs = std::filesystem;

        int iter = 0;
        while (true) {
            std::string fileIn = getTestPath(iter, "in");
            std::string fileOut = getTestPath(iter, "out");

            if (!fs::exists(fileIn) || !fs::exists(fileOut)) break;

            std::vector<std::string> inputs = readFileLines(fileIn);
            std::string expected = readFile(fileOut);  // результат всегда одна строка

            _run(iter, inputs, expected);
            ++iter;
        }
    }

private:
    //внутренняя функция запуска одного теста
    void _run(int testNum, const std::vector<std::string>& inputs, const std::string& expected) {
        // Форматируем аргументы для вывода
        std::string args_str = formatArgs(inputs);

        std::ostringstream out_stream;
        out_stream << "Тест " << std::setw(2) << std::setfill('0') << testNum;

        auto start = std::chrono::high_resolution_clock::now();

        try {

            auto result = call_function(inputs);
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

            // Проверяем результат
            if constexpr (std::is_arithmetic_v<ResultType>) {

                ResultType result_val = result;
                ResultType expected_val = std::stoull(expected);

                double tolerance = 1e-10;
                bool is_equal;

                if constexpr (std::is_integral_v<ResultType>) {
                    is_equal = (result_val == expected_val);
                }
                else {
                    is_equal = (std::abs(result_val - expected_val) < tolerance * std::abs(expected_val));
                }

                if (is_equal) {
                    out_stream << " OK: " << args_str << " результат: ";
                    if constexpr (std::is_integral_v<ResultType>) {
                        out_stream << result_val;
                    }
                    else {
                        out_stream << std::setprecision(15) << result_val;
                    }
                    out_stream << " время: " << duration << "us";
                }
                else {
                    double diff = std::abs(static_cast<double>(result_val) - static_cast<double>(expected_val));
                    out_stream << " ОШИБКА: " << args_str << " результат: ";
                    if constexpr (std::is_integral_v<ResultType>) {
                        out_stream << result_val;
                    }
                    else {
                        out_stream << std::setprecision(15) << result_val;
                    }
                    out_stream << " ожидалось: ";
                    if constexpr (std::is_integral_v<ResultType>) {
                        out_stream << expected_val;
                    }
                    else {
                        out_stream << std::setprecision(15) << expected_val;
                    }
                    out_stream << " разница: " << std::scientific << std::setprecision(3) << diff
                        << " время: " << duration << "us";
                }
            }
            else {
                std::string result_str = convert_result(result);
                if (result_str == expected) {
                    out_stream << " OK: " << args_str << " результат: " << result_str
                        << " время: " << duration << "us";
                }
                else {
                    out_stream << " ОШИБКА: " << args_str << " результат: " << result_str
                        << " ожидалось: " << expected << " время: " << duration << "us";
                }
            }
        }
        catch (const std::exception& e) {
            out_stream << " НЕ ВЫПОЛНЕН: " << args_str << " ошибка: " << e.what();
        }

        std::cout << out_stream.str() << std::endl;
    }

    // Упрощенная версия для конкретных случаев (неуниверсальная)
    ResultType call_function(const std::vector<std::string>& inputs) {
        // 1. uint64_t(uint32_t) — пятью восемь
        if constexpr (std::is_same_v<ResultType, uint64_t> && sizeof...(Args) == 1) {
            uint32_t arg = static_cast<uint32_t>(std::stoul(inputs[0]));
            return test_run(arg);
        }
        // 2. double(double, uint64_t) — степень (старое)
        else if constexpr (std::is_same_v<ResultType, double> && sizeof...(Args) == 2) {
            double arg1 = std::stod(inputs[0]);
            uint64_t arg2 = static_cast<uint64_t>(std::stoull(inputs[1]));
            return test_run(arg1, arg2);
        }
        // 3. std::string(const std::string&) — горох
        else if constexpr (std::is_same_v<ResultType, std::string> && sizeof...(Args) == 1) {
            return test_run(inputs[0]);
        }
        // 4. Елочка, острова
        else if constexpr (std::is_same_v<ResultType, int> && sizeof...(Args) == 2) {
            using SecondArg = std::tuple_element_t<1, std::tuple<Args...>>;

            if constexpr (std::is_same_v<std::remove_reference_t<SecondArg>, std::vector<int>>) {
                // Ёлочка: int(size_t, std::vector<int>)
                size_t height = static_cast<size_t>(std::stoull(inputs[0]));

                std::vector<int> tree;
                for (size_t i = 1; i < inputs.size(); ++i) {
                    std::stringstream ss(inputs[i]);
                    int num;
                    while (ss >> num) {
                        tree.push_back(num);
                    }
                }

                return test_run(height, tree);
            }
            else if constexpr (std::is_same_v<SecondArg, std::vector<std::vector<int>>>) {
                // Острова через лямбду: int(int, std::vector<std::vector<int>>)
                // Первый аргумент (размер) игнорируем, но он есть в inputs[0]
                int n = std::stoi(inputs[0]);  // можно использовать для проверки

                std::vector<std::vector<int>> matrix;
                for (size_t i = 1; i < inputs.size(); ++i) {
                    std::stringstream ss(inputs[i]);
                    std::vector<int> row;
                    int val;
                    while (ss >> val) {
                        row.push_back(val);
                    }
                    if (!row.empty()) {
                        matrix.push_back(row);
                    }
                }

                // Вызываем лямбду: первый аргумент (n) передаём, но лямбда его проигнорирует
                return test_run(n, matrix);
            }
        }
        // 5. Сарай
        else if constexpr (std::is_same_v<ResultType, int> && sizeof...(Args) == 4) {
            // Сарай: int(int, int, int, std::vector<std::pair<int, int>>)
            // Формат входных данных:
            // inputs[0] = "N M"
            // inputs[1] = "T"
            // inputs[2..T+1] = "X Y"

            // Парсим первую строку: N M
            std::stringstream ssNM(inputs[0]);
            int n, m;
            ssNM >> n >> m;

            // Парсим вторую строку: T
            int t = std::stoi(inputs[1]);

            // Парсим координаты построек
            std::vector<std::pair<int, int>> coords;
            for (int i = 0; i < t; ++i) {
                std::stringstream ssCoord(inputs[2 + i]);
                int x, y;
                ssCoord >> x >> y;
                coords.emplace_back(x, y);
            }

            // Вызываем тестируемую функцию
            return test_run(n, m, t, coords);
        }

        throw std::invalid_argument("Unsupported function signature");
    }

    double parse_double(const std::string& str) {
        std::stringstream ss(str);
        double result;
        ss >> result;
        return result;
    }

    std::string convert_result(const ResultType& result) {
        if constexpr (std::is_same_v<ResultType, double>) {
            // Для double используем точку вместо запятой
            std::stringstream ss;
            ss << std::fixed << std::setprecision(12) << result;
            std::string str = ss.str();
            // Убираем лишние нули в конце
            str.erase(str.find_last_not_of('0') + 1, std::string::npos);
            if (str.back() == '.') str.pop_back();
            return str;
        }
        else if constexpr (std::is_same_v<ResultType, std::string>) {
            return result;
        }
        else {
            return std::to_string(result);
        }
    }

    // Чтение одной строки (для файлов с результатом)
    std::string readFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) throw std::runtime_error("Cannot open: " + filename);
        std::string content;
        std::getline(file, content);
        file.close();
        return content;
    }

    // Чтение всех строк (для входных файлов)
    std::vector<std::string> readFileLines(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) throw std::runtime_error("Cannot open: " + filename);

        std::vector<std::string> lines;
        std::string line;
        while (std::getline(file, line)) {
            if (!line.empty()) {
                lines.push_back(line);
            }
        }
        file.close();
        return lines;
    }

    std::string getTestPath(int testNum, const std::string& extension) {
        return std::string(TEST_DIR) + "/" + folder + "/test." +
            std::to_string(testNum) + "." + extension;
    }

    //форматирование аргументов для вывода
    std::string formatArgs(const std::vector<std::string>& inputs) {
        if (maxArgsToPrint == 0) {
            return "";
        }

        std::string result;
        for (size_t i = 0; i < inputs.size() && i < maxArgsToPrint; ++i) {
            if (i > 0) result += ", ";

            std::string line = inputs[i];
            // Обрезаем слишком длинные строки (например, строку матрицы)
            if (line.length() > 30) {
                line = line.substr(0, 27) + "...";
            }
            result += line;
        }

        if (inputs.size() > maxArgsToPrint) {
            result += ", ... (" + std::to_string(inputs.size() - maxArgsToPrint) + " more)";
        }

        return result;
    }

private:
    TestFunction test_run;
    std::string folder;
    size_t maxArgsToPrint;  // сколько аргументов показывать (0 - не показывать)
};