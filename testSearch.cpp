#include <iostream>
#include <fstream>
#include <string>
#include <dirent.h>
#include <algorithm>
#include <vector>
#include <sstream>

using namespace std;

void ProcessDirectory(string directory,string word, ofstream& filePaths);
void ProcessFile(string file,string word, ofstream& filePaths);
void ProcessEntity(struct dirent* entity,string word, ofstream& filePaths);
bool hasEnding (string const &fullString, string const &ending);
int stringMatchCount(string file, string word);

string path = "/home/skon/books/";
int fileCount = 0;
int matchCount = 0;
int fileMatchCount = 0;
long long wordCount = 0;
string delimiters = " ,.;:?'\"()[]";
unsigned short pathNum = 0;
vector<string> stopList;
vector<string> wordList;
int wordListCounter = 0;
int wordCounter = 0;
int stopCount = 0;


int main()
{
	ifstream fin;
	fin.open("stopwords.txt");
	string stopword;
	while(!fin.eof()) //make the list of stopwords
	{
		getline(fin, stopword);
		stopList.push_back(stopword);
		stopCount++;
	}
	fin.close(); //close the stopwords text file
	ofstream filePaths; //open the filepaths txt file
	filePaths.open("filePaths.txt", std::ios_base::app);
	string word;
	string directory = "";
	cout << "Word to search for: ";
	cin >> word;
	// Convert to lower case
	transform(word.begin(), word.end(), word.begin(), ::tolower);
	
	ProcessDirectory(directory,word,filePaths);
	
	
	cout << "Vector formed, beginning sort." << endl;
	sort(wordList.begin(), wordList.end());
	cout << "Sort finished, outputting to text file." << endl;
	ofstream sortedWords;
	sortedWords.open("sortedWords.txt");
	for(int i = 0; i < wordListCounter; i++)
	{
		sortedWords << "!" << wordList[i] << "@" << endl;
	}
	sortedWords.close();
	cout << "The word \"" << word << "\" found " << matchCount << " times in " << fileMatchCount << " books and " << wordCount << " words" << endl; 
	cout << "Total Books:" << fileCount << endl;
	filePaths.close();
	return 0;
}

bool hasEnding (string const &fullString, string const &ending) {
  if (fullString.length() >= ending.length()) {
    return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
  } else {
    return false;
  }
}

void ProcessDirectory(string directory, string word, ofstream& filePaths)
{
  string dirToOpen = path + directory;
  DIR *dir;
  dir = opendir(dirToOpen.c_str());

  //set the new path for the content of the directory
  path = dirToOpen + "/";

  //  cout << "Process directory: " << dirToOpen.c_str() << endl;

  if(NULL == dir)
    {
      cout << "could not open directory: " << dirToOpen.c_str() << endl;
      return;
    }
  struct dirent *entity;
  entity = readdir(dir);

  while(entity != NULL && wordListCounter < 25000)
    {
      ProcessEntity(entity,word,filePaths);
      entity = readdir(dir);
    }

  //we finished with the directory so remove it from the path
  path.resize(path.length() - 1 - directory.length());
  closedir(dir);
}

void ProcessEntity(struct dirent* entity, string word, ofstream& filePaths)
{
  //find entity type
  if(entity->d_type == DT_DIR)
    {//it's an direcotry
      //don't process the  '..' and the '.' directories
      if(entity->d_name[0] == '.')
	{
	  return;
	}

      //it's an directory so process it
      ProcessDirectory(string(entity->d_name),word,filePaths);
      return;
    }

  if(entity->d_type == DT_REG)
    {//regular file
      ProcessFile(string(entity->d_name), word, filePaths);
      return;
    }

  //there are some other types
  //read here http://linux.die.net/man/3/readdir
  cout << "Not a file or directory: " << entity->d_name << endl;
}

void ProcessFile(string file, string word, ofstream& filePaths)
{
  ofstream log;
  log.open("log.txt", std::ios_base::app);
  string fileType = ".txt";
  if (hasEnding(file,fileType)) {
      fileCount++;
      if (word.length()>0 && wordListCounter < 10000) {
	int matches = stringMatchCount(file,word);
	if (1) {
	  fileMatchCount++;
	  matchCount += matches;
	  cout << path << file << endl; //when it encounters a new file
	  log << path << file << endl;
	  pathNum++;
	}
      }
  }
  log.close();
  //if you want to do something with the file add your code here
}

string getNext(string & line) {
  string next;
  //cout << "$" << line.length();
  size_t start = line.find_first_not_of(delimiters);
  if (start != string::npos) {
    //cout << ":" << start;
    size_t end = line.find_first_of(delimiters,start);
    if (end != string::npos) {
      //cout << "#" << end;
      // word with delimiters on both sides
      next = line.substr(start,end-start);
      line.erase(0,end+1);
    } else {
      // word with delimiter only at start
      next = line.substr(start);
      line = "";
    }
  } else {
    // not delimiters found at all
    next = line;
    line = "";
  }
  //cout << "%" << next << "&" << line << endl;
  return next;
}

int stringMatchCount(string file, string word) {
  ifstream infile;
  int count = 0;
  string line,w;

  try{
    string fileWithPath = path+file;
    infile.open(fileWithPath.c_str());
    //    cout << "open:" << fileWithPath << endl;
	int lines = 0;
	int words;
    while(!infile.eof() && lines++ < 10000)
	{
		words = 0;
		// normalize to lower case
		getline(infile,line);
		//cout << line << endl;
		while (line.length()>0 && words++ < 100) 
		{
			bool wordPresent = false;
			bool stopPresent = false;
			w = getNext(line);
			transform(w.begin(), w.end(), w.begin(), ::tolower);
			//cout << "*" << w << "*";
			wordCount++;
			for(int i = 0; i < stopCount; i++)
			{
				if(stopList[i] == w)
				{
					stopPresent = true;
					//cout << "Stop True\n";
				}
			}
			for(int i = 0; i < wordListCounter; i++)
			{
				if(w == wordList[i].substr(0, wordList[i].find_first_of("#")))
				{
					wordPresent = true;
					string p = "#" + to_string(pathNum) + "$" + to_string(lines) + "%";
					wordList[i] = wordList[i] + p;
					//cout << "Present True\n";
				}
			}
			if(wordPresent == false && stopPresent == false)
			{
				/*if(!w.empty() && w[w.size()-1] == '\r')
				{
					w.erase(w.size()-1);
				}*/
				w += "#";
				w += to_string(pathNum);
				w += "$";
				w += to_string(lines);
				w += "%";
				wordList.push_back(w);
				cout << wordListCounter << endl;
				wordListCounter++;
			}
			for(int i = 0; i < wordListCounter; i++)
			{
				//cout << "Word is: " << w << endl;
				//cout << "Vector position " << i << " is: " << wordList[i] << endl;
			}
			//cin.get();
		}
		//cout << endl << line << endl;
    }

    infile.close();
  }catch(ifstream::failure e){
    //cout<<e<<endl;
  }
  return count;
}
