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
#include "fifo.h"
using namespace std;
string filePath = "/home/students/seavera/project6/Search/txtFiles/wordfiles/";

string recieve_fifo = "searchWord_ajax_to_server";
string send_fifo = "searchWord_server_to_ajax";

int main()
{
	Fifo searchWordFifo_recieve(recieve_fifo);
	Fifo searchWordFifo_send(send_fifo);
	searchWordFifo_recieve.openread();
	string rec = searchWordFifo_recieve.recv();
	cout << rec << endl;
	searchWordFifo_recieve.fifoclose();
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
	
	unsigned short value;
	string fileName = filePath + rec + ".bin";
	int counter = 0;
	ifstream fin(fileName, ios::in | ios::binary);
	if (fin.is_open()) 
	{
    	while(!fin.eof())// && counter < 10) 
    	{
			unsigned short bookNum, lineNum;
			string book, line;
      		fin.read((char*)&value,sizeof(unsigned short));
      		if(!fin.eof())
      		{
      			bookNum = value;
      		}
			fin.read((char*)&value,sizeof(unsigned short));
      		if(!fin.eof())
      		{
      			lineNum = value;
      		}
			counter++;
			
			book = paths[bookNum].substr(paths[bookNum].find_first_of("@")+1, paths[bookNum].size()-paths[bookNum].find_first_of("@")+1);
			cout << book << endl;
			inPaths.open(book);
			for(int i = 0; i <= lineNum; i++)
			{
				getline(inPaths, line);
				if(i+1 == lineNum)
				{
					cout << line << endl;
					string sendString = book + "!" + line;
					searchWordFifo_send.openwrite();
					searchWordFifo_send.send(sendString);
					searchWordFifo_send.fifoclose();
				}
			}
			inPaths.close();
    	}
		fin.close();
		searchWordFifo_send.openwrite();
		searchWordFifo_send.send("$END");
		searchWordFifo_send.fifoclose();
		cout << "Done\n";
  	} 
  	else 
  	{
    	cout << "Sorry, that word does not appear to be in our list.\n";
  	}
  	return 0;
}