#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iterator>
#include <iostream>

using namespace std;

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

int main() {
    vector<string> result;
    vector<string> test;
    vector<string> dictionary;
    vector<string> original;

    ifstream resultFile("result.txt"); 
    ifstream testFile("test.txt");
    ifstream dictionaryFile("dictionary.txt");
    ifstream originalFile("original.txt");

    copy(istream_iterator<string>(resultFile),
         istream_iterator<string>(),
         back_inserter(result));
    copy(istream_iterator<string>(testFile),
         istream_iterator<string>(),
         back_inserter(test));
    copy(istream_iterator<string>(dictionaryFile),
         istream_iterator<string>(),
         back_inserter(dictionary));
    copy(istream_iterator<string>(originalFile),
         istream_iterator<string>(),
         back_inserter(original));

    int good = 0;
    int bad = 0;

   if(result.size() == test.size()) {
      for (int i = 0; i < result.size(); ++i)
      {
         string r = result[i];
         string t = test[i];
         if(r == t) {
            ++good;
         }
         else {
            cout << i+1 << " ORIGINAL: " << original[i] << " RESULT: " << result[i] << " TEST: " << test[i] << ' ';
             if (!count(dictionary.begin(), dictionary.end(), deleteSymb(test[i]))) {
                 cout << "Element not found";
             }
             else if(deleteSymb(original[i]) == deleteSymb(result[i])) {
                 cout << "PROBLEM";
             }
             cout << endl;
            ++bad;
         }
      }
      cout << "GOOD: " << good << endl << "BAD: " << bad << endl;
   } 
   else {
      cout << "Can't compare. Different Size " << result.size() << " " << test.size() << endl;
   }
}

