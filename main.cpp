// Project 2
// Naomi Shah - nns220002

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <algorithm>
// #include <memory> // for smart pointers

using namespace std;

char *readCorrectAnswers(string filename, int &testSize) {
    // Open file
    ifstream infile(filename);
    if (!infile) {
        cerr << "Error opening file" << endl;
        return nullptr;
    }
    
    // Find number of values in file for array size
    char x;
    while (infile >> x) {
        testSize++;
    }
    
    // Display error if file is empty
    if (testSize == 0) {
        cerr << "File is empty!" << endl;
        return nullptr;
    }
    
    // Reset pointer and flags
    infile.clear(); // make bits good
    infile.seekg(0, ios::beg); // move to beginning of file to read
    
    // Read answers into array
    char *answers = new char[testSize]; // dynamically allocate array
    char *ptr = answers;
    while (!infile.eof()) {
        infile >> *ptr;
        ptr++;
    }
    // Close file
    infile.close();
    
    // Return char array
    return answers;
}

string *readIDs(string filename, int &numContestants) {
    // Open file
    ifstream infile(filename);
    if (!infile) {
        cerr << "Error opening file" << endl; // display error
        EXIT_FAILURE;
    }
    
    // Find number of contestants
    string line;
    while (getline(infile, line)) {
        numContestants++;
    }
    
    // Display error if file is empty
    if (numContestants == 0) {
        cerr << "File is empty!" << endl; // display error
        return nullptr;
    }
    
    // Reset pointer and flags
    infile.clear(); // make bits good
    infile.seekg(0, ios::beg); // move to beginning of file to read
    
    // Allocate memory for array
    string *contestantIDs = new string[numContestants];
    
    // Read IDs into array by parsing each line
    string *ptr = contestantIDs;
    for (int i = 0; i < numContestants; i++) {
        getline(infile, line); // read line
        auto space = line.find(' '); // find first space
        *ptr = line.substr(0, space); // store ID
        ptr++;
    }
    // Close file
    infile.close();
    
    return contestantIDs;
}

string *readConAnswers(string filename, int &numContestants) {
    // Open file
    ifstream infile(filename);
    if (!infile) {
        cerr << "Error opening file" << endl; // display error
        return nullptr;
    }
    
    // Allocate memory for array
    string *contestantAnswers = new string[numContestants];
    
    // Read answers into array by parsing each line
    string line;
    string *ptr = contestantAnswers;
    for (int i = 0; i < numContestants; i++) {
        getline(infile, line); // read line
        auto space = line.find(' '); // find first space
        
        // Parse student answer string
        *ptr = line.substr(space + 1, line.length() - space);
        (*ptr).erase(remove_if((*ptr).begin(), (*ptr).end(), ::isspace), (*ptr).end()); // removes spaces from string of answers
        ptr++;
    }
    // Close file
    infile.close();
    
    return contestantAnswers;
}

double *calculateScores(char *correctAnswers, string *contestantAnswers, int testSize, int numContestants) {
    // Allocate memory for array
    double *scores = new double[numContestants];
    double *scoresPtr = scores;
    
    // For every contestant, check their score
    string *sptr = contestantAnswers;
    for (int i = 0; i < numContestants; i++) {
        int correctCount = 0;
        string current = *sptr;
        
        // For every question, check if the answers matches the key
        char *cptr = correctAnswers;
        char *currentPtr = &current[0]; // initialize pointer to beginning of string
        for (int j = 0; j < testSize; j++) {
            if (*currentPtr == *cptr) {
                correctCount++; // increment correct answer count
            }
            currentPtr++;
            cptr++;
        }
        // Calculate and store score
        *scoresPtr = (static_cast<double>(correctCount) / testSize) * 100;
        scoresPtr++; // move to next score
        sptr++;
    }
    return scores;
}

