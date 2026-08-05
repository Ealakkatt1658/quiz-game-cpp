#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

// Structure to store each question
struct Question {
  string category;
  int qid;
  string context;
  string question;
  string ans0;
  string ans1;
  string ans2;
  int label;
  int ai;

  // Game state tracking
  int user_ans;
  int attempts;
  Question* next;

  Question() {
    user_ans = -1;
    attempts = 0;
    next = nullptr;
  }
};

// Global dataset to hold parsed CSV data
vector<Question> dataset;

// Adds quotes around strings that contain commas to prevent CSV format breaking
string addQuotes(string s) {
  if (s.find(',') != string::npos) {
    return "\"" + s + "\"";
  }
  return s;
}

// Loads the dataset from version_1.csv into the global "dataset" vector
void loadDataset() {
  ifstream file("version_1.csv");
  if (!file.is_open()) {
    cout << "Warning: could not open version_1.csv - starting with an empty question bank.\n\n";
    return;
  }

  string line;
  getline(file, line); // Skip header line

  while (getline(file, line)) {
    if (line.empty() || line[0] == '\r') continue;

    vector<string> cols;
    string temp = "";
    bool inQuotes = false;

    // Parse CSV ignoring commas inside quotes
    for (char c : line) {
      if (c == '"') {
        inQuotes = !inQuotes;
      } else if (c == ',' && !inQuotes) {
        cols.push_back(temp);
        temp = "";
      } else {
        temp += c;
      }
    }
    cols.push_back(temp);

    // Remove carriage return if present
    if (!cols.empty() && !cols.back().empty() && cols.back().back() == '\r') {
      cols.back().pop_back();
    }

    if (cols.size() >= 9) {
      try {
        Question q;
        q.category = cols[0];
        q.qid = stoi(cols[1]);
        q.context = cols[2];
        q.question = cols[3];
        q.ans0 = cols[4];
        q.ans1 = cols[5];
        q.ans2 = cols[6];
        q.label = stoi(cols[7]);
        q.ai = stoi(cols[8]);
        dataset.push_back(q);
      } catch (...) {
        // Skip malformed rows instead of crashing
        continue;
      }
    }
  }
  file.close();
}

// Linked list class for the quiz
class Quiz {
public:
  Question* head;
  Question* tail;

  Quiz() {
    head = nullptr;
    tail = nullptr;
  }

  void clear() {
    Question* curr = head;
    while (curr != nullptr) {
      Question* temp = curr;
      curr = curr->next;
      delete temp;
    }
    head = nullptr;
    tail = nullptr;
  }

  void addQuestion(Question q) {
    Question* newQ = new Question;
    *newQ = q; // Copy the data
    newQ->next = nullptr;
    newQ->attempts = 0;
    newQ->user_ans = -1;

    if (head == nullptr) {
      head = newQ;
      tail = newQ;
    } else {
      tail->next = newQ;
      tail = newQ;
    }
  }
};

Quiz myQuiz;

bool isValidCategory(string cat) {
  for (size_t i = 0; i < dataset.size(); i++) {
    if (dataset[i].category == cat) return true;
  }
  return false;
}

bool isValidQID(string cat, int qid) {
  for (size_t i = 0; i < dataset.size(); i++) {
    if (dataset[i].category == cat && dataset[i].qid == qid) return true;
  }
  return false;
}

void buildQuiz() {
  myQuiz.clear();
  int num_cats;

  cout << "How many categories to include? ";
  if (!(cin >> num_cats) || num_cats <= 0) {
    cout << "Please enter a positive number.\n\n";
    cin.clear();
    cin.ignore(10000, '\n');
    return;
  }

  for (int i = 0; i < num_cats; i++) {
    string cat;
    while (true) {
      cout << "Enter category: ";
      cin >> cat;
      if (isValidCategory(cat)) break;
      cout << "Invalid input\n";
    }

    int num_q = 0;
    while (true) {
      cout << "How many questions from " << cat << "? ";
      if (cin >> num_q && num_q > 0) break;
      cout << "Please enter a positive number.\n";
      cin.clear();
      cin.ignore(10000, '\n');
    }

    while (true) {
      cout << "Enter " << num_q << " question IDs separated by space: ";
      vector<int> qids(num_q);
      bool valid = true;

      for (int j = 0; j < num_q; j++) {
        if (!(cin >> qids[j])) {
          valid = false;
          cin.clear();
          cin.ignore(10000, '\n');
          break;
        }
        if (!isValidQID(cat, qids[j])) {
          valid = false;
        }
      }

      if (valid) {
        for (int j = 0; j < num_q; j++) {
          for (size_t k = 0; k < dataset.size(); k++) {
            if (dataset[k].category == cat && dataset[k].qid == qids[j]) {
              myQuiz.addQuestion(dataset[k]);
              break;
            }
          }
        }
        break;
      } else {
        cout << "Invalid input\n";
        cin.clear();
        cin.ignore(10000, '\n');
      }
    }
  }
  cout << "Quiz built successfully!\n\n";
}

