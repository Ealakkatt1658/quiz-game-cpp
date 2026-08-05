# quiz-game-cpp

A C++ console quiz game (originally "Project 6"). Questions are loaded from a CSV file into a custom linked-list data structure, and the program supports building a custom quiz, playing it in Human vs Human or Human vs AI mode, and viewing/exporting a report of the results.

## Features

- Loads quiz questions from a CSV file (`version_1.csv`) into memory, handling quoted fields that contain commas.
- Lets you build a quiz by choosing categories and specific question IDs.
- Play mode supports two scoring modes: Human vs Human (compare against the `label` column) and Human vs AI (compare against the `ai` column). Missed questions are recycled to the end of their category block so they come up again.
- Generates a CSV report (`gameX_netID.csv`) of every question, the system answer, your answer, and how many attempts it took.

## Building

Requires a C++17-compatible compiler (e.g. g++ or clang++).

```bash
g++ -std=c++17 -Wall -Wextra -o quiz main.cpp
```

This has been verified to compile cleanly (0 errors, only minor signed/unsigned comparison warnings) and run correctly.

## Running

```bash
./quiz
```

You'll need a `version_1.csv` file in the same directory with a header row followed by rows in this column order:

```
category,qid,context,question,ans0,ans1,ans2,label,ai
```

Once running, use the menu to build a new quiz, play it, view a report, or exit.
