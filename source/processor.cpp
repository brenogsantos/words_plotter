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
    
    //std::vector<std::string> seek_words; // Vector to store the input strings
    std::string line; // String to store the input line

    std::getline(std::cin, line); // Read the entire line of input

    // Use an istringstream to read space-separated strings from the input line
    std::istringstream iss(line);
    std::string word;
    while (iss >> word) {
        w.seek_words.push_back(word); // Add each word to the vector

    }

    // Print the contents of the vector
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
            // print the top 5 elements
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

/*int Words::getLastVecPosition() {
    return filenames.size() - 1;
}*/

void Words::plotData()
{
    FILE* gnuplot = popen("gnuplot", "w");
    fprintf(gnuplot, "set grid lw 2\n");
    /*fprintf(gnuplot, "plot \"data.dat\" using 1:2 title 'Column' with lines, \
                      \"data.dat\" using 1:3 title 'Beam' with lines\n");
    */
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
            std::cout << p.first << "   " << p.second << "\t\t";
        }
        std::cout << std::endl;
    }


}

void Words::saveToTxtWordsMap()
{
    
// Open the file for writing
    std::ofstream outfile("output.txt");
    std::string lines {""};
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
            outfile << pair.first << '\t'<< count << '\t' << pair.second << '\n';
            count++;
        }
        while(count < (filenames.size() + 1))
        {
           // lines += std::to_string(0) + "\t" +  std::to_string(count) + '\t' + "" + '\n';
            outfile << 0 << '\t'<< count << '\t' << "" << '\n';
            count++;
            flag_buffer_complete = 1;
        }

        if(flag_buffer_complete)
        {

        }


    }


    // Close the file
    outfile.close();

    
}