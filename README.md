# words_plotter


<div align="left">
    <img src="https://img.shields.io/badge/language-c++-green">
    <img src="https://img.shields.io/badge/cmd-gnuplot-darkblue">
</div>


## Table of Contents
1. [About the Project](#about-the-project)
2. [Plot](#plot)
3. [Bugs to Fix](#bugs-to-fix)
4. [Resources](#resources)

# About the Project

This is a project that analyzes a collection of text files based on the user's input. The user can specify certain keywords that they want to search for (there is also a file to define which words you want the program to ignore, "invalid_words"), and the program will return a graph showing the frequency of those terms in each file. Additionally, the user can request a list of the most frequently occurring terms in each file. The program uses the Gnuplot library to generate the graphs. Overall, my project is designed to help users better understand the content of their text files and identify patterns or trends in the data.

The gnuplot reads the output.txt file.

# Plot

<p align="center">
<img src="https://user-images.githubusercontent.com/61205851/229319952-fe2fed4f-ea71-4f68-809a-2324debacba2.png" width="640" height="480" /> 
</p>


# Bugs to Fix

- [x] ~~If the file does not contain the word, the "output.txt" is written in an unplanned way (the lines related to each word are out of order) and the Plot output is wrong~~
- [ ] The lines are in order, but if the last line (of the group related to one word) is a line which refers to a file that didn't have any repetitiveness of the word, the program crashes.


# Resources
