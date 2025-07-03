#include <iostream>
#include <stack>
#include <vector>
using namespace std;

// making alphabets and variables
vector<char> encoding(string str) {
    vector<char> result;
    int i = 0;
    while (i < int(str.length())) {
        if (str[i] != ' ') {
            result.push_back(str[i]);
        }
        i++;
    }
    return result;
}

// making grammar rules
vector<string> grammarEncoder(string str) {
    int i = 1;
    while (str[i] != '>') {
        i++;
    }
    vector<string> rules;
    string rule;
    for (int j = i + 1; j < int(str.length()); ++j) {
        if (str[j] == ' ') {
            continue;
        }
        if (str[j] == '|') {
            rules.push_back(rule);
            rule = "";
            continue;
        }
        rule += str[j];
    }
    rules.push_back(rule);
    return rules;
}

void printGrammar(vector<char> variables, vector<vector<string>> rules, string str) {
    if(rules.size()<1) return;
    cout << "Grammar after deleting "<<str<<":"<<endl;
    for (int i = 0; i < int(rules.size()); ++i) {
        cout << variables[i] << " -> ";
        for (int j = 0; j < int(rules[i].size()); ++j) {
            cout << rules[i][j] << " | ";
        }
        cout << "\b\b  " << endl;
    }
}

bool isInTheseVariables(vector<char> Variables, char variable) {
    for (int i = 0; i < int(Variables.size()); ++i) {
        if (Variables[i] == variable)
            return true;
    }
    return false;
}

// returning number of landaVariables used in one rule
int landaVariablesNum(char variable, string rule) {
    int res = 0;
    for (int i = 0; i < int(rule.size()); ++i) {
        if (rule[i] == variable)
            res++;
    }
    return res;
}

