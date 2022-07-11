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
string urlForListStudents = "adminVal.cgi";
string accountID;
void printHTML();
form_iterator password;
form_iterator email;
int main(int argc, char** argv) {
	try {
		
		Cgicc cgi;
		inputV iv;
		password = cgi.getElement("password");
		email = cgi.getElement("email");
		iv.isLoggedAdmin(cgi,url,accountID,password,email);
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
//	cout << HTTPHTMLHeader().setCookie(HTTPCookie("count", "1"));
	// !<DOCTYPE HTML PUBLIC ....>, HTML 4.0 is used
	cout << HTMLDoctype(HTMLDoctype::eStrict) << endl;
	// We can use cgicc classes to output HTML which is more readable incode, 
	// but Direct HTML is easier to type
	cout << html() << endl; // Same as <html>
	cout << "<h1> Admin Portal </h1> \n\n" << endl;
	db.adminView();
	cout << "<br><h4>Assign Modules</h4>" << endl;
	cout << "<form method = 'post' action = '"<< urlForListStudents<<"' enctype = 'multipart/form-data'>" << endl;
	cout << "Enter Module ID : <input type = 'text' name = 'moduleName' MAXLENGTH = '255' / > " << endl;
	cout << "Enter ID : <input type = 'text' name = 'ID' MAXLENGTH = '255' / ><br / > " << endl;
	//cout << "Change for student or  : <input type = 'checkbox' name = 'isAdmin' / >" << endl;
	cout << "<input type = 'checkbox' name = 'teacher'  checked = 'checked' / > Lecturer" << endl;
	cout <<	"<input type = 'checkbox' name = 'student' / > Student" << endl;
	cout << "<input type = 'submit' value = 'Assign Module'/ >" << endl;
	cout << " </form>" << endl;
	
	cout << html() << endl; //Same as </html>
}