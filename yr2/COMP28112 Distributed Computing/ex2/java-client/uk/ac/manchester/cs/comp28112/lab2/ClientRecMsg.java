package uk.ac.manchester.cs.comp28112.lab2;

import org.apache.commons.httpclient.HttpClient;
import org.apache.commons.httpclient.HttpStatus;
import org.apache.commons.httpclient.methods.GetMethod;

/*
 * Simple client to get and parse a response.
 * Reads from the command line the URI of the message you wish to get.
 */

public class ClientRecMsg {
	public static void main(String[] args) {

		GetMethod getMethod = null;

		String msg_uri = args[0];

		try {
			getMethod = new GetMethod(msg_uri);

			getMethod.addRequestHeader("Content-Type", "application/xml");
			getMethod.addRequestHeader("Accept", "application/xml");

			HttpClient client = new HttpClient();
			int responseCode = client.executeMethod(getMethod);

			if (responseCode == HttpStatus.SC_OK){
				Response response = Response.parseGetResponse(getMethod.getResponseBodyAsString());
				System.out.println("Response type: " + response.getResponseType());
				System.out.println(response.toString());
			} else if (responseCode == HttpStatus.SC_NOT_FOUND){
				System.out.println("Message not found:" + responseCode);
				System.out.println("Error message:" + getMethod.getResponseBodyAsString());
			} else if (responseCode == HttpStatus.SC_UNAUTHORIZED) {
				System.out.println("Invalid username/password to retrieve message:" + responseCode);
				System.out.println("Error message:" + getMethod.getResponseBodyAsString());
			} else {
				System.out.println("Error code:" + responseCode);
				System.out.println("Error message:" + getMethod.getResponseBodyAsString());
			}
		} catch (Exception err) {
			System.out.println("Usage: 'java ClientRecMsg msg_uri'");

			err.printStackTrace();
		}
	}
}
