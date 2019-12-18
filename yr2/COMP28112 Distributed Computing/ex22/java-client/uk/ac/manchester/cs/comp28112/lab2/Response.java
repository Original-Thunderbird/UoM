package uk.ac.manchester.cs.comp28112.lab2;

import java.io.StringReader;
import java.util.ArrayList;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;

import javax.xml.xpath.XPathConstants;
import javax.xml.xpath.XPathExpression;
import javax.xml.xpath.XPathExpressionException;
import javax.xml.xpath.XPathFactory;
import javax.xml.xpath.XPath;

import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.InputSource;

/**
 * Class to process a response message.
 * 
 * @author dkuo
 *
 */
public class Response {
	private static DocumentBuilderFactory documentFactory;

	private static DocumentBuilder documentBuilder;

	private static XPathFactory xPathFactory;

	private static XPath xPath;
	
	public static String MSG_ID_ELEMENT = "msg_uri";
	private static final String MSG_URI_XPATH_EXPR = "/descendant::"+MSG_ID_ELEMENT;
	private static XPathExpression msgIdXPathExpression;

	private static final String CODE_ELEMENT = "code";
	private static final String CODE_XPATH_EXPR = "/descendant::"+CODE_ELEMENT;
	private static XPathExpression codeXPathExpression;
	
	private static final String BODY_ELEMENT = "body";
	private static final String BODY_XPATH_EXPR = "/descendant::"+BODY_ELEMENT;
	private static XPathExpression bodyXPathExpression;
	
	public static String SLOT_ID_ELEMENT = "slot_id";
	private static final String SLOT_EXPR = "/descendant::"+SLOT_ID_ELEMENT;
	private static XPathExpression slotIdXPathExpression;
	
	int response_code;
	String response_body = "";
	
	/**
	 * Constructor. 
	 * 
	 * @param response_code The response code of a response message.
	 */
	public Response(int response_code){
		this.response_code = response_code;
	}
	
	/**
	 * Constructor
	 * 
	 * @param response_code The response code in a response message.
	 * @param response_body The body of a response message.
	 */
	
	public Response(int response_code, String response_body) {
		this.response_code = response_code;
		this.response_body = response_body;
	}
	
	/**
	 * Sets the response body.
	 * 
	 * @param response_body The body of a response message.
	 */
	public void setResponseBody(String response_body){
		this.response_body = response_body;
	}
	
	/**
	 * Gets the response code.
	 * 
	 * @return The response code.
	 */
	public int getResponseCode() {
		return this.response_code;
	}
	
	/**
	 * Gets the response body.
	 * 
	 * @return The body of a response message.
	 */
	public String getResponseBody(){
		return this.response_body;
	}
	
	/**
	 * Gets the response type. 
	 * 
	 * @return The type of response - uk.ac.manchester.cs.comp28112.lab2.Response or uk.ac.manchester.cs.comp28112.lab2.Slots
	 */
	
	public String getResponseType() {
		return this.getClass().getName();
	}
	
	/**
	 * Return a string representation of the object.
	 * 
	 * @return String representation of the response.
	 */
	public String toString() {
		return "response_code: " + this.response_code + "\nresponse_body: " + this.response_body;
	}
	
	/**
	 * Create all the singleton objects for parsing XML and compiling XPath expressions.
	 * 
	 * @throws ParserConfigurationException
	 * @throws XPathExpressionException
	 */
	static private void createBuilder() throws ParserConfigurationException, XPathExpressionException {
		if (documentFactory == null) {
			documentFactory = DocumentBuilderFactory.newInstance();
		}
		
		if (documentBuilder == null) {
			documentBuilder = documentFactory.newDocumentBuilder();
		}
		
		if (xPathFactory == null) {
			xPathFactory = XPathFactory.newInstance();
		}

		if (xPath == null) {
			xPath = xPathFactory.newXPath();
		}
		
		if (msgIdXPathExpression == null) {
			msgIdXPathExpression = xPath.compile(MSG_URI_XPATH_EXPR);
		}
		
		if (codeXPathExpression == null) {
			codeXPathExpression = xPath.compile(CODE_XPATH_EXPR);
		}

		if (bodyXPathExpression == null) {
			bodyXPathExpression = xPath.compile(BODY_XPATH_EXPR);
		}
		
		if (slotIdXPathExpression == null) {
			slotIdXPathExpression = xPath.compile(SLOT_EXPR);
		}
	}
	
	/**
	 * Retrieves the URI of a put message/request.
	 * 
	 * @param xmlString The XML (as a string) that the client receives when it sucessfully puts a message (request) on the server.
	 * @return The message URI that the client can use to later fetch the result of the request.
	 * @throws ParseException
	 */
	static public String getMsgURI(String xmlString) throws ParseException {

		try {
			Response.createBuilder();
			InputSource source = new InputSource(new StringReader(xmlString));
			
			Node node = (Node) msgIdXPathExpression.evaluate(source, XPathConstants.NODE);
			
			return node.getTextContent();
			
		} catch (XPathExpressionException e) {
			throw new ParseException();
		} catch (ParserConfigurationException e) {
			throw new ParseException();
		}
	}
	
	/**
	 * Parses a response to a Get request. 
	 * @param xml_string
	 * @return A response object - if the response contains a list of slots then the response 
	 * will be of class Slots; otherwise the response is of class Response.
	 * @throws ParseException
	 */
	
	static public Response parseGetResponse(String xml_string)
			throws ParseException {
		Response response = null;

		try {
			Response.createBuilder();
			InputSource source_code = new InputSource(new StringReader(xml_string));
			InputSource source_body = new InputSource(new StringReader(xml_string));
			InputSource source_slots = new InputSource(new StringReader(xml_string));
			
			Node code_node = (Node) codeXPathExpression.evaluate(source_code, XPathConstants.NODE);
			Node body_node = (Node) bodyXPathExpression.evaluate(source_body, XPathConstants.NODE);
			NodeList slot_nodes = (NodeList) slotIdXPathExpression.evaluate(source_slots, XPathConstants.NODESET);

			if (slot_nodes.getLength() == 0){
				response = new Response(Integer.parseInt(code_node.getTextContent()), body_node.getTextContent());
			} else {
				ArrayList <Integer> slot_list = new ArrayList<Integer>();
				
				for (int i = 0; i < slot_nodes.getLength(); i++){
					slot_list.add(Integer.parseInt(slot_nodes.item(i).getTextContent()));
				}

				response = new Slots(Integer.parseInt(code_node.getTextContent()), body_node.getTextContent(), slot_list);				
			}
			
			return response;			
		} catch (XPathExpressionException e) {
			throw new ParseException(e);
		} catch (ParserConfigurationException e) {
			throw new ParseException(e);
		}

	}
}
