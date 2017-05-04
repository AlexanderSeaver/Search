/* Project Gutenberg Search*/ 


/*Precondition: Page has not loaded.                                                                                  
**Postcondition: XML object has been created according to user browser.*/
function initializeXMLHttp()
{
        if(navigator.appName == "Microsoft Internet Explorer")
        {
                XMLHttp = new ActiveXObject("Microsoft.XMLHTTP"); //if the user is using IE                           
        }
        else
        {
                XMLHttp = new XMLHttpRequest(); //this part is entered using any browser other than IE                
        }
}


function getBasicResults() //get searchWord, input book numbers and lines into arrays
{
	var searchWord = document.getElementById('searchWord').value;
	var sendStr = "/cgi-bin/seavera_searchAjax.cgi?" + "&searchWord=" + searchWord;
	XMLHttp.open("GET", sendStr, true);

	XMLHttp.onreadystatechange=function() //check for response from the cgi                                       
	{
	if (XMLHttp.readyState == 4) //the cgi has sent a response                                                    
			{
				  document.getElementById('response').innerHTML = XMLHttp.responseText;    
	
			}
	}

    XMLHttp.send(null);

}
