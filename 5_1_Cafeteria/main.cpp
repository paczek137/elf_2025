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

using namespace std;

struct Ingredient 
{
    long long id;
    string symbol;

    Ingredient(const long long _id) : id(_id), symbol(std::to_string(_id)) {}

    static std::string const ToString(const Ingredient &ingredient)
    {
        return ingredient.symbol;
    }
    friend std::ostream &operator<<(std::ostream &os, const Ingredient &ingredient)
    {
        return os << Ingredient::ToString(ingredient);
    }
};

struct IngredientRange
{
    Ingredient lowerBound;
    Ingredient upperBound;

    IngredientRange(const Ingredient _lowerBound, const Ingredient _upperBound) :
        lowerBound(_lowerBound), upperBound(_upperBound) {}

    bool IsFresh(const Ingredient& ingredient) const
    {
        return (ingredient.id >= this->lowerBound.id) && (ingredient.id <= this->upperBound.id);
    }

    static std::string const ToString(const IngredientRange &ingredientRange)
    {
        return Ingredient::ToString(ingredientRange.lowerBound) + "-" + Ingredient::ToString(ingredientRange.upperBound);
    }
    friend std::ostream &operator<<(std::ostream &os, const IngredientRange &ingredientRange)
    {
        return os << IngredientRange::ToString(ingredientRange);
    }
};

struct Cafeteria
{
    std::vector<IngredientRange> freshIngredients;
    std::vector<IngredientRange> freshIngredientsCleaned;
    std::vector<Ingredient> availableIngredients;

    // void CleanIngredientRanges()
    // {

    // }

    long long GetNumberOfFreshIngredients()
    {
        return std::accumulate(this->availableIngredients.cbegin(), this->availableIngredients.cend(), 0LL, [this](long long sum, const Ingredient &ingredient)
                                {

                                    long long val = std::any_of(freshIngredients.cbegin(), freshIngredients.cend(), [ingredient](const IngredientRange& ingredientRange)
                                                        {
                                                            return ingredientRange.IsFresh(ingredient);
                                                        }) ? 1 : 0;
                                    // cout << val << "\n";
                                    // std::cout << "ID: " << ingredient.id << ": " << val << "\n";
                                    return sum + val;
                                });
    }

    void ParseInput(const std::vector<std::string> &input)
    {
        for (auto const& line : input)
        {
            std::regex regFreshIngredients("([0-9]+)-([0-9]+)");
            std::regex regAvailableIngredients("([0-9]+)");
            std::smatch match;
            auto searchStart(line.cbegin());

            if (std::regex_search(searchStart, line.cend(), match, regFreshIngredients))
            {
                // cout << "match1: " << match[1] << ", match2: " << match[2] << "\n";
                freshIngredients.emplace_back(std::stoll(match[1]), std::stoll(match[2]));
                continue;
            }
            if (std::regex_search(searchStart, line.cend(), match, regAvailableIngredients))
            {
                // cout << "match1: " << match[1] << "\n";
                availableIngredients.emplace_back(std::stoll(match[1]));
            }
        }
    }

    static std::string const ToString(const Cafeteria &cafeteria)
    {
        return boost::algorithm::join(cafeteria.freshIngredients |
                    boost::adaptors::transformed([cafeteria](const IngredientRange &ingredientRange)
                    {
                        return IngredientRange::ToString(ingredientRange);
                    }), "\n") +
                "\n" +
                boost::algorithm::join(cafeteria.availableIngredients |
                    boost::adaptors::transformed([cafeteria](const Ingredient &ingredient)
                    {
                        return Ingredient::ToString(ingredient);
                    }), "\n");
    }
    friend std::ostream &operator<<(std::ostream &os, const Cafeteria &cafeteria)
    {
        return os << Cafeteria::ToString(cafeteria);
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

    // string filename("example_input"); // 3
    string filename("input"); // 739

    std::vector<std::string> lines{};
    Helper::ParseFile(filename, lines);

    Cafeteria cafeteria{};
    cafeteria.ParseInput(lines);
    std::cout << cafeteria << "\n";
    // cafeteria.CleanIngredientRanges();
    // std::cout << cafeteria << "\n";
    std::cout << "GetNumberOfFreshIngredients: " << cafeteria.GetNumberOfFreshIngredients() << "\n";

    return EXIT_SUCCESS;
}
