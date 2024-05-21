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
    const int& numTranslations, 
    unique_ptr<TranslationTree>& translationTree){

    for(int i = 0; i < numTranslations; i++){
        char srcLetter, destLetter;
        cin >> srcLetter >> destLetter;
        translationTree->addTranslation(srcLetter, destLetter);
    }
}

void matchWordPairs(
    const int& numWordPairs,
    unique_ptr<TranslationTree>& translationTree){
    
    for (int i = 0; i < numWordPairs; i++){
        string baseWord, translatedWord;
        bool isTranslation = true;
        cin >> baseWord >> translatedWord;

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
        
        // Writes to standard output
        cout << (isTranslation ? YES : NO) << endl;
    } 
}



int main() {
    // Disable synchronization between C and C++
    ios_base::sync_with_stdio(false);
    // VARIABLES READ FROM INPUT
    int numTranslations = 0;
    int numWordPairs = 0;
    unique_ptr<TranslationTree> translationTree(new TranslationTree());
    
    // Reads the total number of translations and word pairs
    cin >> numTranslations >> numWordPairs;

    createTranslations(numTranslations, translationTree);

    matchWordPairs(numWordPairs, translationTree);

    return 0;     
}

