#include "../include/processor.h"


void loopInputFiles(Words& w)
{

    w.inputNewFileFromTxt();

    int num_inputs {w.filenamesFromTxt.size()};


    //std::cout << "Enter the num. of files: ";
    //std::cin >> num_inputs;

    int cont {0};
    while(cont < num_inputs)
    {
            //w.inputNewFile();
            w.inputNewFileByIndex(cont);
            w.readTopWordsFromFile(w.getLastVecPosition(), 10);
            w.logSeekWordsQty(cont);
            std::cout << "\n\n";

            cont++;
    }  
    //std::cout << "Press any key to exit" << std::endl;
    std::cin.ignore();

}


void Words::inputNewFileFromTxt()
{
    word_count.clear();
    while(!top_words.empty()) top_words.pop();

    std::ifstream file("filenames.txt");
            if (file) {
                std::string line;
                while (std::getline(file, line)) {
                    std::istringstream iss(line);
                    std::string word;
                    while (iss >> word) {
                        
                        filenamesFromTxt.push_back(word);
                    }
                }

            } else {
                std::cerr << "Failed to open file: filenames.txt" << std::endl;
            }

}

void loopInputSeekWords(Words& w)
{
    int num_inputs {0};

    std::cout << "Enter the words, separated by space: ";
    

    std::string line; 

    std::getline(std::cin, line); 


    std::istringstream iss(line);
    std::string word;
    while (iss >> word) {
        w.seek_words.push_back(word);

    }

    std::cout << "You entered " << w.seek_words.size() << " words:" << std::endl;
    for (const auto& w : w.seek_words) {
        std::cout << w << std::endl;
    }


   // std::cin.ignore();

}





std::string cleanString(std::string& str) {
    str.erase(std::find_if(str.begin(), str.end(), [](unsigned char c) { return std::ispunct(c); }), str.end());
    std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c){ return std::tolower(c); });
    return str;
}


namespace INVALID_WORDS
{
    void logInvalidWords(std::string filename)
    {
        std::ifstream file(std::move(filename));
        if (file) {
            std::string line;
            while (std::getline(file, line)) {
                std::istringstream iss(line);
                std::string word;
                while (iss >> word) {
                    
                    invalid.insert(word);
                }
            }

        } else {
            std::cerr << "Failed to open file: " << filename << std::endl;
        }

    }

    void printInvalidWords()
    {
        for (auto &n : invalid)
        {
            std::cout << n << "\n";
        }
    }

    bool checkInvalidWord(std::string word)
    {
        std::string word_clean = cleanString(word);
        //std::transform(word.begin(), word.end(), word.begin(), ::tolower);
        return invalid.count(word_clean)  ;
    }


}




Words::Words(/* args */)
{
}

Words::~Words()
{
}

void Words::inputNewFile()
{
    word_count.clear();
    while(!top_words.empty()) top_words.pop();

    std::string filename;
    std::cout << "Enter the filename: ";
    std::cin >> filename;
    filenames.push_back(filename + ".txt");
}

void Words::inputNewFileByIndex(const int index)
{
    word_count.clear();
    while(!top_words.empty()) top_words.pop();

    filenames.push_back(filenamesFromTxt[index] + ".txt");
}


void Words::readTopWordsFromFile(const int index, int num_topWords)
{
    
    std::ifstream file(filenames[index]);

     if (file) {
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string word;
            while (iss >> word) {
                ++word_count[cleanString(word)];
            }
        }
        for (const auto& entry : word_count) {
           // if(entry.first != "-" && entry.first != "●")
            if(!INVALID_WORDS::checkInvalidWord(entry.first))
            top_words.emplace(entry.second, entry.first);
        }

        std::cout << "Top " << num_topWords << " most frequent words:" << "\n";

        std::unique_ptr<std::priority_queue<std::pair<int, std::string>>> temp(new std::priority_queue<std::pair<int, std::string>>(top_words));
        int count = 0;
        while (!temp->empty() && count < num_topWords) {
            std::cout << temp->top().second << " (" << temp->top().first << " occurrences)" << "\n";
            temp->pop();
            count++;
        }

       
        
        
    } else {
        std::cerr << "Failed to open file: " << filenames[index] << std::endl;
    }

}



void Words::plotData()
{
    FILE* gnuplot = popen("gnuplot", "w");
    fprintf(gnuplot, "set grid lw 2\n");

    fprintf(gnuplot, "set xlabel 'String'\n");
    fprintf(gnuplot, "set ylabel 'Repetitions'\n");
    fprintf(gnuplot, "set style data lines\n");

        int i = 0;
        std::string str_xAxis = "set xtics (";

        while(i < filenames.size())
        {

            str_xAxis += "'";
            str_xAxis += filenames[i];
            if(i == filenames.size()) "' " + std::to_string(i+1);
            else str_xAxis += "' " + std::to_string(i+1) + ", ";
            
            i++;
        }
        str_xAxis += ")\n";
        
    //fprintf(gnuplot, "set xtics ('1st' 1, '2nd' 2, '3rd' 3)\n");
    fprintf(gnuplot, str_xAxis.c_str());

    std::string str_xAxis_size = "set for [i=1:";

    str_xAxis_size += std::to_string(filenames.size());
    str_xAxis_size += "] xtics add (i 1)\n";
    fprintf(gnuplot, str_xAxis_size.c_str());
    //fprintf(gnuplot, "set for [i=1:4] xtics add (i 1)\n");



    fprintf(gnuplot, "set xtics scale 0,1\n");

    std::string str_xAxis_range = "set xrange [1:";
    str_xAxis_range += std::to_string(filenames.size());
    str_xAxis_range +=  "]\n";
    fprintf(gnuplot, str_xAxis_range.c_str());
    // fprintf(gnuplot, "set xrange [1:3]\n");
    
    
    int base {filenames.size() - 1};
    std::string str1 = "plot 'output.txt' every ::0::" + std::to_string((base)) +  " using 2:1 title ";
    std::string test1 =  "'" + std::string(seek_words[0]) + "'" + ", \ ";
    //std::string str1 = "plot 'output.txt' every ::0::2 using 2:1 title ";

    int count {0};
    std::string str_rest {};
    base++;
    while(count < seek_words.size())
    {    
            std::cout << "aaaasds "<< "::" << base << "::" << base + filenames.size() -1 << "\n";
            str_rest += "'output.txt' every ::" + std::to_string(base) + "::" + std::to_string((base) + filenames.size() -1 )
            +  " using 2:1 title ";
            if(count == (seek_words.size() - 1))
            {
                str_rest +=  "'" + seek_words.back() + "'" + "\n";
                
            } 
            else 
            {
                    str_rest +=  "'" + seek_words[count+1] + "'" + ", \ ";
            }

            base += (filenames.size());
            count++;
            
    }

    std::string result = str1 + test1 + str_rest;

    fprintf(gnuplot, result.c_str());

        
        
        
    fflush(gnuplot);

        // Wait for Gnuplot to complete
    std::this_thread::sleep_for(std::chrono::seconds(10));

        // Close the pipe
        //pclose(gnuplot);

}


