#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <ios>

# define YES "yes"
# define NO "no"

using namespace std;


class TranslationTree {
    private:
        unordered_map<char, unordered_set<char>> ttree;
    public:
        TranslationTree() : ttree({}) {}
        void addTranslation(char srcLetter, char destLetter) {
            ttree[srcLetter].insert(destLetter);
        }

        bool letterExistsInTree(char letter) const{
            return ttree.find(letter) != ttree.end();
        }

        // Implementation of DFS
        bool isTranslation(const char& srcLetter, const char& destLetter, int counter = 0) {
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
                bool foundTranslation = false;
                for (const char nextLetter : ttree[srcLetter]) {
                    if(isTranslation(nextLetter, destLetter, counter + 1))
                        return true;
                    
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
            for (int j = 0; j < baseWord.length(); j++) {
                if (!translationTree->isTranslation(baseWord[j], translatedWord[j])) {
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
