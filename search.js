/* Project Gutenberg Search*/ 


/*Precondition: Page has not loaded.                                                                                  
**Postcondition: XML object has been created according to user browser.*/
/*function initializeXMLHttp()
{
	console.log("page load");
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
	console.log(XMLHttp.readyState);
	XMLHttp.open("GET", sendStr, true);
	console.log(XMLHttp.readyState);
	XMLHttp.onload=function() //check for response from the cgi                                       
	{
		console.log(XMLHttp.readyState);
		if (XMLHttp.readyState == 4) //the cgi has sent a response                                                    
		{
			console.log("readystate == 4");
			document.getElementById('response').innerHTML = XMLHttp.responseText;    
			//console.log(XMLHttp.responseText);
		}
	}

    XMLHttp.send(null);
}*/



$(document).ready(function () {
    
    $("#search-btn").click(getMatches);
    
    $(document).ajaxStart(function () {
	$("#loading").show();
    }).ajaxStop(function () {
	$("#loading").hide();
    });

});

function getMatches(){
    if ($('#searchWord').val().length < 3) return;
    $('#searchresults').empty();
    $.ajax({
		url: '/cgi-bin/seavera_searchAjax.cgi?searchWord=' + $('#searchWord').val(),
		dataType: 'html',
		success: processResults,
		error: function(){alert("Error: Something went wrong");}
    });

}

function processResults(results)
{
	$('#searchresults').empty();
    $('#searchresults').append(results);
}

function topFunction() //top of page function
{
	if(navigator.appName == "Microsoft Internet Explorer")
        {
        	document.documentElement.scrollTop = 0;     
        }
        else
        {
			document.body.scrollTop = 0;
        }
}
