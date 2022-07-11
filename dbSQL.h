#pragma once
#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <list>
#include <iostream>
using namespace std;

// Small interface like class
class DBSQL {
    MYSQL* con;
    char currentDatabase[255] = "testdb";
    char address[255] = "localhost";
    char user[255] = "root";
    char pw[255] = ""; // No password
    int port = 3306;

    void createConnection(bool noDB = 0) {
        con = mysql_init(NULL);
        // Check if initialized successfully
        if (con == NULL)
        {
            fprintf(stderr, "%s\n", mysql_error(con));
            exit(1);
        }
        // Make connection to the MySQL server
        // localhost is the address with port 3306 and root is username
        // & password is empty, we connected to table
        
   
        if (noDB) {
            if (mysql_real_connect(con, address, user, pw,
                NULL, port, NULL, 0) == NULL) finish_with_error(con);
        }
        else {
            if (mysql_real_connect(con, address, user, pw,
                currentDatabase, port, NULL, 0) == NULL) finish_with_error(con);
        }
    }
    void closeConnection() {
        mysql_close(con);
        //exit(0);
    }
    void insertAtBracket(string &s, string input) {

        for (int i = 0; i+1 < s.length(); i++) {
            if (s[i] == '{' && s[i + 1] == '}') {
                s.erase(i, 2); // remove brackets
                s.insert(i, input); // add input string instead
                break;
            }
        }
        
    }
    void finish_with_error(MYSQL* con)
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        exit(1);
    }
    void run_query(string q) {
        const char* query = stringToChars(q);
        if (mysql_query(con, query))
            finish_with_error(con); 
    };
    char* stringToChars(string str) {
        char arr[255];
        strcpy_s(arr, str.c_str());
        return arr;
    }
    void printRows()
    {
        // Get result of query
        MYSQL_RES* result = mysql_store_result(con);
        if (result == NULL)
            finish_with_error(con);

        int num_fields = mysql_num_fields(result);
        MYSQL_ROW row; // 2D array of strings

        // Loop through all the rows
        while ((row = mysql_fetch_row(result)))
        {
            // Print all columns
            for (int i = 0; i < num_fields; i++)
            {
                printf("%s ", row[i] ? row[i] : "NULL");
            }
            printf("\n<br>");
        }

        mysql_free_result(result);
    }
    void strlower(string& s) {
        for (int i = 0; i < s.length(); i++)
            s[i] = tolower(s[i]);
    }

