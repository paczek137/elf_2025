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
// #include <boost/algorithm/string/join.hpp>
// #include <boost/algorithm/string/replace.hpp>
// #include <boost/range/adaptor/tokenized.hpp>
// #include <boost/range/adaptor/transformed.hpp>
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

struct Safe
{
    size_t zeroCounter;
    int currentPosition;
    int lowerBound;
    int upperBound;

    Safe(const size_t _lowerBound, const size_t _upperBound, const size_t _currentPosition) :
        lowerBound(_lowerBound),
        upperBound(_upperBound+1), 
        currentPosition(_currentPosition), 
        zeroCounter(0) {}

    size_t GetZeroCounter() const { return zeroCounter; }
    int GetCurrentPosition() const { return currentPosition; }

    void Move(const char direction, const int steps)
    {
        if ('L' == direction)
        {
            if (steps >= this->GetCurrentPosition())
            {
                cout << "0: " << this->GetZeroCounter() << ", " << this->GetCurrentPosition() << "L" << steps << "\n";
                if (0 != this->GetCurrentPosition())
                {
                    this->zeroCounter += 1;
                }
                this->zeroCounter += std::abs((this->currentPosition - steps) / this->upperBound);
                this->currentPosition = (((this->currentPosition - steps) % this->upperBound) + this->upperBound) % this->upperBound;
            }
            else
            {
                this->currentPosition = this->currentPosition - steps;
            }
        }
        else if ('R' == direction)
        {
            if (this->currentPosition + steps >= this->upperBound)
            {
                cout << "0: " << this->GetZeroCounter() << ", " << this->GetCurrentPosition() << "R" << steps << "\n";
                this->zeroCounter += (this->currentPosition + steps) / this->upperBound;
            }
            this->currentPosition = (this->currentPosition + steps) % this->upperBound;
        }
        else cout << "Wrong direction!!!\n";
        cout << "zeroCounter: " << zeroCounter << "\n";
        // if (0 == this->GetCurrentPosition()) { this->zeroCounter++; }
    }

    void ParseInput(const std::vector<std::string> &input)
    {
        for (auto const& line : input)
        {
            std::stringstream ss(line);
            while (!ss.eof())
            {
                char direction{};
                int number{};
                ss >> direction;
                ss >> number;
                this->Move(direction, number);
                // cout << line << "-> " << this->GetCurrentPosition() << "\n";
            }
        }
    }
};



int main()
{
    setlocale(LC_ALL, "");
    srand(time(0));
    
    std::filesystem::path cwd = std::filesystem::current_path().filename();

    // string filename("../1_1_Safe/example_input"); // 6
    string filename("../1_1_Safe/input"); // 6860
    ifstream input_file(filename);

    string line{};
 
    if (not input_file.is_open())
    {
        cout << "Could not open the file: " << filename << endl;
        return EXIT_FAILURE;
    }
    
    std::vector<std::string> lines{};

    while (getline(input_file, line, '\n')) 
    {
        lines.push_back(line);
    }

    Safe safe(0, 99, 50);
    safe.ParseInput(lines);
    std::cout << "zero counter: " << safe.GetZeroCounter() << "\n";


    return EXIT_SUCCESS;
}
