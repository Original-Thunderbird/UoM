/**
 * Helpers to construct XML requests.
 *
 * @author dkuo
 */

package uk.ac.manchester.cs.comp28112.lab2;

import java.io.*;

import javax.xml.parsers.*;
import javax.xml.transform.*;
import javax.xml.transform.stream.StreamResult;
import javax.xml.transform.dom.DOMSource;

import org.w3c.dom.*;

public class XMLRequest {

	private static String RESERVE_ELEMENT = "reserve";

	private static String CANCEL_ELEMENT = "cancel";

	private static String AVAILABILITY_ELEMENT = "availability";

	private static String BOOKINGS_ELEMENT = "bookings";

	private static String REQUEST_ID_ELEMENT = "request_id";

	private static String USERNAME_ELEMENT = "username";

	private static String PASSWORD_ELEMENT = "password";

	private static String SLOT_ID_ELEMENT = "slot_id";

	private static DocumentBuilderFactory documentFactory;

	private static DocumentBuilder documentBuilder;

	private static TransformerFactory transformerFactory;

	private static Transformer transformer;


	/**
	 * Creates a singleton document factory and document factory.
	 *
	 * @throws ParserConfigurationException
	 */
	static private void createBuilder() throws ParserConfigurationException {
		if (documentFactory == null) {
			documentFactory = DocumentBuilderFactory.newInstance();
		}
		if (documentBuilder == null) {
			documentBuilder = documentFactory.newDocumentBuilder();
		}
	}

	/**
	 * Creates a singleton transformer factory and transformer.
	 * @throws TransformerFactoryConfigurationError
	 * @throws TransformerConfigurationException
	 */
	static private void createTransformer()
			throws TransformerFactoryConfigurationError,
			TransformerConfigurationException {
		if (transformerFactory == null) {
			transformerFactory = TransformerFactory.newInstance();
		}
		if (transformer == null) {
			transformer = transformerFactory.newTransformer();
		}
	}

	/**
	 * Returns the XML string of an XML document.
	 *
	 * @param xml_document
	 * @return
	 * @throws TransformerFactoryConfigurationError
	 * @throws TransformerConfigurationException
	 * @throws TransformerException
	 */

	static private String toString(Document xml_document)
			throws TransformerFactoryConfigurationError,
			TransformerConfigurationException, TransformerException {

		XMLRequest.createTransformer();
		StreamResult result = new StreamResult(new StringWriter());
		transformer.transform(new DOMSource(xml_document), result);
		return result.getWriter().toString();
	}

	/**
	 * Returns the an XML string for a reservation request.
	 * <p>
	 * The triple (request_id, username, password) uniquely identifies this request.
	 *
	 * @param request_id The request identifier.
	 * @param username  Your username to the service.
	 * @param password  Your password to the service.
	 * @param slot_id The slot you wish to reserve.
	 * @return XML string of your reservation request.
	 * @throws RequestException
	 */

	static public String Reservation(String request_id, String username,
			String password, int slot_id) throws RequestException {
		try {
			XMLRequest.createBuilder();

			Document document = documentBuilder.newDocument();
			Element reserve_element = document.createElement(RESERVE_ELEMENT);
			document.appendChild(reserve_element);

			Node id_element = document.createElement(REQUEST_ID_ELEMENT);
			id_element.appendChild(document.createTextNode(request_id));
			reserve_element.appendChild(id_element);

			Node username_element = document.createElement(USERNAME_ELEMENT);
			username_element.appendChild(document.createTextNode(username));
			reserve_element.appendChild(username_element);

			Node password_element = document.createElement(PASSWORD_ELEMENT);
			password_element.appendChild(document.createTextNode(password));
			reserve_element.appendChild(password_element);

			Node slot_id_element = document.createElement(SLOT_ID_ELEMENT);
			slot_id_element.appendChild(document.createTextNode(new Integer(
					slot_id).toString()));
			reserve_element.appendChild(slot_id_element);

			return XMLRequest.toString(document);

		} catch (ParserConfigurationException e) {
			throw new RequestException(e);
		} catch (TransformerConfigurationException e) {
			throw new RequestException(e);
		} catch (TransformerFactoryConfigurationError e) {
			throw new RequestException(e.getException());
		} catch (TransformerException e) {
			throw new RequestException(e);
		}
	}

