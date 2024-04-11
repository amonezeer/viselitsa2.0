#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <algorithm>

using namespace std;
using namespace chrono;

const vector<string> HANGMAN = {
    R"(
     ------
     |    |
     |
     |
     |
     |
     |
    ----------
    )",
    R"(
     ------
     |    |
     |    O
     |
     |
     |
     |
    ----------
    )",
    R"(
     ------
     |    |
     |    O
     |    |
     | 
     |   
     |    
    ----------
    )",
    R"(
     ------
     |    |
     |    O
     |   /|
     |   
     |   
     |   
    ----------
    )",
    R"(
     ------
     |    |
     |    O
     |   /|\
     |   
     |   
     |     
    ----------
    )",
    R"(
     ------
     |    |
     |    O
     |   /|\
     |   /
     |   
     |    
    ----------
    )",
    R"(
     ------
     |    |
     |    O
     |   /|\
     |   / \
     |   
     |   
    ----------
    )"
};

string decryptCaesar(const string& encrypted, int shift) {
    string decrypted = encrypted;
    for (char& c : decrypted) {
        if (isalpha(c)) {
            char base = isupper(c) ? 'A' : 'a';
            c = (c - base - shift + 26) % 26 + base;
        }
    }
    return decrypted;
}

vector<string> readEncryptedWordsFromFile(const string& filename) {
    ifstream file(filename);
    vector<string> encryptedWords;
    string word;

    if (file.is_open()) {
        while (getline(file, word)) {
            encryptedWords.push_back(word);
        }
        file.close();
    }
    else {
        cerr << "Невозможно открыть файл: " << filename << endl;
        exit(1); 
    }

    return encryptedWords;
}

string selectRandomWord(const vector<string>& encryptedWords) {
    srand(time(0));
    int index = rand() % encryptedWords.size();
    return decryptCaesar(encryptedWords[index], 3); 
}

bool contains(const string& word, char guess) {
    return word.find(guess) != string::npos;
}

int main() {
    system("chcp 1251");

    string filename = "words.txt";
    vector<string> encryptedWords = readEncryptedWordsFromFile(filename);
    string word = selectRandomWord(encryptedWords);
    string so_far(word.size(), '_');
    vector<char> used;
    int wrong = 0;
    int max_wrong = HANGMAN.size() - 1;

    auto start = high_resolution_clock::now();

    cout << "Добро пожаловать в игру Виселица!\n";
    cout << "Попробуйте угадать слово:\n";

    while (wrong < max_wrong && so_far != word) {
        cout << HANGMAN[wrong] << endl;
        cout << "\nВы использовали следующие буквы:\n";
        for (char letter : used) {
            cout << letter << " ";
        }
        cout << "\n\nНа данный момент слово выглядит так:\n" << so_far << endl;

        char guess;
        cout << "\n\nВведите свою догадку: ";
        cin >> guess;
        cout << "<--------------------------------------->" << endl;

        while (find(used.begin(), used.end(), guess) != used.end()) {
            cout << "Вы уже вводили букву " << guess << ". Пожалуйста, введите другую букву: ";
            cin >> guess;
        }

        used.push_back(guess);

        if (contains(word, guess)) {
            cout << "\nДа, буква \"" << guess << "\" есть в слове!\n";
            for (int i = 0; i < word.size(); ++i) {
                if (word[i] == guess) {
                    so_far[i] = guess;
                }
            }
        }
        else {
            cout << "\nК сожалению, буквы \"" << guess << "\" нет в слове.\n";
            ++wrong;
        }
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(stop - start);

    if (wrong == max_wrong) {
        cout << HANGMAN[wrong] << endl;
        cout << "\nВас повесили!\n";
    }
    else {
        cout << "\nВы угадали слово!\n";
    }
    cout << "<------------------------------------->" << endl;
    cout << "\nЗагаданное слово было: \"" << word << "\"\n";
    cout << "Количество попыток: " << used.size() << endl;
    cout << "Время выполнения: " << duration.count() << " секунд." << endl;
    return 0;
}