package uk.ac.manchester.cs.comp28112.lab2;

import java.util.ArrayList;

/**
 * A response that contains a list of slots - request to retrieve available (free) slots and 
 * to slots reserved by a particular user.
 * 
 * @author dkuo
 */
public class Slots extends Response {
	
	ArrayList<Integer> slot_list = null;
	
	/**
	 * Constructor. 
	 * 
	 * @param response_code The response code of the request.
	 * @param response_body The response body in XML.
	 * @param slot_list List of slots
	 */
	public Slots(int response_code, String response_body, ArrayList <Integer> slot_list ){
		super(response_code, response_body);
		this.slot_list = slot_list;
	}
	
	/**
	 * Returns an array of slots
	 * 
	 * @return An array of slots
	 */
	public ArrayList<Integer> getSlots(){
		return this.slot_list;
	}
	
	/**
	 * @return Comma separated list of slots. 
	 */
	
	public String toString(){
		return slot_list.toString();
	}
}
