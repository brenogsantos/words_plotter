#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

void moveLine(const string& inputFileName, const string& outputFileName, int fromLine, int toLine) {
    // Read the input file into a vector of lines
    ifstream inputFile(inputFileName);
    vector<string> lines;
    string line;
    while (getline(inputFile, line)) {
        lines.push_back(line);
    }
    inputFile.close();
    
    // Move the line to the new position
    string movedLine = lines[fromLine - 1];
    lines.erase(lines.begin() + fromLine - 1);
    lines.insert(lines.begin() + toLine - 1, movedLine);
    
    
    // read each line in the file
    int count_lines {1};
    for(auto &line: lines) {
        size_t start_pos = line.find('\t');  // find the first tab
        size_t end_pos = line.find('\t', start_pos + 1);  // find the second tab

        // check if the second tab is found
        if (end_pos != std::string::npos) {
            std::string new_value = std::to_string(count_lines);  // replace "10" with "99"
            line.replace(start_pos + 1, end_pos - start_pos - 1, new_value);
            std::cout << line << std::endl;  // print the modified line
        }
        count_lines ++;
        if (count_lines > 5) count_lines = 1;
        
    }


    // Write the output file
    ofstream outputFile(outputFileName);
    for (const auto& line : lines) {
        outputFile << line << endl;
    }
    outputFile.close();
}

int main() {
    moveLine("output_test.txt", "output_test.txt", 5, 2);
    return 0;
}