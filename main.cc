#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
using namespace std;

#define MAX_NOTE 99
#define FIND_RANGE_UNDER 1
#define FIND_RANGE_OVER 2
#define DELETE_COST 1
#define INSERT_COST 0.9
#define SUBSTITUTION_COST 1.1
#define TRANSPOSE_COST 2.0

float calc_distance(string str1, string str2)
{
    double d[str1.length()+1][str2.length()+1];
    double l_cost;

    for (int i = 0; i <= str1.length(); i++)
    {
        d[i][0] = i;
    }
    for(int j = 0; j <= str2.length(); j++)
    {
        d[0][j] = j;
    }
    for (int i = 1; i <= str1.length(); i++)
    {
        for(int j = 1; j <= str2.length(); j++)
        {
            l_cost = str1[i-1] == str2[j-1] ? 0 : SUBSTITUTION_COST;
            d[i][j] = min(d[i-1][j] + DELETE_COST, // delete
                    min(d[i][j-1] + INSERT_COST, // insert
                    d[i-1][j-1] + l_cost) // substitution
            );
            if( (i > 1) && (j > 1) && (str1[i-1] == str2[j-2]) && (str1[i-2] == str2[j-1]))
            {
                d[i][j] = min(d[i][j], d[i-2][j-2] + l_cost + 200); //transposition
            }
        }
    }
    return d[str1.length()][str2.length()];
}

string getSymb(const string &s) {
    int found = s.find(',');
    if(found != -1) {
       return ",";
    }
    found=s.find('.');
    if(found != -1) {
       return ".";
    }
    found=s.find(':');
    if(found != -1) {
       return ":";
    }
    found=s.find(';');
    if(found != -1) {
       return ";";
    }
    return "";
}

string deleteSymb(const string &s) {
    int found = s.find(',');
    if(found != -1) {
        return s.substr(0, found);
    }
    found=s.find('.');
    if(found != -1) {
        return s.substr(0, found);
    }
    found=s.find(':');
    if(found != -1) {
        return s.substr(0, found);
    }
    found=s.find(';');
    if(found != -1) {
        return s.substr(0, found);
    }
    return s;
}

string getWord(const string &word, const map<int, vector<string>> &dic) {
    string result;
    double min = MAX_NOTE;
    int from = deleteSymb(word).size() <= dic.begin()->first ? dic.begin()->first  : deleteSymb(word).size()-FIND_RANGE_UNDER;
    int to = deleteSymb(word).size() >= dic.end()->first  ? dic.end()->first  : deleteSymb(word).size()+FIND_RANGE_OVER;
    to = to >= dic.end()->first ? dic.end()->first : to;
    for (int j = from; j <= to; ++j) {
        vector<string> dicPart = dic.find(j)->second;
        vector<string> sameNotes;
        for(int i = 0; i < dicPart.size(); ++i) {
            double aux = calc_distance(deleteSymb(word), dicPart[i]);
            if(aux <= min) {
                min = aux;
                result = dicPart[i];
            }
        }
    }
    return result + getSymb(word);
}

int main() {
    map<int, vector<string> > dictionary;

    ifstream originalFile("original.txt");
    ifstream dictionaryFile("dictionary.txt");
    string str;
    while (getline(dictionaryFile, str)) {
        if(dictionary.count(str.size()) != 0) {
            dictionary[str.size()].push_back(str);
        } else {
            vector<string> aux = {str};
            dictionary[str.size()] = aux;
        }
    }

    while (getline(originalFile, str)) {
        istringstream ss(str);
        string word;
        bool first = true;
        while (ss >> word) {
            if(first) {
                first = false;
                cout << getWord(word, dictionary);
            }
            else {
                cout << ' ' << getWord(word, dictionary);
            }
        }
        cout << endl;
    }
}

// PRINT MAP
//    for(auto it = dictionary.cbegin(); it != dictionary.cend(); ++it)
//    {
//        cout << it->first << endl;
//        for(int i = 0; i < it->second.size(); ++i) {
//            cout <<  it->second[i] << ' ';
//        }
//        cout << endl;
//    }

