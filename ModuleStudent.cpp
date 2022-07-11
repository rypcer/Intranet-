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
string urlForListStudents = "GradeChange.cgi";
string accountID;
string extractedText;
void printHTML();

int main(int argc, char** argv) {

	try {
		
		Cgicc cgi;
		inputV iv;
		cgicc::form_iterator mName = cgi.getElement("moduleName");
		iv.validateTextField(cgi,extractedText,mName,url);
		
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
	cout << "<h1> Students for Module - "<< extractedText <<"</h1> \n" << endl;
	if (!db.listStudentsForModule(extractedText)) {
		cout << "<br/><h3 style='color:red;'>MODULE NOT FOUND</h3>" << endl;
	}
	else {
		cout << "<h4>Change Grade</h4>" << endl;
		cout << "<form method = 'post' action = '" << urlForListStudents << "' enctype = 'multipart/form-data'>" << endl;
		cout << "Enter Module Name : <input type = 'text' name = 'moduleName' value='"<<extractedText<<"' MAXLENGTH = '255' / ><br / > " << endl;
		cout << "Enter Student ID : <input type = 'text' name = 'studentID' MAXLENGTH = '255' / ><br / > " << endl;
		cout << "Enter NEW MARK : <input type = 'text' name = 'mark' MAXLENGTH = '255' / ><br / > " << endl;
		cout << "<input type = 'submit' value = 'Update Mark'/ >" << endl;
		cout << " </form>" << endl;
	}
	cout << html() << endl; //Same as </html>
}