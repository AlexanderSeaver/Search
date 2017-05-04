/*************************************************************/
/* Project Gutenberg Search CGI program                      */
/*************************************************************/

#include <iostream>
// Stuff for AJAX                                                                                                     
#include "cgicc/Cgicc.h"
#include "cgicc/HTTPHTMLHeader.h"
#include "cgicc/HTMLClasses.h"

//Stuff for pipes                                                                                                     
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <string>
#include "fifo.h"

using namespace std;
using namespace cgicc; // Needed for AJAX functions.  

int main()
{
	cout << "Content-Type: text/plain\n\n"; //Properly formats the output strings
	
	//Pipe Information
	string searchWordPipe_ServerToAjax = "searchWord_server_to_ajax";
    string searchWordPipe_AjaxToServer = "searchWord_ajax_to_server";
	
	//Get search word from webpage
	Cgicc cgiWord; // Ajax object                                                                       
    form_iterator word = cgiWord.getElement("searchWord");
    string searchWord = **word;
    
    //send back to webpage
    //cout << searchWord;
    
    //Make Fifos
	Fifo searchWordFifo_ServerToAjax(searchWordPipe_ServerToAjax);
	Fifo searchWordFifo_AjaxToServer(searchWordPipe_AjaxToServer);
	
	searchWordFifo_AjaxToServer.openwrite();
	searchWordFifo_AjaxToServer.send(searchWord);
	
	string result =" ";
	searchWordFifo_ServerToAjax.openread();
	while (result != "$END")
	{
		result = searchWordFifo_ServerToAjax.recv();
		if (result !="$END")
		{
			string bookResult = result.substr(0, result.find_first_of("!"));
			string textResult = result.substr(result.find_first_of("!") + 1, (result.size() - (result.find_first_of("!") + 1)));
			cout << bookResult << "<br>" << textResult << "<br><br>";
			if(textResult.find("<i>") != string::npos)
			{
				cout << "</i>";
			}
			if(textResult.find("<b>") != string::npos)
			{
				cout << "</b>";
			}
			if(textResult.find("<u>") != string::npos)
			{
				cout << "</u>";
			}
		}
	}
	searchWordFifo_ServerToAjax.fifoclose();
	searchWordFifo_AjaxToServer.fifoclose();
	
}