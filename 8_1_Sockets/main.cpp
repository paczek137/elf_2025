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
#include <cmath>

using namespace std;

using myType = long long;

struct Point
{
    myType x;
    myType y;
    myType z;

    Point (const myType _x, const myType _y, const myType _z) : 
                x(_x), y(_y), z(_z)  {}

    static double CalculateDistance(const Point& p1, const Point& p2)
    {
        // std::hypot calculates sqrt(x^2 + y^2 + z^2) safely
        return std::hypot(p2.x - p1.x, p2.y - p1.y, p2.z - p1.z);
    }

    static myType CalculateSquaredDistance(const Point& p1, const Point& p2)
    {
        return (p2.x - p1.x) * (p2.x - p1.x) +
               (p2.y - p1.y) * (p2.y - p1.y) +
               (p2.z - p1.z) * (p2.z - p1.z);
    }

    static std::string const ToString(const Point &point)
    {
        return std::to_string(point.x) + "," + std::to_string(point.y) + "," + std::to_string(point.z);
    }
    friend std::ostream &operator<<(std::ostream &os, const Point &point)
    {
        return os << Point::ToString(point);
    }
    friend bool operator==(const Point &l, const Point &r)
    {
        // std::cout << "comparing " << l.symbol << "==" << r.symbol << "\n";
        return (l.x == r.x) and (l.y == r.y) and (l.z == r.z);
        // return (l.symbol == r.symbol);
    }
};

using Circuit = std::vector<Point>;

struct Grid
{
    std::vector<Point> sockets;
    std::vector<Circuit> circuits;

    void ParseInput(const std::vector<string> &input)
    {
        for (auto const& line : input)
        {
            std::regex reg3dPoints("([0-9]+),([0-9]+),([0-9]+)");
            std::smatch match;
            auto searchStart(line.cbegin());

            if (std::regex_search(searchStart, line.cend(), match, reg3dPoints))
            {
                // cout << "match1: " << match[1] << ", match2: " << match[2] << "\n";
                sockets.emplace_back(std::stoul(match[1]), std::stoul(match[2]), std::stoul(match[3]));
                continue;
            }
        }
    }

    void Connect()
    {
        // Initialize with the largest possible double value
        myType min_dist_sq = std::numeric_limits<myType>::max();
        size_t i;
        size_t j;
        size_t best_i = 0;
        size_t best_j = 0;

        // Iterate through every unique pair
        // Outer loop: 0 to N-1
        for (i = 0; i < sockets.size(); ++i) {
            
            // Inner loop: i+1 to N (Avoids redundant checks and self-checks)
            for (j = i + 1; j < sockets.size(); ++j) {
                
                myType dist_sq = Point::CalculateSquaredDistance(sockets[i], sockets[j]);

                // Update minimum if found smaller
                if (dist_sq < min_dist_sq)
                {
                    min_dist_sq = dist_sq;
                    best_i = i;
                    best_j = j;
                }
            }
        }

        std::cout << "Min distance is between (" << Point::ToString(sockets[best_i]) <<
                     ") and (" << Point::ToString(sockets[best_j]) << ")\n"; 
        // Only perform sqrt once at the end
        // return std::sqrt(min_dist_sq);
    }

    static std::string const ToString(const Grid &grid)
    {
        return boost::algorithm::join(grid.sockets |
                boost::adaptors::transformed([](const Point &point)
                {
                    return Point::ToString(point);
                }), "\n");
    }
    friend std::ostream &operator<<(std::ostream &os, const Grid &grid)
    {
        return os << Grid::ToString(grid);
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

    string filename("example_input"); // 40
    // string filename("input"); // 

    std::vector<std::string> lines{};
    Helper::ParseFile(filename, lines);

    Grid grid{};
    grid.ParseInput(lines);
    std::cout << grid << "\n";

    grid.Connect();
    // std::cout << Grid::ToString(grid.allGridNodes) << "\n";
    // std::cout << "\nAcivating Beam!\n\n";
    // grid.ActivateBeam();
    // std::cout << Grid::ToString(grid.allGridNodes) << "\n";
    // std::cout << "GetSplitsCounter: " << grid.GetSplitsCounter() << "\n";

    // std::cout << "GetTotalSumOfEquations: " << table.GetTotalSumOfEquations() << "\n";

    return EXIT_SUCCESS;
}
