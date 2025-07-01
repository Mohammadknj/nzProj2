#include <iostream>
#include <stack>
#include <vector>
using namespace std;

vector<char> encoding(string str) {
    vector<char> result;
    int i = 0;
    while (i < str.length()) {
        if (str[i] != ' ') {
            result.push_back(str[i]);
        }
        i++;
    }
    return result;
}

vector<string> grammarEncoder(string str) {
    int i = 1;
    while (str[i] != '>') {
        i++;
    }
    vector<string> rules;
    string rule;
    for (int j = i + 1; j < str.length(); ++j) {
        // if (str[j] != ' ') {
        //     if (str[j]!='|'){
        //         while ((str[j] != ' ' && str[j]!='|') && j < str.length()) {
        //             rule += str[j];
        //             j++;
        //         }
        //         rules.push_back(rule);
        //         rule = "";
        //         j--;
        //     }
        // }
        if (str[j] == ' ') {
            continue;
        }
        if (str[j] == '|') {
            rules.push_back(rule);
            rule = "";
            continue;
        }
        // while ((str[j] != ' ' && str[j]!='|') && j < str.length()) {
        //     rule += str[j];
        //     j++;
        // }
        rule += str[j];
    }
    rules.push_back(rule);
    return rules;
}

void printGrammar(vector<char> variables, vector<vector<string>> rules) {
    for (int i = 0; i < rules.size(); ++i) {
        cout << variables[i] << " -> ";
        for (int j = 0; j < rules[i].size(); ++j) {
            cout << rules[i][j] << " | ";
        }
        cout << "\b\b  " << endl;
    }
}

bool isBetweenTheseVariables(vector<char> Variables, char variable) {
    for (int i = 0; i < Variables.size(); ++i) {
        if (Variables[i] == variable)
            return true;
    }
    return false;
}

int hasLandaVariable(char variable, string rule) {
    int res = 0;
    for (int i = 0; i < rule.size(); ++i) {
        if (rule[i] == variable)
            res++;
    }
    return res;
}

bool isThisRuleNullable(vector<char> alphabet, vector<char> landaVariables,
                        string rule) {
    for (int i = 0; i < alphabet.size(); ++i) {
        for (int j = 0; j < rule.size(); ++j) {
            if (rule[j] == alphabet[i])
                return false;
        }
    }
    for (int i = 0; i < landaVariables.size(); ++i) {
        int len = rule.size();
        for (int j = 0; j < len; ++j) {
            if (rule[j] == landaVariables[i]) {
                rule.erase(j, 1);
                j--;
            }
        }
    }
    if (rule != "")
        return false;
    return true;
}

string makeNewRule(char landaVariable, string rule) {
    string result = "";
    for (int i = 0; i < rule.length(); ++i) {
        if (rule[i] != landaVariable)
            result += rule[i];
    }
    return result;
}

vector<string> makeNewRules(char landaVariable, string rule, int outputSize) {
    vector<string> result;
    string s;
    int cnt = 0;
    int indx[outputSize];
    for (int i = 0; i < rule.length(); ++i) {
        if (rule[i] == landaVariable) {
            indx[cnt] = i;
            cnt++;
        }
    }
    for (int i = 0; i < outputSize; ++i) {
        s = "";
        for (int j = 0; j < rule.size(); ++j) {
            if (indx[i] != j)
                s += rule[j];
        }
        result.push_back(s);
    }
    result.push_back(makeNewRule(landaVariable, rule));
    return result;
}

void selfRuleDeleting(vector<char> variables, vector<vector<string>> &rules) {
    for (int j = 0; j < rules.size(); ++j) {
        int len = rules[j].size();
        for (int k = 0; k < len; ++k) {
            if (rules[j][k].size() == 1 && variables[j] == rules[j][k][0]) {
                rules[j].erase(rules[j].begin() + k);
                k--;
                len--;
            }
        }
    }
}

