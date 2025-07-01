#include <iostream>
#include <vector>
using namespace std;

vector<char> encoding(string str){
    vector<char> result;
    int i = 0;
    while(i < str.length()){
        if(str[i] != ' '){
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
        if (str[j]=='|'){
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
void printGrammar(vector<char> variables, vector<vector<string>> rules){
    for (int i = 0; i < rules.size(); ++i) {
        cout<<variables[i] << " -> ";
        for (int j = 0; j < rules[i].size(); ++j) {
            cout<<rules[i][j]<<" | ";
        }
        cout<<"\b   j   \n";
    }
}
bool isLandaVariable(vector<char> landaVariables, char variable){
    for (int i = 0; i < landaVariables.size(); ++i) {
        if(landaVariables[i] == variable)
            return true;
    }
    return false;
}
bool hasLandaVariable(char variable, string rule){
    for (int i = 0; i < rule.size(); ++i) {
        if(rule[i] == variable)
            return true;
    }
    return false;
}
bool isThisRuleNullable(vector<char> alphabet, vector<char> landaVariables, string rule){
    for (int i = 0; i < alphabet.size(); ++i) {
        for (int j = 0; j < rule.size(); ++j) {
            if(rule[j] == alphabet[i])
                return false;
        }
    }
    for (int i = 0; i < landaVariables.size(); ++i) {
        for (int j = 0; j < rule.size(); ++j) {
            if(rule[j] == landaVariables[i])
                rule.erase(j, 1);
        }
    }
    if(rule != "")
        return false;
    return true;
}
string new_rule(char landaVariable, string rule){
    string result="";
    for (int i = 0; i < rule.length(); ++i) {
        if(rule[i] != landaVariable)
            result += rule[i];
    }
    // if(result == "") result
    return result;
}
void deletingLanda(vector<char> alphabet, vector<char> variables, vector<vector<string>> rules){
    bool landaFound = false;
    vector<char> landaVariables;
    for (int i = 0; i < rules.size(); ++i) {
        for (int j = 0; j < rules[i].size(); ++j) {
            if(rules[i][j]== "@"){
                landaVariables.push_back(variables[i]);
                landaFound = true;
                break;
            }
        }
    }
    if(!landaFound)
        return;
    for (int i = 0; i < rules.size(); ++i) {
        // Checking to prevent check a landaVariable
        if(!isLandaVariable(landaVariables, variables[i])){
            for (int j = 0; j < rules[i].size(); ++j) {
                if(isThisRuleNullable(alphabet, landaVariables, rules[i][j])){
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
                if(rules[i][j]== "@"){
                    rules[i].erase(rules[i].begin()+j);
                    j--;
                    len--;
                }
                if (hasLandaVariable(landaVariables[k], rules[i][j])) {
                    if (rules[i][j].length() > 1)
                        rules[i].push_back(
                            new_rule(landaVariables[k], rules[i][j]));
                    else {
                        rules[i].erase(rules[i].begin() + j);
                        j--;
                        len--;
                    }
                }
            }
        }
    }
    cout<<"Grammar after deleting landa rules:\n";
    printGrammar(variables, rules);
}
int main()
{
    string s;
    cout<<"HELLO! Welcome to our CNF convertor\n";
    cout<<"Please enter number of grammar's rule: ";
    int rulesNum;
    cin>>rulesNum;
    cout<<"Enter the alphabet: ";
    cin.ignore();
    getline(cin,s);
    vector<char> alphabet = encoding(s);
    cout<<"Enter the variables: ";
    getline(cin,s);
    vector<char> variables = encoding(s);
    cout<<"Enter grammar's rules. You should separate them with an enter:\n";
    vector<vector<string>> rules;
    // string rules[3]
    for (int i = 0; i < variables.size(); ++i) {
        getline(cin,s);
        // vector<string> rule = ;
        rules.push_back(grammarEncoder(s));
    }
    deletingLanda(alphabet, variables, rules);
    return 0;
}
// 3
// a b
// S A B
// S -> ASB
// A -> aAS | a | @
// B -> SbS | A | bb
