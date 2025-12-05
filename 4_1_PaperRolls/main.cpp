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

struct GridNode
{
    char symbol;
    size_t x;
    size_t y;
    GridNode *left = nullptr;
    GridNode *right = nullptr;
    GridNode *up = nullptr;
    GridNode *down = nullptr;

    GridNode (const char _symbol) : symbol(_symbol){}
    GridNode (const char _symbol, const size_t _x, const size_t _y) : 
                symbol(_symbol), x(_x), y(_y)  {}

    static std::string const ToString(const GridNode &node)
    {
        return std::string(1, node.symbol);
    }
    friend std::ostream &operator<<(std::ostream &os, const GridNode &node)
    {
        return os << GridNode::ToString(node);
    }
    friend bool operator==(const GridNode &l, const GridNode &r)
    {
        // std::cout << "comparing " << l.symbol << "==" << r.symbol << "\n";
        return (l.x == r.x) and (l.y == r.y) and (l.symbol == r.symbol);
        // return (l.symbol == r.symbol);
    }
};

struct GridRow
{
    std::vector<GridNode> gridRow;
};

struct Grid
{
    std::vector<GridNode> allGridNodes;
    std::vector<GridRow> gridRows;
    size_t height;
    size_t width;

    void ParseInput(const std::vector<string> &input)
    {
        this->height = input.size();
        this->width = input[0].size();
        std::cout << "height: " << this->height << ", width: " << this->width << "\n";
        size_t x = 0;
        size_t y = 0;
        for (auto const& line : input)
        {
            for (const auto &c : line)
            {
                this->allGridNodes.emplace_back(c, x++, y);
            }
            x=0;
            y++;
        }
        this->AddNeighbours();
        // this->GroupGridRows();
    }

    void AddNeighbours()
    {
        for (size_t i=0; i<this->allGridNodes.size(); i++)
        {
            // → → →
            if (((i+1) < this->allGridNodes.size()) and ((i+1)%this->width != 0))  this->allGridNodes[i].right = &this->allGridNodes[i+1];
            // ← ← ←
            if ((i > 0) and (i % this->width != 0))  this->allGridNodes[i].left = &this->allGridNodes[i-1];
            // ↑ ↑ ↑
            if (i > (this->width-1))  this->allGridNodes[i].up = &this->allGridNodes[i-this->width];
            // ↓ ↓ ↓
            if ((i+this->width) < this->width * this->height)  this->allGridNodes[i].down = &this->allGridNodes[i+this->width];
        }
    }

    void AddNodeToGridRow(const GridNode &node, GridRow &gridRow)
    {
        std::cout << "trying to add: " << node.symbol << ", x: " << node.x << ", y: " << node.y << "\n";
        if (gridRow.gridRow.cend() != std::find(gridRow.gridRow.cbegin(), gridRow.gridRow.cend(), node))
        {
            return;
        }

        gridRow.gridRow.push_back(node);
        if ( (node.left != nullptr) and ((*node.left).symbol == node.symbol)) this->AddNodeToGridRow(*(node.left), gridRow);
        if ( (node.right != nullptr) and ((*node.right).symbol == node.symbol)) this->AddNodeToGridRow(*(node.right), gridRow);
        if ( (node.up != nullptr) and ((*node.up).symbol == node.symbol)) this->AddNodeToGridRow(*(node.up), gridRow);
        if ( (node.down != nullptr) and ((*node.down).symbol == node.symbol)) this->AddNodeToGridRow(*(node.down), gridRow);
    }

    void GroupGridRows()
    {
        size_t j=0;
        for (size_t i=0; i<this->allGridNodes.size(); i++)
        {
            if (std::any_of(this->gridRows.begin(), this->gridRows.end(), [currentGridNode = this->allGridNodes[i]](GridRow &gridRow)
                            {
                                return gridRow.gridRow.cend() != std::find(gridRow.gridRow.cbegin(), gridRow.gridRow.cend(), currentGridNode);
                            }))
            {
                continue;
            }
            this->gridRows.push_back(GridRow{});
            std::cout << "New gridRow: " << this->allGridNodes[i].symbol << "\n";
            this->AddNodeToGridRow(this->allGridNodes[i], this->gridRows[j++]);
        }
        for (const auto &gridRow : this->gridRows)
        {
            std::cout << "GridNode of region: " << gridRow.gridRow[0].symbol << ", size: " << gridRow.gridRow.size() << "\n";
        }
    }

    // void CalculatePrice()
    // {
    //     size_t total=0;
    //     for (size_t i=0; i<this->fences.size(); i++)
    //     {
    //         size_t perimeter=0;
    //         for (size_t j=0; j<this->fences[i].fence.size(); j++)
    //         {
    //             if (((this->fences[i].fence[j].left != nullptr) and ((*(this->fences[i].fence[j].left)).symbol != this->fences[i].fence[j].symbol))
    //                 or ((this->fences[i].fence[j].x == 0) and (this->fences[i].fence[j].left == nullptr))) {perimeter++;}
    //             if (((this->fences[i].fence[j].right != nullptr) and ((*(this->fences[i].fence[j].right)).symbol != this->fences[i].fence[j].symbol))
    //                 or ((this->fences[i].fence[j].x == this->width-1) and (this->fences[i].fence[j].right == nullptr))) {perimeter++;}
    //             if (((this->fences[i].fence[j].up != nullptr) and ((*(this->fences[i].fence[j].up)).symbol != this->fences[i].fence[j].symbol))
    //                 or ((this->fences[i].fence[j].y == 0) and (this->fences[i].fence[j].up == nullptr))) {perimeter++;}
    //             if (((this->fences[i].fence[j].down != nullptr) and ((*(this->fences[i].fence[j].down)).symbol != this->fences[i].fence[j].symbol))
    //                 or ((this->fences[i].fence[j].y == this->height-1) and (this->fences[i].fence[j].down == nullptr))) {perimeter++;}
    //         }
    //         std::cout << "Region: " << this->fences[i].fence[0].symbol << ", area: " << this->fences[i].fence.size() << 
    //                      ", perimeter: " << perimeter << "\n";
    //         total = total +  this->fences[i].fence.size() * perimeter;
    //     }

    //     std::cout << "Total: " << total << "\n";
    // }

    static std::string const ToString(const std::vector<GridNode> &nodes)
    {
        return boost::algorithm::join(nodes |
                boost::adaptors::transformed([](const GridNode &node)
                {
                    return GridNode::ToString(node) + ((node.right == nullptr) ? "\n" : "");
                }), "");
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

    string filename("example_input"); // 13
    // string filename("input"); // 17403

    std::vector<std::string> lines{};
    Helper::ParseFile(filename, lines);

    Grid grid{};
    grid.ParseInput(lines);
    std::cout << Grid::ToString(grid.allGridNodes) << "\n";

    return EXIT_SUCCESS;
}