void deleteRepeatedRules(vector<char> variables, vector<vector<string>> &rules){
    int len;
    for (int i = 0; i < rules.size(); ++i) {
        for (int j = 0; j < rules[i].size(); ++j) {
            len = rules[i].size();
            for (int k = 0; k < len; ++k) {
                if(k != j && rules[i][k] == rules[i][j]){
                    rules[i].erase(rules[i].begin()+k);
                    k--;
                    len--;
                }
            }
        }
    }
}
void deletingLanda(vector<char> alphabet, vector<char> variables,
                   vector<vector<string>> &rules) {
    bool landaFound = false;
    vector<char> landaVariables;
    for (int i = 0; i < rules.size(); ++i) {
        for (int j = 0; j < rules[i].size(); ++j) {
            if (rules[i][j] == "@") {
                landaVariables.push_back(variables[i]);
                landaFound = true;
                break;
            }
        }
    }
    if (!landaFound)
        return;
    for (int i = 0; i < rules.size(); ++i) {
        // Checking to prevent check a landaVariable
        if (!isBetweenTheseVariables(landaVariables, variables[i])) {
            for (int j = 0; j < rules[i].size(); ++j) {
                if (isThisRuleNullable(alphabet, landaVariables, rules[i][j])) {
                    // If the rule is nullable, so this variable is landaVariable
                    landaVariables.push_back(variables[i]);
                    i = -1;
                    break;
                }
            }
        }
    }
    // Deleting landa and adding new rules for each landaVariable
    for (int k = 0; k < landaVariables.size(); ++k) {
        // Checking whole grammar
        // cout<<"Dj";
        for (int i = 0; i < rules.size(); ++i) {
            int len = rules[i].size();
            for (int j = 0; j < len; ++j) {
                if (rules[i][j] == "@") {
                    rules[i].erase(rules[i].begin() + j);
                    j--;
                    len--;
                    continue;
                }
                int LandaVariablesNum =
                    hasLandaVariable(landaVariables[k], rules[i][j]);
                if (LandaVariablesNum == 1) {
                    if (rules[i][j].length() > 1)
                        rules[i].push_back(makeNewRule(landaVariables[k], rules[i][j]));
                } else if (LandaVariablesNum > 1) {
                    vector<string> new_rules =
                        makeNewRules(landaVariables[k], rules[i][j], LandaVariablesNum);
                    for (int k = 0; k < new_rules.size(); ++k) {
                        if (new_rules[k].size() > 0)
                            rules[i].push_back(new_rules[k]);
                    }
                }
            }
        }
    }
    if (isBetweenTheseVariables(landaVariables, variables[0]))
        rules[0].push_back("@");
    selfRuleDeleting(variables, rules);
    deleteRepeatedRules(variables,rules);
}

int findVariableIndex(vector<char> variables, char variable){
    for (int i = 0; i < variables.size(); ++i) {
        if(variable == variables[i]){
            return i;
        }
    }
}
void deletingChain(vector<char> variables,
                   vector<vector<string>> &rules) {
    stack<char> chainSequence;
    for (int i = 0; i < rules.size(); ++i) {
        for (int j = 0; j < rules[i].size(); ++j) {
            if (rules[i][j].size() == 1 &&
                isBetweenTheseVariables(variables, rules[i][j][0]) &&
                rules[i][j][0] != variables[i]) {
                chainSequence.push(variables[i]);
            }
        }
    }
    while(!chainSequence.empty()){
        int index=0;
        for (int i = 0; i < variables.size(); ++i) {
            if(chainSequence.top() == variables[i]){
                index = i;
                break;
            }
        }
        int len = rules[index].size();
        for (int i = 0; i < len; ++i) {
            if (rules[index][i].size() == 1 &&
                isBetweenTheseVariables(variables, rules[index][i][0]) &&
                rules[index][i][0] != variables[index]) {
                // chainSequence.push(variables[i]);
                int ind = findVariableIndex(variables, rules[index][i][0]);
                rules[index].erase(rules[index].begin()+i);
                i--;
                len--;
                for (int j = 0; j < rules[ind].size(); ++j) {
                    rules[index].push_back(rules[ind][j]);
                }
            }
        }
        selfRuleDeleting(variables, rules);
        deleteRepeatedRules(variables, rules);
        chainSequence.pop();
    }
}

void deletingUselessProductions(vector<char> alphabet, vector<char> variables,
                                vector<vector<string>> &rules){

}

int main() {
    string s;
    cout << "HELLO! Welcome to our CNF convertor\n";
    cout << "Please enter number of grammar's rule: ";
    int rulesNum;
    cin >> rulesNum;
    cout << "Enter the alphabet: ";
    cin.ignore();
    getline(cin, s);
    vector<char> alphabet = encoding(s);
    cout << "Enter the variables: ";
    getline(cin, s);
    vector<char> variables = encoding(s);
    cout << "Enter grammar's rules. You should separate them with an enter:\n";
    vector<vector<string>> rules;
    // string rules[3]
    for (int i = 0; i < variables.size(); ++i) {
        getline(cin, s);
        // vector<string> rule = ;
        rules.push_back(grammarEncoder(s));
    }
    deletingLanda(alphabet, variables, rules);
    cout << "Grammar after deleting landa rules:\n";
    printGrammar(variables, rules);
    deletingChain(variables, rules);
    cout << "Grammar after deleting chain rules:\n";
    printGrammar(variables, rules);
    deletingUselessProductions(alphabet, variables, rules);
    return 0;
}
// 3
// a b
// S A B
// S -> ASB
// A -> aAS | a | @
// B -> SbS | A | bb

// 4
// a b c
// S A B C
// S -> ACA
// A -> aAa | B | C
// B -> bB | b
// C -> cC | @

// 3
// a b c
// S B A
// S -> Aa | B
// B -> A | bb
// A -> a | bc | B
