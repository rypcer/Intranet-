#include <cgicc/Cgicc.h> 
#include <cgicc/HTTPHTMLHeader.h> 
#include <cgicc/HTMLClasses.h>  // used for html() for example
#include <cgicc/HTTPRedirectHeader.h> // Used to just write redirect eg. Location: url
#include <string>
#include <iostream>
#include "dbSQL.h"
#include "inputValidation.h"
#include <time.h>
using namespace cgicc;
using namespace std;

DBSQL db;
string url = "2FALogin.cgi";

string accountID;
void printHTML();
form_iterator password;
form_iterator email;
form_iterator acode;

int main(int argc, char** argv) {

	try {
		
		Cgicc cgi;
		inputV iv;
		password = cgi.getElement("password");
		email = cgi.getElement("email");
		acode = cgi.getElement("accessKey");
		iv.isLogged(cgi,url,accountID,password,email);
		// Once Logged in send email with access code,
		// And add to database
		if (!db.findAccessCode(**acode, **email)) {
			cout << HTTPRedirectHeader(url);
		}
		
		// Once code has been found we can print page
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
	cout << "<h1>SUCESSFUL LOGIN!</h1>" << endl;
	cout << html() << endl; //Same as </html>
}