import java.sql.*;
import java.util.*;

/**
 * The class implements methods of the video and bookstore database
 * interface.
 *
 * All methods of the class receive a Connection object through which all
 * communication to the database should be performed. Note: the
 * Connection object should not be closed by any method.
 *
 * Also, no method should throw any exceptions. In particular, in case
 * that an error occurs in the database, then the method should print an
 * error message and call System.exit(-1);
 */
public class StoreApplication {

	/**
	 * Return a list of phone numbers of customers, given a first name and
	 * last name.
	 */

	// Helper function to call when catching errors. Prints error message and terminates.
	public void SQLError(SQLException e){
		System.out.println("Error caught: ");
		System.out.println(e.getMessage());
		System.exit(-1);
	}

	public List<String> getCustomerPhoneFromFirstLastName(Connection connection,
			String firstName, String lastName) {

		List<String> result = new ArrayList<String>();	
		PreparedStatement pstmt = null;
      ResultSet rs = null;
		String query = "SELECT d.phone FROM dv_address d, mg_customers c WHERE c.address_id = d.address_id AND c.first_name = ? and c.last_name = ?";
		try{
			pstmt = connection.prepareStatement(query);
			pstmt.setString(1, firstName);
			pstmt.setString(2, lastName);
         rs = pstmt.executeQuery();
         while(rs.next()){
            String res = rs.getString(1);
            System.out.println(res);
            result.add(res);
         }
         if(pstmt != null) 
            pstmt.close();
         if(rs != null) 
            rs.close();
		} catch (SQLException e){
			SQLError(e);
		}
		System.out.println(result);
		return result;
	}

	/**
	 * Return list of film titles whose length is is equal to or greater
	 * than the minimum length, and less than or equal to the maximum
	 * length.
	 */
	public List<String> getFilmTitlesBasedOnLengthRange(Connection connection,
			int minLength, int maxLength) {
		List<String> result = new LinkedList<String>();
      PreparedStatement pstmt = null;
      ResultSet rs = null;
		String query = "SELECT title FROM dv_film WHERE length >= ? AND length <= ?";
		try{
			pstmt = connection.prepareStatement(query);
			pstmt.setInt(1, minLength);
			pstmt.setInt(2, maxLength);
         rs = pstmt.executeQuery();
         while(rs.next()){
            String res = rs.getString(1);
            result.add(res);
         }
         if(pstmt != null) 
            pstmt.close();
         if(rs != null) 
            rs.close();
		} catch (SQLException e){
			SQLError(e);
		}
		System.out.println(result);
		return result;
	}

	/**
	 * Return the number of customers that live in a given district and
	 * have the given active status.
	 */
	public final int countCustomersInDistrict(Connection connection,
			String districtName, boolean active) {
		int result = -1;
      PreparedStatement pstmt = null;
      ResultSet rs = null;
		String query = "SELECT COUNT(c.customer_id) FROM mg_customers c, dv_address d WHERE c.address_id = d.address_id AND d.district = ? AND c.active = ?";
		try{
         pstmt = connection.prepareStatement(query);
         pstmt.setString(1, districtName);
         pstmt.setBoolean(2, active);
         rs = pstmt.executeQuery();
         while(rs.next()){
            String res = rs.getString(1);
            System.out.println(res);
         }
         if(pstmt != null) 
            pstmt.close();
         if(rs != null) 
            rs.close();
      } catch (SQLException e){
         SQLError(e);
      }
      return result;
	}

	/**
	 * Add a film to the inventory, given its title, description,
	 * length, and rating.
	 *
	 * Your query will need to cast the rating parameter to the
	 * enumerated type mpaa_rating. Whereas an uncast parameter is
	 * simply a question mark, casting would look like ?::mpaa_rating 
	 */
	public void insertFilmIntoInventory(Connection connection, String
			title, String description, int length, String rating){
      PreparedStatement pstmt = null;
      String update = "INSERT INTO dv_film (title, description, length, rating) VALUES(?, ?, ?, ?::mpaa_rating)";
      try{
         pstmt = connection.prepareStatement(update);
         pstmt.setString(1, title);
         pstmt.setString(2, description);
         pstmt.setInt(3, length);
         pstmt.setString(4, rating);
         pstmt.executeUpdate();
         if(pstmt != null) 
            pstmt.close();
      } catch (SQLException e){
         SQLError(e);
      }
	}

	/**
	 * Constructor
	 */
	public StoreApplication()
	{}

};
