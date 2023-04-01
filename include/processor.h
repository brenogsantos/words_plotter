#pragma once

#ifndef PROCESSOR_H
#define PROCESSOR_H


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <queue>
#include <unordered_set>
#include <algorithm> // for std::transform
#include <cctype>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <functional>
#include <memory>
#include <algorithm>






namespace INVALID_WORDS
{
   // static std::vector<std::string> invalid;
    static std::unordered_set<std::string> invalid;
    void logInvalidWords(std::string);
    void printInvalidWords();
    bool checkInvalidWord(std::string);


}


class Words
{
private:
    /* data */
    std::unordered_map<std::string, int> word_count; //per file
    std::priority_queue<std::pair<int, std::string>> top_words; //temporary data, criar um banco pra eles
    std::unordered_map<std::string, std::vector<std::pair<int,std::string>>> words_totalMap;

    
public:
    Words(/* args */);
    ~Words();

    void inputNewFile();
    void inputNewFileByIndex(const int);
    void inputNewFileFromTxt();
    void readTopWordsFromFile(const int, const int);
    inline int getLastVecPosition() {return filenames.size() - 1;};
    void logSeekWordsQty(const int);
    void printWordsMap();
    void saveToTxtWordsMap();

    void plotData();

    std::vector<std::string> filenames;
    std::vector<std::string> filenamesFromTxt;
    std::vector<std::string> seek_words;


}; 

std::string cleanString(std::string&);
void loopInputFiles(Words&);
void loopInputFilesByTxt(Words&);
void loopInputSeekWords(Words&);



#endif