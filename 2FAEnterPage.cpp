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
string url = "loginPagewith2FA.cgi";
string urlSuccess = "2faPage.cgi";
string accountID;
void printHTML();
form_iterator password;
form_iterator email;
string sendEmail();
int main(int argc, char** argv) {

	try {
		
		Cgicc cgi;
		inputV iv;
		password = cgi.getElement("password");
		email = cgi.getElement("email");
		iv.isLogged(cgi,url,accountID,password,email);
		// Once Logged in send email with access code,
		// And add to database
		string accessCode = sendEmail();
		if (!db.addAccessCode(accessCode, **email)) {
			cout << HTTPRedirectHeader(url);
		}
		printHTML();
		
	}

	catch (exception& e) {
		// Caught a standard library exception
	}

	return 0;
}
string sendEmail() {
	
	// Used to simulate mailbox for received emails 
	ofstream outfile;

	// append instead of overwrite
	outfile.open("mailSpool", ios_base::app);

	// generating a random number
	// Use current time as seed for random generator
	srand(time(NULL));
	string accessCode = "";
	// create random 6 digit number
	for (int i = 0; i < 6; i++)
		accessCode += to_string(rand() % 10); //range 0 to 9 
	// Then write to file
	outfile << **email << " Your AccessCode is: " << accessCode << "\n";
	return accessCode;
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
	cout << "<form method = 'post' action = '"<< urlSuccess <<"' enctype = 'multipart/form-data'>" << endl;
	cout << "Email : <input type = 'email' name = 'email' MAXLENGTH = '255' / ><br/> " << endl;
	cout << "<h3>AccessCode sent to"<<**email <<"Re-enter the same login details again. </h3><br/>";
	cout << "Password : <input type = 'password' name = 'password' MAXLENGTH = '255' / ><br / >" << endl;
	cout << "Enter 2FA Access Key : <input type = 'text' name = 'accessKey' MAXLENGTH = '255' / ><br / > " << endl;
	cout << "<input type = 'submit' value = 'Submit'/ >" << endl;
	cout << " </form>" << endl;
	cout << html() << endl; //Same as </html>
}