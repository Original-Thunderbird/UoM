package uk.ac.manchester.cs.comp28112.lab2;

/**
 * An exception for parsing responses from requests (puts and gets). 
 * @author dkuo
 *
 */
public class ParseException extends Exception {

	private static final long serialVersionUID = 1L;

	public ParseException(){super();}
	
	public ParseException(String s) {super(s);}
	
	public ParseException(Exception e){super(e);}
}
