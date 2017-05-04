#MakeFile to build and deploy SearchBooks
# For CSC3004 Software Development

# Put your user name below:
USER= seavera

CC= g++ -std=c++11

#For Optimization
#CFLAGS= -O2
#For debugging
CFLAGS= -std=c++11

RM= /bin/rm -f

all: testSearch readWordFile searchAjax PutCGI PutHTML

readWordFile.o: readWordFile.cpp fifo.h
	$(CC) -c $(CFLAGS) readWordFile.cpp
	
readWordFile: readWordFile.o fifo.o
	$(CC) $(CFLAGS) readWordFile.o fifo.o -o readWordFile -L/usr/local/lib -lcgicc
	
searchAjax.o: searchAjax.cpp fifo.h
	$(CC) -c $(CFLAGS) searchAjax.cpp
	
fifo.o:		fifo.cpp fifo.h
		g++ -c fifo.cpp
	
searchAjax: searchAjax.o fifo.o
	$(CC) $(CFLAGS) searchAjax.o fifo.o -o searchAjax -L/usr/local/lib -lcgicc

testSearch.o: testSearch.cpp fifo.h
	$(CC) -c testSearch.cpp 

testSearch: testSearch.o fifo.o
	$(CC) testSearch.o fifo.o -o testSearch	

PutCGI:
	chmod 757 searchAjax
	cp searchAjax /usr/lib/cgi-bin/$(USER)_searchAjax.cgi 

	echo "Current contents of your cgi-bin directory: "
	ls -l /usr/lib/cgi-bin/

PutHTML:
	cp search.html /var/www/html/class/softdev/$(USER)
	cp search.js /var/www/html/class/softdev/$(USER)
	cp search.css /var/www/html/class/softdev/$(USER)

	echo "Current contents of your HTML directory: "
	ls -l /var/www/html/class/softdev/$(USER)

clean:
	rm -f *.o SearchBooks testSearch
