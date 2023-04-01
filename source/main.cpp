#include "../include/processor.h"



int main(){
 


    Words words;

    

    INVALID_WORDS::logInvalidWords("invalidWords.txt");

    loopInputSeekWords(words);
    loopInputFiles(words);

    words.printWordsMap();
    words.saveToTxtWordsMap();





/*PLOT*/
    std::thread plotThread(std::bind(&Words::plotData, &words));
    plotThread.detach(); // detach the thread

    // Wait for the user to press a key
    std::cout << "Press any key to exit" << std::endl;
    std::cin.ignore();


     plotThread.join();


    

    return 0;
}