	/**
	 * Returns the XML string that represents your cancellation request.
	 * <p>
	 * The triple (request_id, username, password) uniquely identifies this request.
	 *
	 *
	 * @param request_id  The request identifier.
	 * @param username  Your username to the service.
	 * @param password  Your password to the service.
	 * @param slot_id  The slot you wish to cancel.
	 * @return XML String of your cancellation request
	 * @throws RequestException
	 */
	static public String cancel(String request_id, String username,
			String password, int slot_id) throws RequestException {

				try {
					XMLRequest.createBuilder();

					Document document = documentBuilder.newDocument();
					Element cancel_element = document.createElement(CANCEL_ELEMENT);
					document.appendChild(cancel_element);

					Node id_element = document.createElement(REQUEST_ID_ELEMENT);
					id_element.appendChild(document.createTextNode(request_id));
					cancel_element.appendChild(id_element);

					Node username_element = document.createElement(USERNAME_ELEMENT);
					username_element.appendChild(document.createTextNode(username));
					cancel_element.appendChild(username_element);

					Node password_element = document.createElement(PASSWORD_ELEMENT);
					password_element.appendChild(document.createTextNode(password));
					cancel_element.appendChild(password_element);

					Node slot_id_element = document.createElement(SLOT_ID_ELEMENT);
					slot_id_element.appendChild(document.createTextNode(new Integer(
							slot_id).toString()));
					cancel_element.appendChild(slot_id_element);

					return XMLRequest.toString(document);

				} catch (ParserConfigurationException e) {
					throw new RequestException(e);
				} catch (TransformerConfigurationException e) {
					throw new RequestException(e);
				} catch (TransformerFactoryConfigurationError e) {
					throw new RequestException(e.getException());
				} catch (TransformerException e) {
					throw new RequestException(e);
				}
	}

	/**
	 * Returns the XML string that represents your query to check for free slots.
	 * <p>
	 * The triple (request_id, username, password) uniquely identifies this request.
	 *
	 * @param request_id  The request identifier.
	 * @param username  Your username to the service.
	 * @param password  Your password to the service.
	 * @return XML string of your request to check availability.
	 * @throws RequestException
	 */

	static public String availability(String request_id, String username, String password)
			throws RequestException {

		try {
			XMLRequest.createBuilder();

			Document document = documentBuilder.newDocument();
			Element root = document.createElement(AVAILABILITY_ELEMENT);
			document.appendChild(root);

			Node id_element = document.createElement(REQUEST_ID_ELEMENT);
			id_element.appendChild(document.createTextNode(request_id));
			root.appendChild(id_element);

			Node username_element = document.createElement(USERNAME_ELEMENT);
			username_element.appendChild(document.createTextNode(username));
			root.appendChild(username_element);

			Node password_element = document.createElement(PASSWORD_ELEMENT);
			password_element.appendChild(document.createTextNode(password));
			root.appendChild(password_element);

			return XMLRequest.toString(document);

		} catch (ParserConfigurationException e) {
			throw new RequestException(e);
		} catch (TransformerConfigurationException e) {
			throw new RequestException(e);
		} catch (TransformerFactoryConfigurationError e) {
			throw new RequestException(e.getException());
		} catch (TransformerException e) {
			throw new RequestException(e);
		}
	}

	/**
	 * Returns the XML string that represents your query to retrieve your existing bookings.
	 * <p>
	 * The triple (request_id, username, password) uniquely identifies this request.
	 *
	 * @param request_id  The request identifier.
	 * @param username  Your username to the service.
	 * @param password  Your password to the service.
	 * @return  XML string of your request to retrieve your bookings.
	 * @throws RequestException
	 */

	static public String bookings(String request_id, String username,
			String password) throws RequestException {
				try {
					XMLRequest.createBuilder();

					Document document = documentBuilder.newDocument();
					Element root = document.createElement(BOOKINGS_ELEMENT);
					document.appendChild(root);

					Node id_element = document.createElement(REQUEST_ID_ELEMENT);
					id_element.appendChild(document.createTextNode(request_id));
					root.appendChild(id_element);

					Node username_element = document.createElement(USERNAME_ELEMENT);
					username_element.appendChild(document.createTextNode(username));
					root.appendChild(username_element);

					Node password_element = document.createElement(PASSWORD_ELEMENT);
					password_element.appendChild(document.createTextNode(password));
					root.appendChild(password_element);

					return XMLRequest.toString(document);

				} catch (ParserConfigurationException e) {
					throw new RequestException(e);
				} catch (TransformerConfigurationException e) {
					throw new RequestException(e);
				} catch (TransformerFactoryConfigurationError e) {
					throw new RequestException(e.getException());
				} catch (TransformerException e) {
					throw new RequestException(e);
				}
	}

}
