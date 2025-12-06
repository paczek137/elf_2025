#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <filesystem>
#include <memory>
#include <map>
#include <set>
// #include <boost/algorithm/string/classification.hpp> // Include boost::for is_any_of
// #include <boost/algorithm/string/split.hpp> // Include for boost::split
// #include <boost/lexical_cast.hpp>
// #include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>
// #include <boost/algorithm/string/replace.hpp>
// #include <boost/range/adaptor/tokenized.hpp>
#include <boost/range/adaptor/transformed.hpp>
// #include <boost/range/adaptors.hpp>
// #include <boost/range/algorithm/copy.hpp>
#include <chrono>
#include <thread>
#include <compare>
#include <initializer_list>
#include <ctime>
#include <concepts>
#include <regex>
#include <deque>
#include <climits>
#include <thread>
#include <execution>

using namespace std;

using myType = long long;
struct Equation
{
    std::vector<myType> components;
    char operation;
    std::function<myType(myType, myType)> operationFunction;
    myType initialValue;

    // Equation(const std::vector<myType>& _components) : components(_components) {}

    void SetOperator(const char _operation)
    {
        this->operation = _operation;
        if ('+' == this->operation)
        {
            operationFunction = std::plus<>();
            initialValue = 0LL;
        }
        if ('*' == this->operation)
        {
            operationFunction = std::multiplies<>();
            initialValue = 1LL;
        }

    }

    myType Execute() const
    {
        return std::reduce(std::execution::par, components.cbegin(), components.cend(), initialValue, operationFunction);
    }
};

struct Table
{
    std::vector<Equation> equations;

    myType GetTotalSumOfEquations()
    {
        return std::accumulate(this->equations.cbegin(), this->equations.cend(), 0LL, [this](myType sum, const Equation &equation)
                                {

                                    myType val = equation.Execute();
                                    // cout << val << "\n";
                                    // std::cout << "ID: " << ingredient.id << ": " << val << "\n";
                                    return sum + val;
                                });
    }

    void ParseInput(const std::vector<std::string> &input)
    {
        // size_t i = 1;
        size_t columnNumber = 0;
        std::regex regColumns(R"(\S+)"); // S+ means one or more non-whitespace characters
        std::smatch matches;
        std::string regInput = input.back();
        while (std::regex_search(regInput, matches, regColumns))
        {
            // cout << matches[0] << "\n";
            Equation equation{};
            equation.SetOperator(matches[0].str().front());
            this->equations.emplace_back(equation);
            regInput = matches.suffix().str();
            columnNumber++;
        }

        cout << "columns: " << columnNumber << "\n";

        for (auto const& line : input)
        {
            std::regex regComponents("[0-9]+");
            regInput = line;
            columnNumber = 0;

            while (std::regex_search(regInput, matches, regComponents))
            {
                cout << matches[0] << " ";
                this->equations.at(columnNumber).components.emplace_back(std::stoll(matches[0]));
                regInput = matches.suffix().str();
                columnNumber++;
            }
            cout << "\n";
        }
    }
};

struct Helper
{
    static bool ParseFile(const string filename, std::vector<std::string> &output)
    {
        string line{};
        ifstream input_file(filename);
        if (not input_file.is_open())
        {
            cout << "Could not open the file: " << filename << endl;
            return false;
            // return EXIT_FAILURE;
        }

        while (getline(input_file, line, '\n')) 
        {
            output.push_back(line);
        }
        return true;
    }
};


int main()
{
    setlocale(LC_ALL, "");
    srand(time(0));
    
    std::filesystem::path cwd = std::filesystem::current_path().filename();

    // string filename("example_input"); // 4277556
    string filename("input"); // 4878670269096

    std::vector<std::string> lines{};
    Helper::ParseFile(filename, lines);

    Table table{};
    table.ParseInput(lines);
    std::cout << "GetTotalSumOfEquations: " << table.GetTotalSumOfEquations() << "\n";

    return EXIT_SUCCESS;
}