void displayReport(string *conIDs, double *scores, string *conAnswers, char *correctAnswers, int numContestants, int testSize) {
    
    string *conIDsPtr = conIDs;
    double *scoresPtr = scores;
    string *conAnswersPtr = conAnswers;
    char *correctAnswersPtr = correctAnswers;
    
    // For every contestant, display their score and incorrect questions
    for (int i = 0; i < numContestants; i++) {
        cout << endl << *conIDsPtr << " - " << fixed << setprecision(2) << *scoresPtr << endl; // display score
        if (*scoresPtr != 100) {
            string current = *conAnswersPtr;
            
            // Display every question number they got
            char *currentPtr = &current[0]; // initialize pointer to beginning of string
            for (int j = 0; j < testSize; j++) {
                if (*currentPtr != *(correctAnswersPtr + j)) {
                    cout << (j+1); // display question numbers
                    if (j != testSize - 1) {
                        cout << " ";
                    }
                }
                currentPtr++; // move to next character in string
            }
            
            cout << endl; // display endl after question numbers
            
            // Display contestant's wrong answers
            char *currentPtr2 = &current[0]; // initialize pointer to beginning
            for (int j = 0; j < testSize; j++) {
                if (*currentPtr2 != *(correctAnswersPtr + j)) {
                    // If two digits
                    if (j > 8) {
                        cout << " ";
                    }
                    cout << *currentPtr2; // display wrong answer
                    if (j != testSize - 1) {
                        cout << " ";
                    }
                }
                currentPtr2++; // move to next character in string
            }
            
            cout << endl; // display endl after wrong answers
            
            // Display correct answers
            char *currentPtr3 = &current[0]; // initialize pointer to beginning
            for (int j = 0; j < testSize; j++) {
                if (*currentPtr3 != *(correctAnswersPtr + j)) {
                    // If two digits
                    if (j > 8) {
                        cout << " ";
                    }
                    cout << *(correctAnswersPtr + j); // display correct answer
                    if (j != testSize - 1) {
                        cout << " ";
                    }
                }
                currentPtr3++;
            }
        }
        
        cout << endl; // display endl after correct answers
        
        // Move pointers to next contestant
        conIDsPtr++;
        scoresPtr++;
        conAnswersPtr++;
        
    }
}

void findMean(double *scores, int numContestants) {
    // Calculate total of scores
    double sum = 0;
    double *ptr = scores;
    for (int i = 0; i < numContestants; i++) {
        sum += *ptr;
        ptr++;
    }
    
    double mean = (sum / numContestants);
    // Display mean
    cout << endl << "Mean: " << mean << endl;
}

void findMedian(double *scores, int numContestants) {
    double median = 0;
    
    // Sort scores array
    double *ptr1 = scores;
    for (int i = 0; i < numContestants - 1; i++) {
        double *ptr2 = scores;
        for (int j = 0; j < numContestants - i - 1; j++) {
            if (*(ptr2 + 1) < *ptr2) {
                // Swap values using temporary variable
                double temp = *ptr2;
                *ptr2 = *(ptr2 + 1);
                *(ptr2 + 1) = temp;
            }
            ptr2++;
        }
        ptr1++;
    }

    // Calculate median
    if (numContestants == 0) {
        median = 0;
    } else if (numContestants % 2 == 0) { // If number of contestants is even
        double *midPtr1 = scores + (numContestants / 2) - 1;
        double *midPtr2 = scores + (numContestants / 2);
        median = (*midPtr1 + *midPtr2) / 2;
    } else { // If number of contestants is odd
        double *midPtr = scores + (numContestants / 2);
        median = *midPtr;
    }
    
    // Display median
    cout << "Median: " << median << endl;
}

