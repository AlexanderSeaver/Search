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
	ifstream fin;
	fin.open("filePaths.txt");
	vector<string> filePaths;
	string path;
	while(!fin.eof())
	{
		getline(fin, path);
		filePaths.push_back(path);
	}
	vector<string> allPaths;
	bool found = false;
	for(int i = 0; i < wordList.size(); i++)
	{
		if(wordList[i].substr(0, wordList[i].find_first_of("#")) == searchWord)
		{
			string locations = wordList[i].substr(wordList[i].find_first_of("#"), (wordList[i].size() - wordList[i].substr(0, wordList[i].find_first_of("#")).size()));
			string book = locations.substr(locations.find_first_not_of("#"), (wordList[i].size() - wordList[i].substr(locations.find_first_not_of("$"), wordList[i].find_first_of("%")).size()));
			string path = filePaths[stoi(book)];
			path = path.substr(path.find_first_of("@") + 1, (path.size() - path.substr(0, path.find_first_of("@")).size() - 1));
			allPaths.push_back(path);
			found = true;
		}
	}
	if(found == false) cout << "Not found." << endl;
	ifstream bookTitle;
	for(int i = 0; i < allPaths.size(); i++)
	{
		string title;
		bookTitle.open(allPaths[i]);
		getline(bookTitle, title);
		cout << title << endl;
		getline(bookTitle, title);
		cout << title << endl;
		bookTitle.close();
	}
	
	
	//it = find (myvector.begin(), myvector.end(), 30);
}
