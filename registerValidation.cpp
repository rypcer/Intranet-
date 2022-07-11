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
string url = "registerpage.cgi";
//string urlsuccess = "accountPage.cgi";
//string urlsuccess2 = "adminPage.cgi";
string placeholder;
void printHTML();
form_iterator password;
form_iterator email;
form_iterator fName;
form_iterator lName;
form_iterator isA;
bool isAdmin = false;

int main(int argc, char** argv) {

	try {
		Cgicc cgi;
		inputV iv;
		password = cgi.getElement("password");
		email = cgi.getElement("email");
		fName = cgi.getElement("firstName");
		lName = cgi.getElement("lastName");
		isA = cgi.getElement("isAdmin");
		iv.validateEmailField(cgi,email,url);
		iv.validatePasswordField(cgi,password,url);
		iv.validateTextField(cgi,placeholder,fName,url);
		iv.validateTextField(cgi,placeholder,lName,url);
		if (!isA->isEmpty() && isA != cgi.getElements().end()) {
			isAdmin = true;
		}
		db.accountRegister(**fName, **lName, **email, **password,isAdmin);
		// Not save to pass in email and password need to add a session access key instead but got no time for now.
		//if (isAdmin) cout << "Location:  "<<urlsuccess2 <<"?email="<<**email <<"&password="<<**password <<"\n";
		//else cout << "Location:  " << urlsuccess << "?email=" << **email << "&password=" << **password << "\n";
		cout << HTTPRedirectHeader("LoginPage.cgi?error=4");
		
	}

	catch (exception& e) {
		// Caught a standard library exception
	}

	return 0;
}

