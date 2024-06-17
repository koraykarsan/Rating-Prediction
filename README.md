# Rating Prediction
This project is part of the Data Structures and Algorithms course. The goal is to implement neighborhood-based collaborative filtering (NBCF) algorithms in C++ to predict movie ratings.

## Project Overview

Recommender systems are used to suggest items such as movies, books, or music based on user preferences. This project focuses on predicting movie ratings using NBCF algorithms, specifically user-based (UBCF) and item-based (IBCF) collaborative filtering. The main tasks include reading the input data, implementing the algorithms, making predictions, and evaluating the performance using RMSE (Root Mean Squared Error).

Requirements

Programming Language: C++
Libraries: Only the C++ Standard Library is allowed.
Team Formation: Up to 3 students per team. Individual work is also allowed but teamwork is encouraged.
Directory Structure

.
├── src
│ ├── main.cpp
│ ├── DataFrame.cpp
│ ├── DataFrame.h
├── README.md
├── report.pdf
└── video.mp4

Files

src/main.cpp: The main driver code.
src/DataFrame.cpp: Implementation of data structures and algorithms.
src/DataFrame.h: Header file for data structures and algorithms.
report.pdf: Detailed project report explaining the implemented algorithms and data structures.
video.mp4: Video recording explaining each team member's contribution.
Getting Started

Clone the repository:
git clone https://github.com/yourusername/rating-prediction.git
cd rating-prediction

Build the project:
g++ src/main.cpp src/DataFrame.cpp -o rating_prediction

Run the project:
./rating_prediction

