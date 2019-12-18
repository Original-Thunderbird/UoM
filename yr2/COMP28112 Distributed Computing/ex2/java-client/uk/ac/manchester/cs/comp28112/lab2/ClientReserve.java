package uk.ac.manchester.cs.comp28112.lab2;

import java.io.IOException;

import org.apache.commons.httpclient.methods.PutMethod;
import org.apache.commons.httpclient.methods.RequestEntity;
import org.apache.commons.httpclient.methods.StringRequestEntity;

import org.apache.commons.httpclient.*;


/*
 * Simple client that reserves slot 80 for user "username" and password "password". The message is
 * uniquely identified by the triple "my_request_id_1, username, password".
 */
public class ClientReserve {

	/**
	 * @param args
	 */
	public static void main(String[] args) {

		PutMethod putMethod = new PutMethod(
				"http://jewel.cs.man.ac.uk:3010/queue/enqueue");
		String xmlString;
		try {
			/*
			 * Prepare the body of a put method. Create the string, in XML syntax, a request to
			 * reserve (book) a slot.
			 */
			xmlString = XMLRequest.Reservation("my_request_id_1", "7hc179", "2iw9PN", 80);

			/*
			 * Set the request's entity (body).
			 */
			RequestEntity entity = new StringRequestEntity(xmlString);
			putMethod.setRequestEntity(entity);

			/*
			 * Set the put method's headers
			 */
			putMethod.addRequestHeader("Content-Type", "application/xml");
			putMethod.addRequestHeader("Accept", "application/xml");

			/*
			 * Create a client and the execute the put method.
			 */
			HttpClient client = new HttpClient();
			int responseCode = client.executeMethod(putMethod);

			/*
			 * Examine and print the response.
			 */
			if (responseCode == HttpStatus.SC_OK) {
				System.out.println("Message uri: " + Response.getMsgURI(putMethod.getResponseBodyAsString()));
			} else {
				System.out.println("Error code:" + responseCode);
				System.out.println("Error message:" + putMethod.getResponseBodyAsString());
			}
		} catch (RequestException e) {
			e.printStackTrace();
		} catch (HttpException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		} catch (ParseException e) {
			e.printStackTrace();
		}
	}

}