public:
    
    void initDatabase() {
        createConnection(1);
        run_query("DROP DATABASE IF EXISTS testdb");
        run_query("CREATE DATABASE testdb");
        closeConnection();
        
        createConnection();
        // Drop Tables if they exists so we can start new
        run_query("DROP TABLE IF EXISTS Admin,Lecturers, Students, Modules, ModulesStudents, ModulesLecturers ");

        // Account Table contains Lecturers and 1 Administrator
        run_query("CREATE TABLE Admin (id INT PRIMARY KEY AUTO_INCREMENT,"
            "firstName VARCHAR(255) NOT NULL, lastName VARCHAR(255) NOT NULL, email VARCHAR(255)"
            "NOT NULL, password VARCHAR(255) NOT NULL)");

        run_query("CREATE TABLE Lecturers (id INT PRIMARY KEY AUTO_INCREMENT,"
            "firstName VARCHAR(255) NOT NULL, lastName VARCHAR(255) NOT NULL, email VARCHAR(255)"
            " NOT NULL, password VARCHAR(255) NOT NULL)");

        run_query("CREATE TABLE Students (id INT PRIMARY KEY AUTO_INCREMENT,"
            "firstName VARCHAR(255), lastName VARCHAR(255))");

        run_query("CREATE TABLE Modules (id INT PRIMARY KEY AUTO_INCREMENT,"
            "moduleName VARCHAR(255))");

        run_query("CREATE TABLE AccessCodes (code INT,"
            "email VARCHAR(255))");

        // RelationShip Tables
        run_query("CREATE TABLE ModulesStudents (moduleID INT NOT NULL, studentID INT NOT NULL,mark INT NOT NULL,"
            "FOREIGN KEY(moduleID) REFERENCES Modules(id) ,"
            "FOREIGN KEY(studentID) REFERENCES Students(id)) ");

        run_query("CREATE TABLE ModulesLecturers (moduleID INT NOT NULL, lecturerID INT  NOT NULL,"
            "FOREIGN KEY(lecturerID) REFERENCES Lecturers(id) ,"
            "FOREIGN KEY(moduleID) REFERENCES Modules(id)) ");
        closeConnection();
    }

    void initWithTestData() {
        createConnection();
        // Modules
        string ModuleNames[] = {"Mathematics","Artificial Intelligence",
            "Internet of Things","Secure Programming","Secure Operating Systems"};
        for (string s : ModuleNames) {
            string q = "INSERT INTO Modules(ModuleName) VALUES('{}')";
            insertAtBracket(q,s);
            run_query(q);
        }
        
        string Students [][3] = { 
            {"George","Baldy"},
            {"Aiden","Faiden"},
            {"Jilen","Milen"},
            {"Faye","Milen"}
        };
       
        for (string* arr : Students) {
            string q = "INSERT INTO Students(firstName,lastName) VALUES('{}','{}')";
            insertAtBracket(q, arr[0]);
            insertAtBracket(q, arr[1]);
            run_query(q);
        }
        
        // Add 1 Lecturer
        run_query("INSERT INTO lecturers (firstName,lastName,email,password)"
            " VALUES('Ian','Baylay','ian@gmail.com','Helloworld1')");

        // Fill empty columns with random values
        run_query("INSERT INTO ModulesStudents VALUES(1,1,80)");
        run_query("INSERT INTO ModulesStudents VALUES(2,1,50)");
        run_query("INSERT INTO ModulesStudents VALUES(3,3,80)");
        run_query("INSERT INTO ModulesStudents VALUES(3,1,20)");
        
        // Add modules to first lecturer in DB
        run_query("INSERT INTO ModulesLecturers VALUES(3,1)");
        run_query("INSERT INTO ModulesLecturers VALUES(2,1)");
        run_query("INSERT INTO ModulesLecturers VALUES(1,1)");
        
        closeConnection();
    }

    bool loginCheckQuery(string email, string password, string &accountID,bool isAdmin = 0) {
        createConnection();
        string s;
        if (isAdmin)  s = "SELECT id FROM Admin WHERE email = '{}' AND password = '{}'"; 
        else s = "SELECT id FROM Lecturers WHERE email = '{}' AND password = '{}'";
        insertAtBracket(s, email);
        insertAtBracket(s, password);
        run_query(s);

        MYSQL_RES* result = mysql_store_result(con);
        mysql_free_result(result);
        if (result == NULL) finish_with_error(con);

        MYSQL_ROW row;
        row = mysql_fetch_row(result);
        if (row) accountID = row[0];
        closeConnection();

        // Return if Login was Successful
        return row ? 1: 0;
    }

    void adminView() {
        createConnection();
        run_query("SELECT * FROM Modules");
        cout << "\n\n<b>ID Modules</b><br>\n\n";
        printRows();
        run_query("SELECT id, firstName, lastName FROM Lecturers");
        cout << "\n\n<b>ID Lecturers</b><br>\n\n";
        printRows();
        run_query("SELECT id, firstName, lastName FROM Students");
        cout << "\n\n<b>ID Students</b><br>\n\n";
        printRows();
        closeConnection();
    }
    bool listLecturersforModules(string LecturerID) {
        createConnection();
        // Check if ID exists
        string s = "SELECT id FROM Lecturers WHERE id = '{}'";
        MYSQL_ROW row;
        if (!findInDatabase(s, LecturerID, row)) return 0;
        // Display Lecturers Modules
        // 1. Joins the Module Table and ModuleLecturer Table
        // 2. Finds the Module by ID
        // 3. Outputs Module Name
        s = "SELECT m.moduleName "
            "FROM ModulesLecturers ms "
            "JOIN Modules m ON ms.moduleID = m.id "
            "WHERE ms.lecturerID ='{}'";
        insertAtBracket(s, row[0]);
        run_query(s);
        // Print to website
        cout << "<h3>Modules</h3>\n";
        printRows();
        closeConnection();
        return 1;
    }

    bool findInDatabase(string s, string toFind, MYSQL_ROW &row) {
        insertAtBracket(s,toFind);
        run_query(s);
        MYSQL_RES* result = mysql_store_result(con);
        if (result == NULL)
            finish_with_error(con);
        row = mysql_fetch_row(result);
        // When nothing found return 0
        if (!row) return 0;
        // something Found return 1
        return 1; 
    }
    // If module Name exists it prints all the students and their marks
    bool listStudentsForModule(string moduleName) {
        
        createConnection();
        // Get Module ID
        string s = "SELECT id FROM Modules WHERE moduleName = '{}'";
        MYSQL_ROW row;
        if (!findInDatabase(s, moduleName,row)) return 0;
        // Display Students for a Module
        // 1. Joins the Module Table and Students Table
        // 2. Finds the Module by ID
        // 3. Outputs columns firstName lastName and mark
        s = "SELECT ms.studentID, s.firstName, s.lastName, ms.mark "
            "FROM ModulesStudents ms "
            "JOIN Students s ON ms.studentID = s.id "
            "WHERE ms.moduleID ='{}'";
        insertAtBracket(s, row[0]);
        run_query(s);
        // Print to website
        cout << "<b>ID Name Mark</b><br>\n";
        printRows();
        closeConnection();
        return 1;
    }

    bool changeMark(string studentID, string moduleName, string newMark) {
        
        createConnection();
        // Check if student ID in Database
        string s = "SELECT id FROM Students WHERE id = '{}'";
        MYSQL_ROW row;
        if (!findInDatabase(s, studentID, row)) return 0;
        
        // Get Module ID
        s = "SELECT id FROM Modules WHERE moduleName = '{}'";
        if (!findInDatabase(s, moduleName, row)) return 0;
        
        // Make query to Change Mark 
        s = "UPDATE ModulesStudents SET mark = '{}' WHERE studentID = '{}' AND moduleID = '{}'";
        insertAtBracket(s, newMark);
        insertAtBracket(s, studentID);
        insertAtBracket(s, row[0]);
        run_query(s);
        closeConnection();
    }
    bool accountRegister(string fName, string lName,string email,string pw, bool isAdmin) {
        createConnection();
        // Find email if it exists then user has already account
        string s = "SELECT email FROM Lecturers WHERE email = '{}' ";
        MYSQL_ROW row;
        if (findInDatabase(s, email,row)) return 0;
        

        // Otherwise Create new Account
        if (isAdmin) {
            // IF admin already exists return as we can only have 1 admin
            s = "SELECT id FROM Admin WHERE id = 1";
            if (findInDatabase(s, email, row)) return 0;
            s = "INSERT INTO Admin VALUES(1,'{}','{}','{}','{}')";
        }
        else
            s = "INSERT INTO Lecturers (firstName, lastname,email,password)VALUES('{}','{}','{}','{}')";
        insertAtBracket(s, fName);
        insertAtBracket(s, lName);
        insertAtBracket(s, email);
        insertAtBracket(s, pw);
        run_query(s);
        closeConnection();
    }
    // for the add modules i am not checking if the module id or lecturer id exists in database yet.
    // so when nothing is found the webpage will just be empty
    bool addModuleLecturer(string lecturerID, string moduleID) {
        createConnection();
        // Find if module already assigned
        string s = "SELECT moduleID FROM ModulesLecturers WHERE  lecturerID ='{}' AND moduleID = '{}' ";
        MYSQL_ROW row;
        insertAtBracket(s, lecturerID);
        // if nothing is found then continue otherwise return 0
        if (findInDatabase(s, moduleID, row)) return 0; 

        // Assign Module to Lecturer
        s = "INSERT INTO ModulesLecturers VALUES('{}','{}')";
        insertAtBracket(s, moduleID);
        insertAtBracket(s, lecturerID);
        run_query(s);
         
        closeConnection();
    }

    bool addModuleStudent(string studentID, string moduleID, string mark = "0") {
        createConnection();
        // Find if module already assigned
        string s = "SELECT moduleID FROM ModulesStudents WHERE  studentID ='{}' AND moduleID = '{}' ";
        MYSQL_ROW row;
        insertAtBracket(s, studentID);
        if (findInDatabase(s, moduleID, row)) return 0;

        // Assign Module to Lecturer
        s = "INSERT INTO ModulesStudents VALUES('{}','{}','{}')";
        insertAtBracket(s, moduleID);
        insertAtBracket(s, studentID);
        insertAtBracket(s, mark);
        run_query(s);

        closeConnection();
    }
    bool findAccessCode(string code, string email) {
        createConnection();
        // Find if access code already created
        // Eventually we have used up all access codes but an expiry tool can delete unused/used codes
        string s = "SELECT code FROM AccessCodes WHERE  email ='{}' AND code = '{}' ";
        MYSQL_ROW row;
        insertAtBracket(s, email);
        if (!findInDatabase(s, code, row)) return 0; // If nothing found
        else return 1;
        closeConnection();
    }
    bool addAccessCode(string code, string email) {
       
        createConnection();
     
        // Find if access code already created
        // Eventually we have used up all access codes but an expiry tool can delete unused/used codes
        string s = "SELECT code FROM AccessCodes WHERE  email ='{}' AND code = '{}' ";
        MYSQL_ROW row;
        insertAtBracket(s, email);
        if (findInDatabase(s, code, row)) return 0;

        // Assign Module to Lecturer
        s = "INSERT INTO AccessCodes VALUES('{}','{}')";
        insertAtBracket(s, code);
        insertAtBracket(s, email);
        run_query(s);

        closeConnection();
    }
};


