#pragma once
#include <cgicc/Cgicc.h> 
#include <cgicc/HTTPHTMLHeader.h> 
#include <cgicc/HTMLClasses.h>  // used for html() for example
#include <cgicc/HTTPRedirectHeader.h> // Used to just write redirect eg. Location: url
#include <string>
#include <iostream>
//#include <regex>
// crt_dupenv_s.c
//#include <stdlib.h>
//#include <stdio.h>
using namespace cgicc;
using namespace std;


class inputV {
	DBSQL db;
public:
	
	// 1 Means Invalid Input
	// 2 Means its Empty
	// 3 wrong login details
	void redirectSite(int errorID, string url) {
		if (errorID > 0) {
			url += "?error=";
			url += (errorID + '0'); // Convert to char and append
		}
		// WARNING: If you have HTTP redirect call after HTTPHTMLHEADER written then it will be converted into 
		// text/html so the redirect won't work as its part of the html website now.
		cout << HTTPRedirectHeader(url) << "\n"; // Same as "Location: LoginPage.html"
	}
	void validateEmailField(Cgicc cgi, form_iterator text,string &url) {
		// Get Path of current URL from envirnoment variables
		//size_t len;
		//errno_t err = _dupenv_s(&currentPathURL, &len, "SCRIPT_NAME");
		//if (err) redirectSite(0); 
		if (text != cgi.getElements().end()) {

			string extractedText = **text;

			// Check if String is empty 
			if (extractedText.empty())
				redirectSite(2, url);
			// HTML input box already truncates the length
			// So its not needed here
			// Check that input size doesnt exceed 255 chars
			/*else if (extracted_name.size() >= 255)
				redirectError(3);*/

			// HTML already has regex pattern checker I wasted my time doing this
			// VULNERABILITY:: Check if String doesn't contain injection code
			// Get extracted RegexCode from File 
			// Regex code couldn't be written as inline string thats why it's inside a txt
			/*
			string code;
			ifstream myfile("regexCode.txt");
			if (myfile.is_open())
			{
				getline(myfile, code);
				myfile.close();
			}
			const regex pattern(code);
			// try to match the string with the regular expression
			// IF it doesnt match redirect
			if (!regex_match(extracted_name, pattern)) {
				redirectError(1);
			}
			*/
		}
		else { redirectSite(0,url); }
	}
	void validateTextField(Cgicc cgi, string& extractedText,form_iterator text, string &url) {
		// Check if the name iterator points to something, 
		// If user did not enter anything in namefield then we dont run below
		// If iterator not empty / not at the end
		if (text != cgi.getElements().end()) {

			// Because iterator and cgicc::FormEntry overload operator*
			// below can be also used to extract, first * returns FormEntry 
			// second * derefernces iterator so returns of type basic_string
			extractedText = **text;

			// Check if String is empty 
			if (extractedText.size() == 0)
				redirectSite(2, url);
			// Check that input size doesnt exceed 255 chars
			/*else if (extracted_name.size() >= 255)
				redirectError(3);*/

				// VULNERABILITY:: Check if String doesn't contain injection code
			for (char c : extractedText) {
				bool withinAlphabet = c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z' || c == ' ';
				if (!withinAlphabet) {
					redirectSite(1, url);
					break;
				}
			}

		}
		else { redirectSite(0, url); }
	}
	void validateNumField(Cgicc cgi, string& extractedText, form_iterator text, string& url) {
		if (text != cgi.getElements().end()) {
			extractedText = **text;

			// Check if String is empty 
			if (extractedText.size() == 0)
				redirectSite(2, url);
			
			// VULNERABILITY:: Check if String doesn't contain injection code
			for (char c : extractedText) {
				bool isNum = c >= '0' && c <= '9';
				if (!isNum) {
					redirectSite(1, url);
					break;
				}
			}

		}
		else { redirectSite(0, url); }
	}
	void validatePasswordField(Cgicc cgi,form_iterator text, string &url) {
		if (text != cgi.getElements().end()) {

			string extractedText = **text;

			// Check if String is empty 
			if (extractedText.size() == 0)
				redirectSite(2, url);
			// Check that input size doesnt exceed 255 chars
			/*else if (extracted_name.size() >= 255)
				redirectError(3);*/

				// VULNERABILITY:: Check if String doesn't contain injection code
			for (char c : extractedText) {
				bool withinAlphabet = c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z';
				bool isNumerical = c >= '0' && c <= '9';
				char specialChars[] = "!@#$%^&*";
				bool isSpecialChar = false;
				for (char sc : specialChars) {
					if (sc == c) {
						isSpecialChar = true;
						break;
					}
				}
				// One of them has to apply for a valid format
				if (!(withinAlphabet || isNumerical || isSpecialChar)) {
					redirectSite(1, url);
					//cout << "FORMAT NOT VALID\n";
					break;
				}

			}

		}
		else { redirectSite(0, url); }

	}
	void isLogged(Cgicc cgi, string &url, string& accountID, form_iterator password, form_iterator email) {

		validatePasswordField(cgi,password, url);
		validateEmailField(cgi, email, url);
		// Once User Input has been validated
		// Check if login details match with Database
		if (!db.loginCheckQuery(**email, **password, accountID, 0)) {
			if (!db.loginCheckQuery(**email, **password, accountID, 1))
				redirectSite(3, url);
			else
				cout << HTTPRedirectHeader("adminLogin.cgi") << "\n";
		}
		// After successful validations we can give access to our site
	}
	void isLoggedAdmin(Cgicc cgi, string& url, string& accountID, form_iterator password, form_iterator email) {

		validatePasswordField(cgi, password, url);
		validateEmailField(cgi, email, url);
		// Once User Input has been validated
		// Check if login details match with Database
		if (!db.loginCheckQuery(**email, **password, accountID, 1)) {
			redirectSite(3, url);	
		}
		// After successful validations we can give access to our site
	}
};