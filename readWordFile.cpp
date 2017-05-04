#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
using namespace std;
string fileName = "/home/students/seavera/project6/Search/wordfiles/book.bin";

int main()
{
	unsigned short value;
	ifstream fin(fileName, ios::in | ios::binary);
	if (fin.is_open()) 
	{
    	while (!fin.eof()) 
    	{
      		fin.read((char*)&value,sizeof(unsigned short));
      		if(!fin.eof())
      		{
      			cout << value << endl;
      		}
    	}
		fin.close();
  	} 
  	else 
  	{
    	cout << "Unable to open file";
  	}
  	return 0;
}