void findModes(double *scores, int numContestants) {
    // Count frequencies of each score
    int maxFrequency = 0;
    int *maxFreqPtr = &maxFrequency;
    
    // Calculate frequencies for each score
    for (int i = 0; i < numContestants; i++) {
        int frequency = 1; // initialize frequency for current score
        int *frequencyPtr = &frequency;
        double *scoreI = scores + i;
        
        for (int j = i + 1; j < numContestants; j++) {
            double *scoreJ = scores + j;
            if (*scoreI == *scoreJ) {
                (*frequencyPtr)++;
            }
        }
        if (*frequencyPtr > *maxFreqPtr) {
            *maxFreqPtr = *frequencyPtr;
        }
    }
    
    cout << "Mode:";
    
    bool firstMode = true; // flag to determine if it's the first mode
    
    for (int i = 0; i < numContestants; ++i) {
        int frequency = 1;
        int *frequencyPtr = &frequency;
        double *scoreI = scores + i;
        
        for (int j = i + 1; j < numContestants; ++j) {
            double *scoreJ = scores + j;
            if (*scoreI == *scoreJ) {
                (*frequencyPtr)++;
            }
        }
        if (*frequencyPtr == *maxFreqPtr) {
            
            if (!firstMode) {
                cout << ", " << *scoreI; // output mode
            } else {
                cout << " " << *scoreI; // output mode
            }
            
            firstMode = false;
            
            // skip duplicates
            while (i + 1 < numContestants && *(scores + i) == *(scores + i + 1)) {
                double *scoreNext = scores + i + 1;
                if (*scoreI == *scoreNext) {
                    i++;
                } else {
                    break;
                }
            }
        }
    }
    cout << endl;
}

void mostMissed(char *correctAnswers, string *conAnswers, int numContestants, int testSize) {
    // Display header for most missed
    cout << endl << "MOST MISSED QUESTIONS" << endl;
    
    // Initialize dynamically allocated array to store percentage wrong
    double *percentWrong = new double[testSize];

    // Loop over each question of the test
    for (int i = 0; i < testSize; i++) {
        int wrongCount = 0;
        double *percentPtr = percentWrong + i;
    
        // Loop through contestant answers
        string *contestAnswersPtr = conAnswers;
        for (int j = 0; j < numContestants; j++) {
            string current = *contestAnswersPtr;
            char *currentPtr = &current[0]; // initialize pointer to beginning of string
            
            if (*(currentPtr + i) != *(correctAnswers + i)) {
                wrongCount++;
            }
            // Store percentage in array
            *percentPtr = ((static_cast<double>(wrongCount))/ numContestants * 100);
        
            contestAnswersPtr++;
        }
    }
    
    // Loop to display question number and % if over 60%
    for (int i = 0; i < testSize; i++) {
        if (*(percentWrong + i) >= 60) {
            cout << i+1 << "\t" << *(percentWrong + i) << "%" << endl;
        }
    }
}

int main() {
    string answerKeyFile, conResponseFile;
    int testSize = 0, numContestants = 0;
    
    // Get answer key filename from user
    cout << "Enter filename for answer key: ";
    cin >> answerKeyFile;
    
    // Get contestant response filename from user
    cout << "Enter filename for answer key: ";
    cin >> conResponseFile;
    
    // Call function to read correct answers
    char *correctAnswers = readCorrectAnswers(answerKeyFile, testSize);
    
    // Call function to read IDs
    string *conIDs = readIDs(conResponseFile, numContestants);
    
    // Call function to read contestants answers
    string *conAnswers = readConAnswers(conResponseFile, numContestants);
    
    // Call function to calculate score
    double *scores = calculateScores(correctAnswers, conAnswers, testSize, numContestants);
    
    // Call function to display report
    displayReport(conIDs, scores, conAnswers, correctAnswers, numContestants, testSize);
    
    // Call function to find mean
    findMean(scores, numContestants);
    
    // Call function to find median
    findMedian(scores, numContestants);
    
    // Call function to find mode(s)
    findModes(scores, numContestants);
    
    // Call function to display most missed questions
    mostMissed(correctAnswers, conAnswers, numContestants, testSize);
    
    return 0;
}

