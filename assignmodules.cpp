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
string url = "adminlogin.cgi";
string tORsID;
string extractedText;
string newMark;
void printHTML();
bool studentorteacher;
int main(int argc, char** argv) {
	
	try {
		
		Cgicc cgi;
		inputV iv;
		form_iterator isTeacher = cgi.getElement("teacher");
		form_iterator isStudent = cgi.getElement("student");
		//db.addModuleLecturer("1", "1");
		if (!isTeacher->isEmpty() && isTeacher != cgi.getElements().end()) {
			studentorteacher = 1;
		}
		else if (!isStudent->isEmpty() && isStudent != cgi.getElements().end()) {
			studentorteacher = 0;
		}
		form_iterator mName = cgi.getElement("moduleName");
		form_iterator sID = cgi.getElement("ID");

		// validate and get extracted text
		iv.validateNumField(cgi,extractedText,mName,url); 
		iv.validateNumField(cgi,tORsID,sID,url); 

		
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

	cout << "<h1>  Assigned Module </h1> \n" << endl;
	if (studentorteacher) {
		if (!db.addModuleLecturer(tORsID, extractedText)) {
			cout << "<br/><h3 style='color:red;'> Already Assigned</h3>" << endl;
		}
		else {
			cout << "<b> Module Assigned </b>" << endl;
		}
	}
	else if (!studentorteacher) {
		if (!db.addModuleStudent(tORsID, extractedText)) {
			cout << "<br/><h3 style='color:red;'>Already Assigned</h3>" << endl;
		}
		else {
			cout << "<b> Module Assigned </b>" << endl;
		}
	}
	else cout << "<br/><h3 style='color:red;'>Already Assigned</h3>" << endl;
	
	cout << html() << endl; //Same as </html>
}