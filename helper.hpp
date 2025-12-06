#include <string>
#include <vector>
#include <fstream>
#include <iostream>

struct Helper
{
    static bool ParseFile(const std::string filename, std::vector<std::string> &output)
    {
        std::string line{};
        std::ifstream input_file(filename);
        if (not input_file.is_open())
        {
            std::cout << "Could not open the file: " << filename << std::endl;
            return false;
            // return EXIT_FAILURE;
        }

        while (getline(input_file, line, '\n')) 
        {
            output.push_back(line);
        }
        return true;
    }

    static int countDigits(long long n)
    {
        if (n == 0) return 1;
        
        n = std::abs(n); // Handle negative numbers
        int count = 0;
        while (n > 0) 
        {
            n /= 10;
            count++;
        }
        return count;
    }
};