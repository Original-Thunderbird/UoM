package lab2;

/**
 * An exception when constructing an XML request.
 * 
 * @author dkuo
 *
 */
public class RequestException extends Exception {
	private static final long serialVersionUID = 1L;

	/**
	 * Constructor.  
	 * @param error_msg Error message
	 */
	public RequestException(String error_msg) {
		super(error_msg);
	}
	
	/**
	 * Constructor
	 * @param e An exception
	 */
	public RequestException(Exception e){
		super(e);
	}
}
