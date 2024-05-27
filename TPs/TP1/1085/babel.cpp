#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <string>
#include <queue>

#define MAXINT numeric_limits<int>::max()

using namespace std;

struct Language {
    string name;
    string previousLanguage = "";
    bool visited = false;
    int knownSmallestSequenceLength = MAXINT;
    char initialLetterLastKnownExpression = '\0';

    Language(){};

    Language(const string& name) {
        this->name = name;
    }

    bool operator<(const Language& other) const {
        return this->knownSmallestSequenceLength > other.knownSmallestSequenceLength;
    }
};

struct MappingExpresionEntry {
    string srcLanguage;
    string destLanguage;
    string expression;

    MappingExpresionEntry(string srcLanguage, string destLanguge, string expression) 
        : srcLanguage(srcLanguage), destLanguage(destLanguge), expression(expression) {}
    
};

class MappingExpressionsDictionary {

    private:
        string startLanguage;
        string endLanguage;
        unordered_map<string, vector<MappingExpresionEntry>> dictionary;
        unordered_map<string, Language> languages;

        bool checkIfLanguageIsValid(const Language& language) {
            bool knownSmallestSequenceLengthIsUpToDate = \
                language.knownSmallestSequenceLength == this->languages[language.name].knownSmallestSequenceLength;


            return knownSmallestSequenceLengthIsUpToDate && !language.visited;
        }

        bool areInitialLettersForAdjacentWordsTheSame(MappingExpresionEntry& mapExpression) {
            Language& previousLanguage = this->languages[mapExpression.srcLanguage];
            Language& nextLanguage = this->languages[mapExpression.destLanguage];
 
            if (previousLanguage.initialLetterLastKnownExpression == '\0') {
                return false;
            }
            else if (previousLanguage.initialLetterLastKnownExpression == mapExpression.expression.front()) {
                return true;
            }
            else {
                return false;
            }
        }

        void dijkstra(void) {
            // Priority Queue Initialization
            priority_queue<Language> pq;
            languages[this->endLanguage].knownSmallestSequenceLength = 0;
            pq.push(this->languages[this->endLanguage]);

            while (!pq.empty() && (pq.top().name != this->startLanguage)) {
                Language& currentLanguage = this->languages[pq.top().name];
                pq.pop();

                if (!checkIfLanguageIsValid(currentLanguage)) {
                    continue;
                }

                currentLanguage.visited = true;

                for (MappingExpresionEntry mapExpression : this->dictionary[currentLanguage.name]) {
                    Language& nextLanguage = this->languages[mapExpression.destLanguage];
                    if (nextLanguage.visited) {
                        continue;
                    }

                    float currentSequenceLength = currentLanguage.knownSmallestSequenceLength;
                    float updatedSequenceLength = currentSequenceLength + mapExpression.expression.length();
                    if ((updatedSequenceLength < nextLanguage.knownSmallestSequenceLength) 
                            && (!areInitialLettersForAdjacentWordsTheSame(mapExpression))) {
                        nextLanguage.knownSmallestSequenceLength = updatedSequenceLength;
                        nextLanguage.previousLanguage = currentLanguage.name;
                        nextLanguage.initialLetterLastKnownExpression = mapExpression.expression.front();
                        pq.push(nextLanguage);
                    }
                }
            }   
        }
    
    public:
        MappingExpressionsDictionary(
                string& startLanguage, 
                string& endLanguage, 
                vector<MappingExpresionEntry>& expressionMaps)
        {
            this->startLanguage = startLanguage;
            this->endLanguage = endLanguage;
            languages[startLanguage] = Language(startLanguage);
            languages[endLanguage] = Language(endLanguage);

            for(const MappingExpresionEntry& mapExpEntry: expressionMaps) {
                if(languages.find(mapExpEntry.srcLanguage) == languages.end()) {
                    languages[mapExpEntry.srcLanguage] = Language(mapExpEntry.srcLanguage); 
                }
                else if(languages.find(mapExpEntry.destLanguage) == languages.end()) {
                    languages[mapExpEntry.destLanguage] = Language(mapExpEntry.destLanguage); 
                }
                dictionary[mapExpEntry.srcLanguage].push_back(mapExpEntry);
                MappingExpresionEntry reversedExpressionMap(mapExpEntry);
                reversedExpressionMap.srcLanguage = mapExpEntry.destLanguage;
                reversedExpressionMap.destLanguage = mapExpEntry.srcLanguage;
                dictionary[reversedExpressionMap.srcLanguage].push_back(reversedExpressionMap);
            }
        }

        int getSmallestSequenceLength() {
            dijkstra();
            return this->languages[this->startLanguage].knownSmallestSequenceLength;
        }

};


void readInput(vector<MappingExpressionsDictionary>& mapExpressionDicts) {
    int numMappings;
    while(true) {
        cin >> numMappings;

        if(numMappings == 0) {
            break;
        }
        string startLanguage, endLanguage;

        cin >> startLanguage >> endLanguage;

        vector<MappingExpresionEntry> expressionMaps;

        for(int i = 0; i < numMappings; i++) {
            string srcLanguage, destLanguage, expression;
            cin >> srcLanguage >> destLanguage >> expression;
            MappingExpresionEntry  mapExpEntry(srcLanguage, destLanguage, expression);
            expressionMaps.emplace_back(mapExpEntry);
        }

        MappingExpressionsDictionary mapExpressionDict(startLanguage, endLanguage, expressionMaps);

        mapExpressionDicts.emplace_back(mapExpressionDict);
    }
}


int main() {
    ios::sync_with_stdio(false);
    vector<MappingExpressionsDictionary> mapExpressionDict;

    readInput(mapExpressionDict);

    for (MappingExpressionsDictionary& mapExpressionDict : mapExpressionDict) {
        int minimumSequenceLength = mapExpressionDict.getSmallestSequenceLength();
        if (minimumSequenceLength == MAXINT) {
            cout << "impossivel" << endl;
        }
        else {
            cout << minimumSequenceLength << endl;
        }
    }

    return 0;
}