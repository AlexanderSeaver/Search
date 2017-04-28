/*Proof of Concept Search Program
 *Created 4-27-17 20:38
 * Coire Gavin-Hanner and Alex Seaver
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
using namespace std;

int main()
{
	//make a vector of the words
	vector<string> wordList;
	ifstream wordsTxt;
	wordsTxt.open("sortedWords.txt");
	string line;
	string indexedWord;
	while(!wordsTxt.eof())
	{
		getline(wordsTxt, indexedWord);
		//cout << line << endl;
		//indexedWord = line.substr(1, line.length()-1);
		wordList.push_back(indexedWord);
	}
	wordsTxt.close();
	for(int i=0; i < wordList.size()-1; i++)
	{
		wordList[i] = wordList[i].substr(1, wordList[i].length()-2);
		if(!wordList[i].empty() && wordList[i][wordList[i].size()-1] == '\r')
				{
					wordList[i].erase(wordList[i].size()-1);
				}
	}

	string searchWord = "";
	cout << "Enter a word to seach for: " << endl;
	cin >> searchWord;
	
	
	if ((binary_search(wordList.begin(), wordList.end(), searchWord)) == true)
	{
		cout << "Your word was found" << endl;
	}
	else
	{
		cout << "Your word was not found" << endl;
	}
	
	
	
	//it = find (myvector.begin(), myvector.end(), 30);
}