void Words::logSeekWordsQty(const int index_File)
{
     
    int cont {0};
    std::unique_ptr<std::priority_queue<std::pair<int, std::string>>> temp(new std::priority_queue<std::pair<int, std::string>>(top_words));

   // std::cout << "aaaasds "<< temp->top().second << "\n";
    std::vector<std::string> not_present_str {};
    while (!temp->empty()) {
        //not_present_str.clear();
        for(auto &str : seek_words)
        {
            if (temp->top().second == str)
            {
                std::cout << " livro: " << filenames[index_File] << "  " << temp->top().second << "   " << temp->top().first << "\n";
                words_totalMap[temp->top().second].emplace_back(temp->top().first, filenames[index_File]);

            }
            else
            {
                 /*   auto is_string_in_container = [&not_present_str](const std::string& s) {
                        return std::find(not_present_str.begin(), not_present_str.end(), s) != not_present_str.end();
                    };

                    if (!is_string_in_container(str)) 
                    {
                        not_present_str.push_back(str);
                        words_totalMap[temp->top().second].emplace_back(0, filenames[index_File]);
                    }*/
            }
            
        }
        temp->pop();
    }




}

void Words::printWordsMap()
{
    
    std::cout << "Word\tFrequency\tFiles\n";
    for (const auto& pair : words_totalMap) {
        std::cout << pair.first << "\t";
        for (const auto& p : pair.second) {
            std::cout << p.first << "   " << p.second << "\t";
        }
        std::cout << std::endl;
    }


}

std::string Words::getIndexNumOnTxt(const std::string& filename)
{

    auto it = std::find(filenames.begin(), filenames.end(), filename);
    auto pos = std::distance(filenames.begin(), it) + 1;
    if (it != filenames.end()) {
     //   std::cout << "Found at position " << std::distance(filenames.begin(), it) << "\n";
    } else {
      //  std::cout << "Not found" << "\n";
    }
    return std::to_string(pos);


}

std::vector<std::string> Words::fixLinesNumsOnTxt(std::vector<std::string>& lines)
{
    int line_counter {1};
    std::string sub {0};
    lines.resize(filenames.size());

    for(auto line : lines)
    {

        std::cout << "LINHAS: " << lines.size() << "  linha:  "  << line << "\n";


        sub = line.substr(line.find('\t')+1);
        sub = sub.substr(0, sub.find('\t'));

        if(std::stoi(sub) > line_counter)
        {
            std::cout << "Sub eh maior  " << sub << "\t" << "line counter:  " << line_counter << "\n";
            lines.insert(lines.begin() + (line_counter - 1), "0\t"+std::to_string(line_counter)+"\t"+filenames[line_counter-1]+"\n");
            line_counter++;
        }

        else
        {
            std::cout << "sub eh igual  " << sub << "\t" << "line counter:  " << line_counter << "\n";
            line_counter++;
        }


    }

    return lines;

}

void Words::saveToTxtWordsMap()
{
    
// Open the file for writing
    std::ofstream outfile("output.txt");
    std::vector<std::string> lines {""};
    int count {1};
    int count_lines_printed {0};
    int flag_buffer_complete {0};
    // Iterate over the map and write each element to the file

    for(auto &n: seek_words)
    {
        lines.clear();
        auto const& value = words_totalMap[n];
        count = 1;
        for (auto const& pair : value) {
            //lines += std::to_string(pair.first) + "\t" +  std::to_string(count) + '\t' + pair.second + '\n';
            //outfile << pair.first << '\t'<< count << '\t' << pair.second << '\n';
            lines.push_back(std::to_string(pair.first) + "\t" +  getIndexNumOnTxt(pair.second) + '\t' + pair.second + '\n');
            count++;
        }
        /*while(count < (filenames.size() + 1))
        {
           // lines += std::to_string(0) + "\t" +  std::to_string(count) + '\t' + "" + '\n';
            outfile << 0 << '\t'<< count << '\t' << "" << '\n';
            count++;
            flag_buffer_complete = 1;
        }*/
      //  std::cout << "count  " << count << "  filenames:  " << filenames.size() << "\n";
        if (count < (filenames.size() + 1))
        {
            lines = fixLinesNumsOnTxt(lines);
            for(auto & line: lines)
            outfile << line;

            
        }
        else
        {
            for(auto & line: lines)
            outfile << line;
        }

    }


    // Close the file
    outfile.close();

    
}