#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <map>
#include "fifo.h"
using namespace std;
string filePath = "/home/students/seavera/project6/Search/txtFiles/wordfiles/";

string recieve_fifo = "searchWord_ajax_to_server";
string send_fifo = "searchWord_server_to_ajax";

int main()
{
	map< unsigned short, string> bookList;
	
	//receive the search word from the Ajax
	Fifo searchWordFifo_recieve(recieve_fifo);
	Fifo searchWordFifo_send(send_fifo);
	
	//create a vector containing all of the book paths
	vector<string> paths;
	ifstream inPaths;
	inPaths.open("/home/students/seavera/project6/Search/txtFiles/filePaths.txt");
	while(!inPaths.eof())
	{
		string fullPath;
		getline(inPaths, fullPath);
		paths.push_back(fullPath);
	}
	inPaths.close();
	cout << "file paths built\n";
	
	//while(1)
	//{
		searchWordFifo_recieve.openread();
		string rec = searchWordFifo_recieve.recv();
		searchWordFifo_recieve.fifoclose();
		unsigned short bookNum; 
		unsigned int lineNum;
		string fileName = filePath + rec + ".bin"; //what word file to open
		int counter = 0;
		ifstream fin(fileName, ios::in | ios::binary); //open the file
		if (!fin.fail()) 
		{
			unsigned int lineCheck;
			while(!fin.eof()) 
			{
				string book, line;
				fin.read((char*)&bookNum,sizeof(bookNum)); //get book and line number from the binary word file
				fin.read((char*)&lineNum,sizeof(lineNum));
				if(lineNum != lineCheck)
				{
					counter++;
					lineCheck = lineNum;
					//get the substring that is the book path
					book = paths[bookNum].substr(paths[bookNum].find_first_of("@")+1, paths[bookNum].size()-paths[bookNum].find_first_of("@")+1);
					//
			
					//look up title of book
					inPaths.open(book);
					string title;
					string tempTitle;
					// Find book Title
					bool titleFound = false;
					bool titleIndexed;
					if (bookList.find(bookNum) != bookList.end())
					{
						titleIndexed = true;
						title = bookList[bookNum];
					}
					else
					{
						titleIndexed = false;
					}
					for (int i = 0; i < 100 && titleFound == false && titleIndexed == false; i++)
					{
						getline(inPaths, tempTitle);
						if(tempTitle.find("Title:") != string::npos)
						{
							titleFound = true;
							title = tempTitle.substr(tempTitle.find("Title:") + 6, (tempTitle.size() - (tempTitle.find("Title:") + 6)));
						}
					}
					if(titleFound == false)
					{
						title = "Title not found for this book";
					}
					//
			
					//Make map of book titles
					if(bookList[bookNum] != title)
					{
						bookList[bookNum] = title;
					}
					inPaths.close();
					//
					inPaths.open(book); 				//open the correct book file
					inPaths.seekg(lineNum, ios::beg); 	//find the correct position in the book
					//make sendString the entire line that the word is found on
					int end_of_line = 0;
					bool endFound = false;
					char buffer[1000];
					inPaths.read(buffer, 1000);
					string wordLine = "";
					for(int i =0; i < 1000 && (buffer[i] != '\r' && buffer[i] != '\n'); i++)
					{
						wordLine.push_back(buffer[i]);
					}
					//
					//string sendString = book + "!" + wordLine;
					string sendString = bookList[bookNum].substr(0, (bookList[bookNum].find_first_of("\r"))) + "!" + wordLine;
					cout << sendString << endl;
					searchWordFifo_send.openwrite();
					searchWordFifo_send.send(sendString);
					searchWordFifo_send.fifoclose();
					inPaths.close();
				}
			}
			fin.close();
			
		} 
		else 
		{
			string sorry = "Sorry, that word does not appear to be in our list.";
			cout << sorry << endl;
			searchWordFifo_send.openwrite();
			searchWordFifo_send.send(sorry);
			searchWordFifo_send.fifoclose();
			searchWordFifo_send.openwrite();
			searchWordFifo_send.send("$END");
			searchWordFifo_send.fifoclose();
			cout << "Done\n";
		
		}
		searchWordFifo_send.openwrite();
		searchWordFifo_send.send("$END");
		searchWordFifo_send.fifoclose();
		cout << "Done\n";
	//}
		return 0;
}