bool isThisRuleNullable(vector<char> alphabet, vector<char> landaVariables,
                        string rule) {
    for (int i = 0; i < int(alphabet.size()); ++i) {
        for (int j = 0; j < int(rule.size()); ++j) {
            if (rule[j] == alphabet[i])
                return false;
        }
    }
    for (int i = 0; i < int(landaVariables.size()); ++i) {
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

// making a new rule without the current landaVariable
string makeNewRule(char landaVariable, string rule) {
    string result = "";
    for (int i = 0; i < int(rule.length()); ++i) {
        if (rule[i] != landaVariable)
            result += rule[i];
    }
    return result;
}

// making a new rule without the current landaVariable
// used for rules with 2 same landaVariable
vector<string> makeNewRules(char landaVariable, string rule, int &landaVarNum) {
    vector<string> result;
    string s;
    int cnt = 0;
    // finding landaVariables index
    int indx[landaVarNum];
    for (int i = 0; i < int(rule.length()); ++i) {
        if (rule[i] == landaVariable) {
            indx[cnt] = i;
            cnt++;
        }
    }
    // making new rules without this currentIndex landaVariable
    for (int i = 0; i < landaVarNum; ++i) {
        s = "";
        for (int j = 0; j < int(rule.size()); ++j) {
            if (indx[i] != j)
                s += rule[j];
        }
        result.push_back(s);
    }
    result.push_back(makeNewRule(landaVariable, rule));
    return result;
}

// deleting rules that are looping on itself like A -> A
void selfRuleDeleting(vector<char> variables, vector<vector<string>> &rules) {
    for (int j = 0; j < int(rules.size()); ++j) {
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

// deleting same rules that are for one variable like A -> a | a  => A -> a
void deleteRepeatedRules(vector<vector<string>> &rules){
    int len;
    for (int i = 0; i < int(rules.size()); ++i) {
        for (int j = 0; j < int(rules[i].size()); ++j) {
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
    // finding landaVariables which has a direct landa rule
    bool landaFound = false;
    vector<char> landaVariables;
    for (int i = 0; i < int(rules.size()); ++i) {
        for (int j = 0; j < int(rules[i].size()); ++j) {
            if (rules[i][j] == "@") {
                landaVariables.push_back(variables[i]);
                landaFound = true;
                break;
            }
        }
    }
    if (!landaFound)
        return;

    // finding landaVariables which has a indirect landa rule
    for (int i = 0; i < int(rules.size()); ++i) {
        // Checking to prevent check a landaVariable
        if (!isInTheseVariables(landaVariables, variables[i])) {
            for (int j = 0; j < int(rules[i].size()); ++j) {
                if (isThisRuleNullable(alphabet, landaVariables, rules[i][j])) {
                    // If the rule is nullable, so this variable is landaVariable
                    landaVariables.push_back(variables[i]);
                    // Start searching from S
                    i = -1;
                    break;
                }
            }
        }
    }
    // Deleting landa and adding new rules for each landaVariable
    for (int k = 0; k < int(landaVariables.size()); ++k) {
        // Checking whole grammar
        for (int i = 0; i < int(rules.size()); ++i) {
            int len = rules[i].size();
            for (int j = 0; j < len; ++j) {
                if (rules[i][j] == "@") {
                    rules[i].erase(rules[i].begin() + j);
                    j--;
                    len--;
                    continue;
                }
                int LandaVariablesNum =
                    landaVariablesNum(landaVariables[k], rules[i][j]);
                if (LandaVariablesNum == 1) {
                    if (rules[i][j].length() > 1)
                        rules[i].push_back(makeNewRule(landaVariables[k], rules[i][j]));
                } else if (LandaVariablesNum > 1) {
                    vector<string> new_rules =
                        makeNewRules(landaVariables[k], rules[i][j], LandaVariablesNum);
                    for (int k = 0; k < int(new_rules.size()); ++k) {
                        if (new_rules[k].size() > 0)
                            rules[i].push_back(new_rules[k]);
                    }
                }
            }
        }
    }
    // If S is landaVariable, put landa in its rules
    if (isInTheseVariables(landaVariables, variables[0]))
        rules[0].push_back("@");
    selfRuleDeleting(variables, rules);
    deleteRepeatedRules(rules);
}

int findVariableIndex(vector<char> variables, char variable){
    for (int i = 0; i < int(variables.size()); ++i) {
        if(variable == variables[i]){
            return i;
        }
    }
}

void deletingChain(vector<char> variables,
                   vector<vector<string>> &rules) {
    // Finding chains and pushing to stack for popping sequenced
    stack<char> chainSequence;
    for (int i = 0; i < int(rules.size()); ++i) {
        for (int j = 0; j < int(rules[i].size()); ++j) {
            if (rules[i][j].size() == 1 &&
                isInTheseVariables(variables, rules[i][j][0]) &&
                rules[i][j][0] != variables[i]) {
                chainSequence.push(variables[i]);
            }
        }
    }
    // deleting chains
    while(!chainSequence.empty()){
        int index = findVariableIndex(variables, chainSequence.top());
        int len = rules[index].size();
        for (int i = 0; i < len; ++i) {
            if (rules[index][i].size() == 1 &&
                isInTheseVariables(variables, rules[index][i][0]) &&
                rules[index][i][0] != variables[index]) {
                int ind = findVariableIndex(variables, rules[index][i][0]);
                rules[index].erase(rules[index].begin()+i);
                i--;
                len--;
                for (int j = 0; j < int(rules[ind].size()); ++j) {
                    rules[index].push_back(rules[ind][j]);
                }
            }
        }
        selfRuleDeleting(variables, rules);
        deleteRepeatedRules(rules);
        chainSequence.pop();
    }
}

bool isInThisRule(string rule, char variable){
    for (int i = 0; i < int(rule.size()); ++i) {
        if(rule[i] == variable)
            return true;
    }
    return false;
}

vector<char> findIrrelativeVariables(vector<char> variables, vector<char> currentRelatives, vector<char> currentIrrelatives, vector<vector<string>> rules){
    vector<char> result = currentIrrelatives;
    int len = result.size();
    for (int i = 0; i < len; ++i) {
        for (int j = 0; j < int(currentRelatives.size()); ++j) {
            int index = findVariableIndex(variables, currentRelatives[j]);
            bool found = false;
            for (int k = 0; k < int(rules[index].size()); ++k) {
                if(isInThisRule(rules[index][k],result[i])){
                    result.erase(result.begin()+i);
                    len--;
                    i--;
                    found = true;
                    break;
                }
            }
            if(found) break;
        }
    }

    return result;
}

void deletingUnavailableVars(vector<char> &variables,
                                vector<vector<string>> &rules){
    vector<char> variablesCpy;
    vector<int> variablesCpyStatus;
    for (int i = 1; i < int(variables.size()); ++i) {
        variablesCpy.push_back(variables[i]);
        // 0 means this variable has not a direct realation with S
        // 1 means this variable has a direct realation with S
        variablesCpyStatus.push_back(0);
    }
    for (int i = 0; i < int(variablesCpy.size()); ++i) {
        for (int j = 0; j < int(rules[0].size()); ++j) {
            if(isInThisRule(rules[0][j],variablesCpy[i])){
                variablesCpyStatus[i] = 1;
                break;
            }
        }
    }
    vector<char> relatives;
    vector<char> irrelatives;
    for (int i = 0; i < int(variablesCpy.size()); ++i) {
        if(variablesCpyStatus[i] == 1)
            relatives.push_back(variablesCpy[i]);
        else
            irrelatives.push_back(variablesCpy[i]);

    }
    if(irrelatives.size()>0){
        irrelatives = findIrrelativeVariables(variables, relatives, irrelatives, rules);
        for (int i = 0; i < int(irrelatives.size()); ++i) {
            int ind = findVariableIndex(variables,irrelatives[i]);
            rules.erase(rules.begin()+ind);
            variables.erase(variables.begin()+ind);

        }
    }
    // cout<<"Grammar after deleting irrelative rules:\n";
    // printGrammar(variables,rules);
}

bool isInThisVector(vector<char> vec, char ch){
    for (int i = 0; i < int(vec.size()); ++i) {
        if(vec[i] == ch)
            return true;
    }
    return false;
}

bool isFinishableRule(string rule, vector<char> alphabet, vector<char> finishableVariables){
    for (int i = 0; i < int(rule.size()); ++i) {
        if(!isInThisVector(alphabet, rule[i]) && !isInThisVector(finishableVariables, rule[i]))
            return false;
    }
    return true;
}
// void deletingZeroRuleVar(vector<vector<string>> &rules){
//     for (int i = 0; i < rules.size(); ++i) {

//     }
// }

void deletingCountlessVariables(vector<char> alphabet, vector<char> &variables, vector<vector<string>> &rules){
    vector<char> finishableVars;
    // Finding finishableVars
    for (int i = 1; i < int(rules.size()); ++i) {
        if(!isInThisVector(finishableVars, variables[i]))
        for (int j = 0; j < int(rules[i].size()); ++j) {
            if(isFinishableRule(rules[i][j], alphabet, finishableVars)){
                finishableVars.push_back(variables[i]);
                i = 0;
                break;
            }
        }
    }
    // Deleting unfinishable variables and it's rule
    finishableVars.insert(finishableVars.begin(), variables[0]);
    vector<char> unfinishableVars;
    for (int i = 0; i < int(variables.size()); ++i) {
        if(!isInThisVector(finishableVars, variables[i])){
            unfinishableVars.push_back(variables[i]);
            rules.erase(rules.begin()+i);
            variables.erase(variables.begin()+i);
        }
    }
    // Deleting rules that consist unfinishable variables
    for (int i = 0; i < int(unfinishableVars.size()); ++i) {
        for (int j = 0; j < int(rules.size()); ++j) {
            int len = rules[j].size();
            for (int k = 0; k < len; ++k) {
                if(isInThisRule(rules[j][k], unfinishableVars[i])){
                    rules[j].erase(rules[j].begin()+k);
                    k--;
                    len--;
                    // break;
                }
            }
        }
    }
    selfRuleDeleting(variables, rules);
    deleteRepeatedRules(rules);
    // for checking countless loop on S
    // finishableVars.erase(finishableVars.begin());
    // int len = rules[0].size();
    // for (int i = 0; i < len; ++i) {
    //     if(isInThisRule(rules[0][i], variables[0])){
    //         for (int j = 0; j < len; ++j) {
    //             if(i != j && !isFinishableRule(rules[0][j], alphabet, finishableVars)){
    //                 rules[0].erase(rules[0].begin()+j);
    //                 j--;
    //                 len--;
    //             }
    //         }
    //     }
    // }
}

void deleteAllVars(vector<char> &variables, vector<vector<string>> &rules){
    // for (int i = 0; i < int(rules.size()); ++i) {
    //     for (int j = 0; j < int(rules[i].size()); ++j) {

    //     }
    // }
    rules.clear();
    variables.clear();
}

void deletingCountlessVariables1(vector<char> alphabet, vector<char> &variables, vector<vector<string>> &rules){
    vector<char> finishableVars;
    // Finding finishableVars
    for (int i = 0; i < int(rules.size()); ++i) {
        if(!isInThisVector(finishableVars, variables[i]))
            for (int j = 0; j < int(rules[i].size()); ++j) {
                if(isFinishableRule(rules[i][j], alphabet, finishableVars)){
                    finishableVars.push_back(variables[i]);
                    i = -1;
                    break;
                }
            }
    }
    // Deleting unfinishable variables and it's rule
    // finishableVars.insert(finishableVars.begin(), variables[0]);
    vector<char> unfinishableVars;
    for (int i = 0; i < int(variables.size()); ++i) {
        if(!isInThisVector(finishableVars, variables[i])){
            // If S variable is countless, So it will just make landa and we have
            if(i == 0){
                cout<<"This start variable ("<<variables[0]<<") just makes landa strings,"<<endl;
                deleteAllVars(variables, rules);
                return;
            }
            unfinishableVars.push_back(variables[i]);
            rules.erase(rules.begin()+i);
            variables.erase(variables.begin()+i);
        }
    }

    // Deleting rules that consist unfinishable variables
    for (int i = 0; i < int(unfinishableVars.size()); ++i) {
        for (int j = 0; j < int(rules.size()); ++j) {
            int len = rules[j].size();
            for (int k = 0; k < len; ++k) {
                if(isInThisRule(rules[j][k], unfinishableVars[i])){
                    rules[j].erase(rules[j].begin()+k);
                    k--;
                    len--;
                    // break;
                }
            }
        }
    }
    selfRuleDeleting(variables, rules);
    deleteRepeatedRules(rules);
    // for checking countless loop on S
    // finishableVars.erase(finishableVars.begin());
    // int len = rules[0].size();
    // for (int i = 0; i < len; ++i) {
    //     if(isInThisRule(rules[0][i], variables[0])){
    //         for (int j = 0; j < len; ++j) {
    //             if(i != j && !isFinishableRule(rules[0][j], alphabet, finishableVars)){
    //                 rules[0].erase(rules[0].begin()+j);
    //                 j--;
    //                 len--;
    //             }
    //         }
    //     }
    // }
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
    for (int i = 0; i < int(variables.size()); ++i) {
        getline(cin, s);
        rules.push_back(grammarEncoder(s));
    }
    deletingLanda(alphabet, variables, rules);
    printGrammar(variables, rules, "landa rules");
    deletingChain(variables, rules);
    printGrammar(variables, rules, "chain rules");
    deletingUnavailableVars(variables, rules);
    printGrammar(variables, rules, "unavailable variables");
    deletingCountlessVariables1(alphabet, variables, rules);
    printGrammar(variables, rules, "countless variables");
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
// A -> aAa | B
// B -> bB | b
// C -> cC | @

// 3
// a b c
// S B A
// S -> Aa | B
// B -> A | bb
// A -> a | bc | B

// 5
// a b
// S A B E F
// S -> BS | B
// A -> aA | aF
// B -> b
// E -> aAb | bSA
// F -> bB | b

// 4
// a b
// S A B C
// S -> aS | A | C
// A -> a
// B -> aa
// C ->aCb

// 3
// a b
// S B A
// S -> Sa | B
// B -> A | bb
// A -> aA

// 3
// a b
// S B A
// S -> ASa | B
// B -> A | bb
// A -> aA
