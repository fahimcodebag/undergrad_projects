#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

class WordFinder {
private:
    ifstream dictionary;
    string puzzle;
    vector<string> output;

public:
    WordFinder(const string& dictFileName) {
        dictionary.open(dictFileName);
        
        if (!dictionary) {
            cout << "Unable to open dictionary" << endl;
            system("pause");
            exit(1);
        }

        cout << "Dictionary File is Open" << endl;
    }

    void enterPuzzle() {
        cout << "Enter The Puzzle" << endl;
        cin >> puzzle;
    }

    void findWordsInPuzzle() {
        int position = 0;
        int n = 3;
        
        while (n <= puzzle.length()) {
            while (position <= puzzle.length() - n) {
                string test_word = puzzle.substr(position, n);
                bool found = false;
                string word;
                
                while (!dictionary.eof()) {
                    dictionary >> word;
                    if (word == test_word) {
                        output.push_back(word);
                        found = true;
                        break;
                    }
                }

                dictionary.clear();
                dictionary.seekg(0, ios::beg); // Go to the beginning of the file
                position++;
            }
            n++;
            position = 0;
        }
    }

    void displayFoundWords() {
        cout << "The found words are:" << endl;
        for (int j = 0; j < output.size(); j++) {
            cout << output[j];
            if (j < output.size() - 1)
                cout << " , ";
        }
        cout << endl;
    }

    void closeDictionary() {
        dictionary.close();
    }
};
int main() {
    
    string dictionaryFileName = "C:/Users/fahim/Downloads/dictionary.txt";
    
    WordFinder wordFinder(dictionaryFileName);
    
    wordFinder.enterPuzzle();
    
    wordFinder.findWordsInPuzzle();
    
    wordFinder.displayFoundWords();
    
    wordFinder.closeDictionary();

    return 0;
    
}
