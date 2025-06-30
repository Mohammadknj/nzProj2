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
    // A:
    for (int i = 0; i < rules.size(); ++i) {
        for (int j = 0; j < rules[i].size(); ++j) {
            if(isThisRuleNullable(alphabet, landaVariables, rules[i][j])){
                landaVariables.push_back(variables[i]);
                // goto A;
                i = -1;
                break;
            }
        }
    }

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
