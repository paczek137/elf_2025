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

struct ID
{
    long long number;
    string id;
    ID(const long long _number) : number(_number), id(std::to_string(_number)) {}

    bool IsValid() const
    {
        string trickyString = this->id + this->id;
        return trickyString.substr(1, trickyString.length()-2).find(this->id) == std::string::npos;
    }
};

struct IDRange
{
    long long lowerBound;
    long long upperBound;
    vector<ID> ids;
    IDRange(const long long _lowerBound, const long long _upperBound) :
            lowerBound(_lowerBound), upperBound(_upperBound)
    {
        this->FillIds();
    }
    void FillIds()
    {
        for (long long i=this->lowerBound; i<=upperBound; i++)
        {
            ids.emplace_back(i);
        }
    }

    long long GetAllAccumulatedInvalidIds() const
    {
        return std::accumulate(this->ids.cbegin(), this->ids.cend(), 0LL, [](long long sum, const ID &id)
                                {
                                    return sum + (id.IsValid() ? 0 : id.number);
                                });
    }

    static std::string const ToString(const IDRange &idRange)
    {
        return std::to_string(idRange.lowerBound) + "-" + std::to_string(idRange.upperBound);
    }
    friend std::ostream &operator<<(std::ostream &os, const IDRange &idRange)
    {
        return os << IDRange::ToString(idRange);
    }
};

struct Shop
{
    vector<IDRange> idRanges;

    void ParseInput(const std::vector<std::string> &input)
    {
        for (auto const& line : input)
        {
            // std::regex reg("[0-9]*-[0-9]*");
            std::regex reg("([0-9]*)-([0-9]*)");
            // std::vector<string> result{};
            std::smatch match;
            auto searchStart(line.cbegin());

            while (std::regex_search(searchStart, line.cend(), match, reg))
            {
                idRanges.emplace_back(std::stoll(match[1]), std::stoll(match[2]));
                // result.push_back(match[0]);
                searchStart = match.suffix().first;
            }
        }
    }

    long long GetAllAccumulatedInvalidIds()
    {
        return std::accumulate(this->idRanges.cbegin(), this->idRanges.cend(), 0LL, [](long long sum, const IDRange &idRange)
                                {
                                    long long val = idRange.GetAllAccumulatedInvalidIds();
                                    // cout << val << "\n";
                                    // return sum + idRange.GetAllAccumulatedInvalidIds();
                                    return sum + val;
                                });
    }

    static std::string const ToString(const Shop &shop)
    {
       return boost::algorithm::join(shop.idRanges |
                boost::adaptors::transformed([shop](const IDRange &idRange)
                {
                    return IDRange::ToString(idRange);
                }), ",");
    }
    friend std::ostream &operator<<(std::ostream &os, const Shop &shop)
    {
        return os << Shop::ToString(shop);
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

    // string filename("../2_1_ProductID/example_input"); // 4174379265
    string filename("../2_1_ProductID/input"); // 38731915928

    std::vector<std::string> lines{};
    Helper::ParseFile(filename, lines);
    Shop shop{};
    shop.ParseInput(lines);
    std::cout << shop << "\n";
    std::cout << shop.GetAllAccumulatedInvalidIds() << "\n";

    return EXIT_SUCCESS;
}
