#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <algorithm>

using namespace std;

vector <string> readWordsfromfile(const string& filename){
    vector<string> words;
    ifstream file(filename);
    
    if(file.is_open()){
        string word;
        while(getline(file,word)){
            words.push_back(word);
        }
        file.close();
    }
    else{
        cerr<<"Error:unable to open file"<<endl;
    }
    return words;
}

string getRandomword(const vector<string>& words){ // randomly pick a word from the input text
    if(words.empty()){
        cerr<<"Error : No words in the vector."<<endl;
        return "";
    }
    srand(static_cast<unsigned int>(time(nullptr)));
    int randomIndex = rand()% words.size();
    return words[randomIndex];
}
void displayWord(const string& secretWord, const vector<char>& guessedLetters){
    for (char letter : secretWord){
        if(find(guessedLetters.begin(),guessedLetters.end(),letter) != guessedLetters.end()){
            cout<<letter<<' ';
        }
        else{
            cout<<"_";
        }
    }
    cout <<endl;
}
char provideHint(const string& secretWord, const vector<char>& guessedLetters){
    for (char letter: secretWord){
        if (find(guessedLetters.begin(),guessedLetters.end(),letter)==guessedLetters.end()){
            return letter;
        }
    }
    return '0';
}

void drawHangman(int incorrectGuesses) {
    cout << "Incorrect Guesses: " << incorrectGuesses << endl;

    if (incorrectGuesses >= 1) {
        cout << "  ________" << endl;
        cout << "  |      |" << endl;
        cout << "  |      O" << endl;
        if (incorrectGuesses == 1) {
            cout << "  |" << endl;
            cout << "  |" << endl;
        }
    }
    if (incorrectGuesses >= 2) {
        if (incorrectGuesses == 2) {
            cout << "  |      |" << endl;
        } else if (incorrectGuesses == 3) {
            cout << "  |     /|" << endl;
        } else {
            cout << "  |     /|\\" << endl;
        }
    }
    if (incorrectGuesses >= 4) {
        if (incorrectGuesses == 4) {
            cout << "  |     /" << endl;
        } else {
            cout << "  |     / \\" << endl;
        }
    }
    cout << " _|_" << endl << endl;
}

int main() {
    const string filename = "words.txt";
   // const string filename = "/Users/eunbihan/Desktop/IE405/IE_final/IE_final/words.txt";

    const int maxAttempts = 6;
    
    vector<string> words = readWordsfromfile(filename);
    
    if (words.empty()) {
        cerr << "Exiting program due to error." << endl;
        return 1;
    }
    
    string secretWord = getRandomword(words);
    vector<char> guessedLetters;
    int incorrectGuesses = 0;
    
    cout << "Welcome to Hangman" << endl;
    
    while (incorrectGuesses < maxAttempts) {
        cout << endl;
        drawHangman(incorrectGuesses);
        displayWord(secretWord, guessedLetters);
        cout<<endl<<endl;

        
        cout << "Enter a letter/guess the entire word, or enter hint : ";
        string guess;
        cin >> guess;
        
        transform(guess.begin(), guess.end(), guess.begin(), ::tolower); // transform user input to lowercase for case-insensitivity
        
        if (guess == "hint") { // check user input for various scenarios
            char hint = provideHint(secretWord, guessedLetters);
            if (hint != '0') {
                cout << "Hint: " << hint << endl;
            }
        } else if (guess.size() == 1 && isalpha(guess[0])) { // process single letter guesses
            char letter = guess[0];
            if (find(guessedLetters.begin(), guessedLetters.end(), letter) != guessedLetters.end()) {
                cout << "You already guessed that letter. Try again." << endl;
            } else { // process a new letter guess
                guessedLetters.push_back(letter);
                if (secretWord.find(letter) == string::npos) {
                    // increment incorrect guess count if the letter is not in the word
                    cout << "Incorrect guess." << endl;
                    incorrectGuesses++;
                }
            }
        } else if (guess.size() == secretWord.size() && guess != secretWord) {
            // inform the user of an incorrect word guess
            cout << "Incorrect. Try again." << endl;
            incorrectGuesses++;
        } else if (guess == secretWord) {

            cout << "Congratulations! You guessed the word: " << secretWord << endl;
            break;
        } else {
            cout << "Please enter a valid letter or 'hint'." << endl;
        }
        //check if all letters of the secret word have been guessed
        if (all_of(secretWord.begin(), secretWord.end(), [&guessedLetters](char letter) {
            return find(guessedLetters.begin(), guessedLetters.end(), letter) != guessedLetters.end();
        })) {
            cout << "Congratulations! You guessed the word: " << secretWord << endl;
            break;
        }
    }
    
    if (incorrectGuesses == maxAttempts) {
        cout << "Sorry, you ran out of attempts. The word was: " << secretWord << endl;
    }
    
    return 0;
}
