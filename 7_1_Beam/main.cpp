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

    void SetBeam()
    {
        this->symbol = '|';
    }
    bool IsStart() const
    {
        return symbol == 'S';
    }
    bool IsSplitter() const
    {
        return symbol == '^';
    }
    bool IsBeam() const
    {
        return symbol == '|';
    }
    bool IsEmpty() const
    {
        return symbol == '.';
    }

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

struct Grid
{
    std::vector<GridNode> allGridNodes;
    size_t height;
    size_t width;
    long splitsCounter;

    long GetSplitsCounter() const
    {
        return splitsCounter;
    }

    GridNode* GetStartNode()
    {
        GridNode *ret;
        auto it = std::find_if(allGridNodes.begin(), allGridNodes.end(), [](GridNode &gridNode)
                    {
                      return gridNode.IsStart();  
                    });
        ret = &(*it);
        std::cout << "found start at: (" << ret->x << ", " << ret->y << ")\n";
        return ret;
        
    }

    void MoveDownward(GridNode* gridNode)
    {
        if (gridNode->down == nullptr) return;
        if (gridNode->down->IsSplitter())
        {
            if (gridNode->down->left != nullptr)
            {
                if (gridNode->down->left->IsEmpty())
                {
                    gridNode->down->left->SetBeam();
                    this->MoveDownward(gridNode->down->left);
                }
            }
            if (gridNode->down->right != nullptr)
            {
                if (gridNode->down->right->IsEmpty())
                {
                    gridNode->down->right->SetBeam();
                    this->splitsCounter++;
                    this->MoveDownward(gridNode->down->right);
                }
            }
            return;
        }
        if (gridNode->down->IsEmpty())
        {
            gridNode->down->SetBeam();
            this->MoveDownward(gridNode->down);
        }
    }

    void ActivateBeam()
    {
        GridNode* startNode = this->GetStartNode();
        this->MoveDownward(startNode);
    }

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

    // string filename("example_input"); // 21
    string filename("input"); // 1646

    std::vector<std::string> lines{};
    Helper::ParseFile(filename, lines);

    Grid grid{};
    grid.ParseInput(lines);
    std::cout << Grid::ToString(grid.allGridNodes) << "\n";
    std::cout << "\nAcivating Beam!\n\n";
    grid.ActivateBeam();
    std::cout << Grid::ToString(grid.allGridNodes) << "\n";
    std::cout << "GetSplitsCounter: " << grid.GetSplitsCounter() << "\n";

    // std::cout << "GetTotalSumOfEquations: " << table.GetTotalSumOfEquations() << "\n";

    return EXIT_SUCCESS;
}
