#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>

void finish_with_error(MYSQL* con)
{
    fprintf(stderr, "%s\n", mysql_error(con));
    mysql_close(con);
    exit(1);
}
void run_query(MYSQL* con, const char* q) {
    if (mysql_query(con, q))
        finish_with_error(con);
};

void runQueries(MYSQL* con);
void createTables(MYSQL* con);
void printRows(MYSQL* con);

int main1(int argc, char** argv)
{
    MYSQL* con = mysql_init(NULL);
    // Check if initialized successfully
    if (con == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        exit(1);
    }
    // Make connection to the MySQL server
    // localhost is the address with port 3306 and root is username
    // & password is empty, we connected to table
    if (mysql_real_connect(con, "localhost", "root", "",
        "testdb", 3306, NULL, 0) == NULL)
    {
        finish_with_error(con);
    }
    
    createTables(con);

    //runQueries(con);
    printRows(con);
    mysql_close(con);
    exit(0);
}

/*OverView Tables:
   Accounts
   Students
   Modules


*/
void createTables(MYSQL* con) 
{
    // Drop Tables if they exists so we can start new
    mysql_query(con, "DROP TABLE IF EXISTS Lecturers, Students, Modules, Modules_Students");

    // Account Table contains Lecturers and 1 Administrator
    const char* query = "CREATE TABLE Lecturers (id INT PRIMARY KEY AUTO_INCREMENT,"
        "firstName VARCHAR(255) NOT NULL, lastName VARCHAR(255) NOT NULL, email VARCHAR(255)"
        " NOT NULL, password VARCHAR(255) NOT NULL)";
    run_query(con, query);

    query = "CREATE TABLE Students (id INT PRIMARY KEY AUTO_INCREMENT,"
        "firstName VARCHAR(255), lastName VARCHAR(255))";
    run_query(con, query);

    query = "CREATE TABLE Modules (id INT PRIMARY KEY AUTO_INCREMENT,"
        "moduleName VARCHAR(255))";
    run_query(con, query);

    // RelationShip Tables
    query = "CREATE TABLE ModulesStudents (moduleID INT NOT NULL, studentID INT NOT NULL,mark INT,"
        "FOREIGN KEY(moduleID) REFERENCES Modules(id) ,"
        "FOREIGN KEY(studentID) REFERENCES Students(id)) ";
    run_query(con, query);

    query = "CREATE TABLE ModulesLecturers (lecturerID INT  NOT NULL, moduleID INT NOT NULL,"
        "FOREIGN KEY(lecturerID) REFERENCES Lecturers(id) ,"
        "FOREIGN KEY(studentID) REFERENCES Modules(id)) ";
    run_query(con, query);

}

void insertQueries(MYSQL* con) 
{

    run_query(con, "INSERT INTO Lecturers (firstName, lastName) VALUES('Ian','Baylay')");
    run_query(con, "INSERT INTO Students (firstName, lastName) VALUES('Jack','Bro')");
    run_query(con, "INSERT INTO Students (firstName, lastName) VALUES('Heinz','Bro')");
    run_query(con, "INSERT INTO Modules (moduleName) VALUES('Differential Calculus')");
    run_query(con, "INSERT INTO ModuleStudents VALUES(1,1,80)");
    run_query(con, "INSERT INTO ModuleStudents VALUES(1,2,40)");
    
}

void searchQueries(MYSQL* con, char* moduleID) 
{   
    //run_query(con, "SELECT * FROM Modules");
    
    // Display Students for a Module
    // 1. Joins the Module Table and Students Table
    // 2. Finds the Module by ID
    // 3. Outputs columns firstName lastName and mark
    run_query(con, "SELECT Students.firstName Students.lastName ModulesStudents.mark "
        "FROM ModulesStudents ms"
        "JOIN Students s ON ms.studentID = s.id"
        "WHERE ms.moduleID ='1'");
}

void runQueries(MYSQL* con) 
{
    insertQueries(con);
}





void printRows(MYSQL* con) 
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
        printf("\n");
    }

    mysql_free_result(result);
}

void testTables() {
    /*
    * //mysql_query(con, "DROP TABLE IF EXISTS cars");
    // TEST MANY TO MANY
    query = "CREATE TABLE cars (id INT PRIMARY KEY AUTO_INCREMENT,"
        "name VARCHAR(255), price INT)";
    run_query(con, query);

    query = "CREATE TABLE rentee (id INT PRIMARY KEY AUTO_INCREMENT,"
        "name VARCHAR(255))";
    run_query(con, query);

    query = "CREATE TABLE carsRentee (renteeID INT, carID INT ,"
        "FOREIGN KEY(renteeID) REFERENCES rentee(id), FOREIGN KEY(carID) REFERENCES cars(id))";
    run_query(con, query);
    */

}

void runQueriesTest(MYSQL* con) {

    run_query(con, "INSERT INTO cars (name,price) VALUES('Audi',52642)");
    run_query(con, "INSERT INTO cars (name,price) VALUES('BMW',62642)");
    run_query(con, "INSERT INTO rentee (name) VALUES('Michail')");
    run_query(con, "INSERT INTO rentee (name) VALUES('Ailu')");
    run_query(con, "INSERT INTO carsRentee (renteeID,carID) VALUES(1,1)");
    run_query(con, "INSERT INTO carsRentee (renteeID,carID) VALUES(1,6)");


    //run_query(con, "SELECT * FROM cars WHERE name='Audi'");
    // We append them in this order:  rentee + carsRentee + cars
    run_query(con, "SELECT * FROM rentee r "
        "JOIN carsRentee cr ON r.id = cr.renteeID "
        "JOIN cars c ON cr.carID = c.id "
        "WHERE c.name='Audi'");

}