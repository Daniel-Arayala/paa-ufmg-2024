#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <ios>
#include <queue>

# define YES "yes"
# define NO "no"

using namespace std;

class TranslationTree {
    private:
        unordered_map<char, unordered_set<char>> ttree;

        bool isLetterVisited(const char& letter, unordered_set<char>& visitedLetters){
            return visitedLetters.find(letter) != visitedLetters.end();
        }

        void addChildrenToQueue(
            queue<char>& q, 
            unordered_set<char>& visitedLetters, 
            char parentLetter) {
            
            for (const char child : ttree[parentLetter])
                if (!isLetterVisited(child, visitedLetters))
                    q.push(child);
        }

        bool letterExistsInTree(char letter) const{
            return ttree.find(letter) != ttree.end();
        }

    public:
        TranslationTree() : ttree({}) {}

        void addTranslation(char srcLetter, char destLetter) {
            ttree[srcLetter].insert(destLetter);
        }

        // Implementation of DFS
        bool isTranslationDFS(const char& srcLetter, const char& destLetter, int counter = 0) {
            if ((srcLetter == destLetter) || (ttree[srcLetter].find(destLetter) != ttree[srcLetter].end())) { // Quick search to find the destLetter in the children
                return true;
            }
            else if (
                (!letterExistsInTree(srcLetter)) 
                 || (counter > ttree.size())) { // To avoid infinite loops in case of cycles
                return false;
            }
            else             
            {
                for (const char nextLetter : ttree[srcLetter]) {
                    if(isTranslationDFS(nextLetter, destLetter, counter + 1))
                        return true;
                    
                }

                return false;
            }
        }
        

        // Implementation of BFS
        bool isTranslationBFS(const char& srcLetter, const char& destLetter) {
            if (srcLetter == destLetter) {
                return true;
            }
            else if (!letterExistsInTree(srcLetter)) {
                return false;
            }
            else if ((ttree[srcLetter].find(destLetter) != ttree[srcLetter].end())) { // Quick search to find the destLetter in the children
                return true;
            }
            else             
            {
                queue<char> toVisit;
                unordered_set<char> visitedLetters;
                // Initializes queue
                toVisit.push(srcLetter);

                while (toVisit.size() > 0) {
                    char currentLetter = toVisit.front();
                    toVisit.pop();
                    visitedLetters.insert(currentLetter);

                    if (currentLetter == destLetter)
                        return true;
                    else
                        addChildrenToQueue(toVisit, visitedLetters, currentLetter);
                }

                return false;
            }
        }
        
};

void createTranslations(
    vector<pair<char, char>> translations,
    unique_ptr<TranslationTree>& translationTree){

    for(int i = 0; i < translations.size(); i++){
        translationTree->addTranslation(
            translations[i].first, translations[i].second);
    }
}

void matchWordPairs(
    vector<pair<string, string>>& wordPairs,
    unique_ptr<TranslationTree>& translationTree,
    vector<string>& wordPairMatches) {
    
    for (int i = 0; i < wordPairs.size(); i++){
        string 
            baseWord = wordPairs[i].first, 
            translatedWord = wordPairs[i].second;
        
        bool isTranslation = true;

        if (baseWord.length() == translatedWord.length()) {
            for (unsigned int j = 0; j < baseWord.length(); j++) {
                if (!translationTree->isTranslationBFS(baseWord[j], translatedWord[j])) {
                    isTranslation = false;
                    break;
                }
            }
        }
        else {
            isTranslation = false;
        }
        
        wordPairMatches.push_back(isTranslation ? YES : NO);  
    } 
}

void readInput(
    vector<pair<char, char>>& translations,
    vector<pair<string, string>>& wordPairs) {
    int numTranslations, numWordPairs;
    // Reads the total number of translations and word pairs
    cin >> numTranslations >> numWordPairs;
    // Reading translations
    for(int i = 0; i < numTranslations; i++){
        char srcLetter, destLetter;
        cin >> srcLetter >> destLetter;
        translations.push_back({srcLetter, destLetter});
    }
    // Reading word pairs
    for(int i = 0; i < numWordPairs; i++){
        string baseWord, translatedWord;
        cin >> baseWord >> translatedWord;
        wordPairs.push_back({baseWord, translatedWord});
    }

}

void printOutput(const vector<string>& wordPairMatches) {
    for (const string& wordPairMatch : wordPairMatches) {
        cout << wordPairMatch << endl;
    }
}

int main() {
    // Disable synchronization between C and C++
    ios_base::sync_with_stdio(false);
    // Stores Input
    vector<pair<char, char>> translations;
    vector<pair<string, string>> wordPairs;
    // Stores the output
    vector<string> wordPairMatches;

    // Translation tree to hold the translations
    unique_ptr<TranslationTree> translationTree(new TranslationTree());
    
    readInput(translations, wordPairs);

    createTranslations(translations, translationTree);

    matchWordPairs(wordPairs, translationTree, wordPairMatches);

    printOutput(wordPairMatches);

    return 0;     
}

