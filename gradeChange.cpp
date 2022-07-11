#include <cgicc/Cgicc.h> 
#include <cgicc/HTTPHTMLHeader.h> 
#include <cgicc/HTMLClasses.h>  // used for html() for example
#include <cgicc/HTTPRedirectHeader.h> // Used to just write redirect eg. Location: url
#include <string>
#include <iostream>
#include "dbSQL.h"
#include "inputValidation.h"

using namespace cgicc;
using namespace std;

DBSQL db;
string url = "loginPage.cgi";
string studentID;
string extractedText;
string newMark;
void printHTML();
;
int main(int argc, char** argv) {

	try {
		
		Cgicc cgi;
		inputV iv;
		form_iterator mName = cgi.getElement("moduleName");
		form_iterator sID = cgi.getElement("studentID");
		form_iterator nMark = cgi.getElement("mark");
		iv.validateTextField(cgi,extractedText,mName,url);
		iv.validateNumField(cgi,studentID,sID,url);
		iv.validateNumField(cgi,newMark,nMark,url);
		
		printHTML();
		
	}

	catch (exception& e) {
		// Caught a standard library exception
	}

	return 0;
}


void printHTML() {

	// HTTP Header which outputs Content-Type: text/html\n\n 
	cout << HTTPHTMLHeader() << endl;
	// !<DOCTYPE HTML PUBLIC ....>, HTML 4.0 is used
	cout << HTMLDoctype(HTMLDoctype::eStrict) << endl;
	// We can use cgicc classes to output HTML which is more readable incode, 
	// but Direct HTML is easier to type
	cout << html() << endl; // Same as <html>
	cout << "<h1>  Grade Changer </h1> \n" << endl;
	if (!db.changeMark(studentID,extractedText,newMark)) {
		cout << "<br/><h3 style='color:red;'>MODULE or Student NOT FOUND</h3>" << endl;
	}
	else {
		cout << studentID <<"<b> Mark is now : </b>"<< newMark << " for "<< extractedText << endl;
	}
	
	cout << html() << endl; //Same as </html>
}