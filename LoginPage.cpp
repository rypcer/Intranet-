
#include <cgicc/Cgicc.h> 
#include <cgicc/HTTPHTMLHeader.h> 
#include <cgicc/HTMLClasses.h>  // used for html() for example
#include <cgicc/HTTPRedirectHeader.h> // Used to just write redirect eg. Location: url
#include <iostream>
#include <string>
#include <mysql/mysql.h>

using namespace cgicc;
using namespace std;


void printHTML();
string redirectSite = "AccountPage.cgi";
int error = 0;
int main(int argc, char** argv) {
	try {
		

		cgicc::Cgicc cgi;
		cgicc::form_iterator errorCode = cgi.getElement("error");
		
		if (errorCode != cgi.getElements().end()) {
			error = (**errorCode)[0] - '0'; // get only first char
			cout << (**errorCode)[0];
		}
		
		// After we have done our validation and everything is correct we can print Site
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
	
	/*
		With get we can see data of form are leaked : / Helloworld.cgi ? name = asd & age = asd & sex = male
		With post its not leaked : / Helloworld.cgi
	*/
	cout << body() << endl;
	cout << "<div>" << endl;
	cout << "<h1>LOGIN</h1>" << endl;
	cout << "<form method = 'post' action = '"<<redirectSite <<"' enctype = 'multipart/form-data'>" << endl;
	//cout << "Your Username : <input type = 'text' name = 'name' MAXLENGTH = '255' / ><br / > " << endl;
	cout << "Email : <input type = 'email' name = 'email' MAXLENGTH = '255' / ><br/> " << endl;
	cout << "Password : <input type = 'password' name = 'password' MAXLENGTH = '255' / ><br / >" << endl;
	//cout << "Your sex : <input type = 'radio' name = 'sex' value = 'male' checked = 'checked' / >Male" << endl;
	//cout << "<input type = 'radio' name = 'sex' value = 'female'/ >Female <br / >" << endl;
	cout << "<input type = 'submit' value = 'Submit Info'/ >" << endl;
	cout << " </form>" << endl;
	cout << "<a href = 'registerPage.cgi'> Not registered yet? </a>" << endl;
	if (error == 1) {
		cout << "<br/><h3 style='color:red;'>INVALID INPUT</h3>" << endl;
	}
	else if (error == 2) {
		cout << "<br/><h3 style='color:red;'>PLEASE FILL OUT FIELDS</h3>" << endl;
	}
	else if (error == 3) {
		cout << "<br/><h3 style='color:red;'>WRONG LOGIN DETAILS </h3>" << endl;
	}
	else if (error == 4) {
		cout << "<br/><h3 style='color:green;'> Successfully Registred Account!</h3>" << endl;
	}
	
	cout << body() << endl;

	cout << html() << endl; //Same as </html>
}