void playQuiz() {
  if (myQuiz.head == nullptr) {
    cout << "No questions in the quiz!\n\n";
    return;
  }

  int mode;
  while (true) {
    cout << "Select game mode:\n";
    cout << "1. Human vs Human\n";
    cout << "2. Human vs AI\n";
    if ((cin >> mode) && (mode == 1 || mode == 2)) break;

    cout << "Invalid input. Please enter 1 or 2.\n";
    cin.clear();
    cin.ignore(10000, '\n');
  }

  Question* curr = myQuiz.head;
  Question* prev = nullptr;

  while (curr != nullptr) {
    cout << "Category: " << curr->category << "\n";
    cout << "Context: " << curr->context << "\n";
    cout << "Question: " << curr->question << "\n";
    cout << "Options:\n";
    cout << "ans0: " << curr->ans0 << "\n";
    cout << "ans1: " << curr->ans1 << "\n";
    cout << "ans2: " << curr->ans2 << "\n";
    cout << "Your answer: ";

    int ans;
    if (!(cin >> ans)) {
      cout << "Invalid input, please enter 0, 1, or 2.\n\n";
      cin.clear();
      cin.ignore(10000, '\n');
      continue;
    }

    curr->attempts++;
    curr->user_ans = ans;

    int correct_ans = (mode == 1) ? curr->label : curr->ai;

    if (ans == correct_ans) {
      cout << "Your guess matches our system answer!\n\n";
      prev = curr;
      curr = curr->next;
    } else {
      cout << "Please try again later\n\n";

      // Move question to the end of its category block
      Question* tail_cat = curr;
      while (tail_cat->next != nullptr && tail_cat->next->category == curr->category) {
        tail_cat = tail_cat->next;
      }

      if (curr != tail_cat) {
        Question* next_q = curr->next;

        if (prev == nullptr) {
          myQuiz.head = next_q;
        } else {
          prev->next = next_q;
        }

        curr->next = tail_cat->next;
        tail_cat->next = curr;

        if (myQuiz.tail == tail_cat) {
          myQuiz.tail = curr;
        }
        curr = next_q;
      }
    }
  }
  cout << "Congratulations! You have completed the quiz!\n\n";
}

void viewReport() {
  if (myQuiz.head == nullptr) {
    cout << "No quiz has been played yet.\n\n";
    return;
  }

  int mode;
  while (true) {
    cout << "Select game mode for report:\n";
    cout << "1. Human vs Human\n";
    cout << "2. Human vs AI\n";
    if ((cin >> mode) && (mode == 1 || mode == 2)) break;

    cout << "Invalid input. Please enter 1 or 2.\n";
    cin.clear();
    cin.ignore(10000, '\n');
  }

  Question* curr = myQuiz.head;
  while (curr != nullptr) {
    cout << "Category: " << curr->category << "\n";
    cout << "QID: " << curr->qid << "\n";
    cout << "Context: " << curr->context << "\n";
    cout << "Question: " << curr->question << "\n";
    cout << "Options: \n";
    cout << "ans0: " << curr->ans0 << "\n";
    cout << "ans1: " << curr->ans1 << "\n";
    cout << "ans2: " << curr->ans2 << "\n";
    cout << "System Answer: " << ((mode == 1) ? curr->label : curr->ai) << "\n";

    if (curr->attempts == 0) {
      cout << "User Answer: \n";
    } else {
      cout << "User Answer: " << curr->user_ans << "\n";
    }
    cout << "Attempts: " << curr->attempts << "\n\n";

    curr = curr->next;
  }

  // Write to CSV report
  string filename = "game" + to_string(mode) + "_netID.csv";
  ofstream out(filename);
  if (out.is_open()) {
    out << "category,question_index,context,question,ans0,ans1,ans2,label,AI,user,attempts\n";

    curr = myQuiz.head;
    while (curr != nullptr) {
      out << curr->category << ","
          << curr->qid << ","
          << addQuotes(curr->context) << ","
          << addQuotes(curr->question) << ","
          << addQuotes(curr->ans0) << ","
          << addQuotes(curr->ans1) << ","
          << addQuotes(curr->ans2) << ","
          << curr->label << ","
          << curr->ai << ",";

      if (curr->attempts > 0) {
        out << curr->user_ans;
      }

      out << "," << curr->attempts << "\n";
      curr = curr->next;
    }
    out.close();
    cout << "Report saved successfully to " << filename << "\n\n";
  }
}

int main() {
  loadDataset();

  int choice;
  while (true) {
    cout << "===== Quiz Game =====\n";
    cout << "1. Build New Quiz\n";
    cout << "2. Play Quiz\n";
    cout << "3. View Report\n";
    cout << "4. Exit\n";
    cout << "Enter choice: ";

    if (!(cin >> choice)) break;

    if (choice == 1) {
      buildQuiz();
    } else if (choice == 2) {
      playQuiz();
    } else if (choice == 3) {
      viewReport();
    } else if (choice == 4) {
      break;
    } else {
      cout << "Invalid choice. Please enter a number from 1 to 4.\n\n";
    }
  }

  return 0;
}
