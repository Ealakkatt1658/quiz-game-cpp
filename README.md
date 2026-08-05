# Quiz Game (C++)

A console-based quiz game written in modern C++. Questions are loaded from a CSV file into a hand-rolled singly linked list, and the program supports building a custom quiz from any combination of categories and question IDs, playing it in either Human vs. Human or Human vs. AI scoring mode, and exporting a CSV report of the results.

## Features

- **CSV-driven question bank** – questions live in `version_1.csv` and are parsed at startup, including support for quoted fields that contain commas.
- **Custom quiz builder** – choose any number of categories and specific question IDs to assemble a quiz on the fly.
- **Two scoring modes** – Human vs. Human compares your answer against the `label` column; Human vs. AI compares against the `ai` column.
- **Adaptive retry logic** – missed questions are moved to the end of their category block and asked again later instead of being dropped.
- **Score reporting** – generates a `gameX_netID.csv` report listing every question, the system answer, your answer, and how many attempts it took.
- **Defensive input handling** – invalid menu choices, non-numeric answers, and malformed CSV rows are handled gracefully instead of crashing the program.

## Project structure

| File | Description |
| --- | --- |
| `main.cpp` | Program source: CSV parsing, the `Quiz` linked list, and the menu-driven game loop. |
| `version_1.csv` | Sample question bank covering Geography, Science, History, Sports, and Technology. |

## Building

Requires a C++17-compatible compiler such as `g++` or `clang++`.

```
g++ -std=c++17 -Wall -Wextra -o quiz main.cpp
```

This compiles cleanly with zero errors and zero warnings.

## Running

```
./quiz
```

Make sure `version_1.csv` is in the same directory as the executable. It needs a header row followed by data rows in this column order:

```
category,qid,context,question,ans0,ans1,ans2,label,ai
```

- `label` and `ai` are the index (0, 1, or 2) of the correct answer among `ans0`/`ans1`/`ans2`, for Human vs. Human and Human vs. AI mode respectively.
- Fields that themselves contain a comma should be wrapped in double quotes.

Once running, use the menu to build a new quiz, play it, view a score report, or exit.

## Sample session

```
===== Quiz Game =====
1. Build New Quiz
2. Play Quiz
3. View Report
4. Exit
Enter choice: 1
How many categories to include? 1
Enter category: Science
How many questions from Science? 2
Enter 2 question IDs separated by space: 1 2
Quiz built successfully!
```

## Notes

The bundled `version_1.csv` is a sample dataset meant to demonstrate the game end-to-end. Feel free to replace it with your own categories and questions — as long as the column order above is followed, the game will load it automatically.
