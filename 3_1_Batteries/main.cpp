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

struct Battery
{
    size_t joltage;
    char id;
    Battery(const size_t _joltage) : joltage(_joltage), id(std::to_string(_joltage).front()) {}

    bool operator<(const Battery &battery) const
    {
        return this->joltage < battery.joltage;
    }
    static std::string const ToString(const Battery &battery)
    {
        return std::string(1, battery.id);
    }
    friend std::ostream &operator<<(std::ostream &os, const Battery &battery)
    {
        return os << Battery::ToString(battery);
    }
};

struct Bank
{
    vector<Battery> batteries;
    list<Battery> batteriesList;
    Bank(vector<Battery>& _batteries) : batteries(_batteries)
    {
        std::copy(batteries.begin(), batteries.end(), std::back_inserter(batteriesList));
        batteriesList.sort(std::less<Battery>());
    }

    long GetLargestJoltage() const
    {
        // std::string number = std::string(1, std::prev(batteriesList.cend(), 1)->id) + std::string(1, std::prev(batteriesList.cend(), 2)->id);
        std::string number = std::string(1, std::max_element(batteries.cbegin(), batteries.cend()-1)->id) + 
                             std::string(1, std::max_element(std::max_element(batteries.cbegin(), batteries.cend()-1)+1, batteries.cend())->id);
        // cout << "number: " << number << " ";
        return std::stol(number);
    }

    static std::string const ToString(const Bank &bank)
    {
        return boost::algorithm::join(bank.batteries |
                boost::adaptors::transformed([bank](const Battery &battery)
                {
                    return Battery::ToString(battery);
                }), "");
    }
    friend std::ostream &operator<<(std::ostream &os, const Bank &bank)
    {
        return os << Bank::ToString(bank);
    }
};

struct Lobby
{
    vector<Bank> banks;

    void ParseInput(const std::vector<std::string> &input)
    {
        for (auto const& line : input)
        {
            // std::stringstream ss(line);
            // vector<Battery> batteries{};
            // while (!ss.eof())
            // {
            //     size_t joltage{};
            //     ss >> joltage;
            //     batteries.emplace_back(joltage);
            // }
            // this->banks.emplace_back(batteries);

            std::regex reg("[0-9]");
            vector<Battery> batteries{};
            std::smatch match;
            auto searchStart(line.cbegin());

            while (std::regex_search(searchStart, line.cend(), match, reg))
            {
                batteries.emplace_back(std::stoul(match[0]));
                searchStart = match.suffix().first;
            }
            this->banks.emplace_back(batteries);
        }
    }

    long GetAccumulatedLargestJoltageFromAllBanks()
    {
        return std::accumulate(this->banks.cbegin(), this->banks.cend(), 0L, [](long sum, const Bank &bank)
                                {
                                    long val = bank.GetLargestJoltage();
                                    // cout << val << "\n";
                                    return sum + val;
                                });
    }

    static std::string const ToString(const Lobby &lobby)
    {
       return boost::algorithm::join(lobby.banks |
                boost::adaptors::transformed([lobby](const Bank &bank)
                {
                    return Bank::ToString(bank);
                }), "\n");
    }
    friend std::ostream &operator<<(std::ostream &os, const Lobby &lobby)
    {
        return os << Lobby::ToString(lobby);
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

    // string filename("example_input"); // 357
    string filename("input"); // 17403

    std::vector<std::string> lines{};
    Helper::ParseFile(filename, lines);

    Lobby lobby{};
    lobby.ParseInput(lines);
    std::cout << lobby << "\n";
    std::cout << "AccumulatedLargestJoltageFromAllBanks: " << lobby.GetAccumulatedLargestJoltageFromAllBanks() << "\n";

    return EXIT_SUCCESS;
}
