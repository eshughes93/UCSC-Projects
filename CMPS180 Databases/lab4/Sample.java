import java.sql.*;
import java.io.*;
import javax.sql.*;

/**
 * A sample class that connects to PostgreSQL and runs a simple query.
 */
public class Sample
{
    Connection conn;
    Statement stmt;

    public Sample() throws ClassNotFoundException, FileNotFoundException, IOException, SQLException
    {
	Class.forName("org.postgresql.Driver");  //Registering the driver
	conn = DriverManager.getConnection("jdbc:postgresql://db.ic.ucsc.edu/your_database_name", "your_login_name" ,"your_database_password" );  //Making the Connection 
	//Your database name is your login name , so for login gaurav the string would look like  
	// "jdbc:postgresql://db.ic.ucsc.edu/gaurav", "gaurav" , "password"
	stmt = conn.createStatement();   //Statement to execute
	ResultSet res = stmt.executeQuery(" Select * from Customers");    //Query to be executed. Results are stored in ResultSet
	if(res!=null)
	    while(res.next())   //Retrieving results from ResultSet
                {
		    //Resultset gives result tuple by tuple
		    // I am retrieving one tuple in this loop
		    String Customer_name = res.getString(1); //Get first attribute in Customer_name
		    System.out.print(Customer_name);
		    String email = res.getString(2); //Get second attribute in email
		    System.out.println("  "+email);
                }
	res.close();  //Closing ResultSet
	stmt.close(); //Closing Statement
	conn.close(); //Closing Connection
    }

    public static void main(String args[])
    {
	try{
	   Sample test = new Sample();
	} catch(Exception exc) {
	    System.err.println("Exception caught.\n " + exc);
	    exc.printStackTrace();
	}
    }
}

