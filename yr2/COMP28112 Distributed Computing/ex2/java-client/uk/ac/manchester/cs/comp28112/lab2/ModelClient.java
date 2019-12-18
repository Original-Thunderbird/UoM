package uk.ac.manchester.cs.comp28112.lab2;

import java.io.IOException;

import org.apache.commons.httpclient.methods.PutMethod;
import org.apache.commons.httpclient.methods.RequestEntity;
import org.apache.commons.httpclient.methods.StringRequestEntity;

import org.apache.commons.httpclient.*;


import org.apache.commons.httpclient.HttpClient;
import org.apache.commons.httpclient.HttpStatus;
import org.apache.commons.httpclient.methods.GetMethod;

import java.text.SimpleDateFormat;
import java.util.*;
import java.util.Scanner;
/*
 * Simple client that reserves slot 80 for user "username" and password "password". The message is
 * uniquely identified by the triple "my_request_id_1, username, password".
 */
public class ModelClient {

  public static void reserve(int slotnum){
    Date date = new Date();
    SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
    PutMethod putMethod = new PutMethod("http://jewel.cs.man.ac.uk:3020/queue/enqueue");
		String xmlString;
		try {
			/*
			 * Prepare the body of a put method. Create the string, in XML syntax, a request to
			 * reserve (book) a slot.*/
			xmlString = XMLRequest.Reservation("my_request_id_"+sdf.format(date), "7hc179", "2iw9PN", slotnum);
      System.out.println(sdf.format(date));
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

      int responseCode;
      int count=0;
      do{
        if(count!=0){
          try {
            // thread to sleep for 1000 milliseconds
            Thread.sleep(1000);
         } catch (Exception e) {
            System.out.println(e);
         }
        }
        responseCode = client.executeMethod(putMethod);
        count++;
      }while(responseCode == HttpStatus.SC_NOT_FOUND || responseCode == HttpStatus.SC_SERVICE_UNAVAILABLE);

			/*
			 * Examine and print the response.
			 */
			if (responseCode == HttpStatus.SC_OK) {
				System.out.println("Message uri: " + Response.getMsgURI(putMethod.getResponseBodyAsString()));
			} else {
				System.out.println("Error code:" + responseCode);
				System.out.println("Error message:" + putMethod.getResponseBodyAsString());
			}

      String hotel_uri = Response.getMsgURI(putMethod.getResponseBodyAsString());
      GetMethod getMethod = new GetMethod(hotel_uri + "?username=7hc179&password=2iw9PN");

			getMethod.addRequestHeader("Content-Type", "application/xml");
			getMethod.addRequestHeader("Accept", "application/xml");

			HttpClient clientGet = new HttpClient();
			int responseGet;

      count=0;
      do{
        if(count!=0){
          try {
            // thread to sleep for 1000 milliseconds
            Thread.sleep(1000);
         } catch (Exception e) {
            System.out.println(e);
         }
        }
        responseGet = client.executeMethod(getMethod);
        count++;
      }while(responseGet == HttpStatus.SC_NOT_FOUND || responseGet == HttpStatus.SC_SERVICE_UNAVAILABLE);


			if (responseGet == HttpStatus.SC_OK){
				Response response = Response.parseGetResponse(getMethod.getResponseBodyAsString());
				System.out.println("Response type: " + response.getResponseType());
				System.out.println(response.toString());
			} else if (responseGet == HttpStatus.SC_UNAUTHORIZED) {
				System.out.println("Invalid username/password to retrieve message:" + responseCode);
				System.out.println("Error message:" + getMethod.getResponseBodyAsString());
			} else {
				System.out.println("Error code:" + responseCode);
				System.out.println("Error message:" + getMethod.getResponseBodyAsString());
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

  public static void cancel(int slotnum){
    Date date = new Date();
    SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
    PutMethod putToHotel = new PutMethod("http://jewel.cs.man.ac.uk:3020/queue/enqueue");
    //PutMethod putToHotel = new PutMethod("http://jewel.cs.man.ac.uk:3010/queue/enqueue");
		String xmlString;
		try {
			 /* Prepare the body of a put method. Create the string, in XML syntax, a request to
			   reserve (book) a slot.*/
			xmlString = XMLRequest.cancel("my_request_id_"+sdf.format(date), "7hc179", "2iw9PN", slotnum);
			//Set the request's entity (body).
			RequestEntity entity = new StringRequestEntity(xmlString);
			putToHotel.setRequestEntity(entity);
			//Set the put method's headers
			putToHotel.addRequestHeader("Content-Type", "application/xml");
			putToHotel.addRequestHeader("Accept", "application/xml");
			//Create a client and the execute the put method.
      HttpClient client = new HttpClient();
      int responseHotel;
      int count=0;
      do{
        if(count!=0){
          try {
              // thread to sleep for 1000 milliseconds
              Thread.sleep(1000);
           } catch (Exception e) {
              System.out.println(e);
           }
        }
        responseHotel = client.executeMethod(putToHotel);
        count++;
      }while(responseHotel == HttpStatus.SC_NOT_FOUND || responseHotel == HttpStatus.SC_SERVICE_UNAVAILABLE);

			 // Examine and print the response.
      String hotel_uri = Response.getMsgURI(putToHotel.getResponseBodyAsString());

			if(responseHotel == HttpStatus.SC_OK) {
				System.out.println("Hotel-Message uri: " + hotel_uri);
			} else {
				System.out.println("Hotel-Error code:" + responseHotel);
				System.out.println("Hotel-Error message:" + hotel_uri);
			}

      GetMethod getMethod = new GetMethod(hotel_uri + "?username=7hc179&password=2iw9PN");

			getMethod.addRequestHeader("Content-Type", "application/xml");
			getMethod.addRequestHeader("Accept", "application/xml");

			HttpClient clientGet = new HttpClient();
			int responseGet;

      count=0;
      do{
        if(count!=0){
          try {
            // thread to sleep for 1000 milliseconds
            Thread.sleep(1000);
         } catch (Exception e) {
            System.out.println(e);
         }
        }
        responseGet = client.executeMethod(getMethod);
        count++;
      }while(responseGet == HttpStatus.SC_NOT_FOUND || responseGet == HttpStatus.SC_SERVICE_UNAVAILABLE);

			if (responseGet == HttpStatus.SC_OK){
				Response response = Response.parseGetResponse(getMethod.getResponseBodyAsString());
				System.out.println("Response type: " + response.getResponseType());
				System.out.println(response.toString());
			} else if (responseGet == HttpStatus.SC_UNAUTHORIZED) {
				System.out.println("Invalid username/password to retrieve message:" + responseGet);
				System.out.println("Error message:" + getMethod.getResponseBodyAsString());
			} else {
				System.out.println("Error code:" + responseGet);
				System.out.println("Error message:" + getMethod.getResponseBodyAsString());
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

  public static void availabilityCheck(){
    Date date = new Date();
    SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
    PutMethod putToHotel = new PutMethod("http://jewel.cs.man.ac.uk:3010/queue/enqueue");
    //PutMethod putToHotel = new PutMethod("http://jewel.cs.man.ac.uk:3010/queue/enqueue");
		String xmlString;
		try {
			 /* Prepare the body of a put method. Create the string, in XML syntax, a request to
			   reserve (book) a slot.*/
			xmlString = XMLRequest.availability("my_request_id_"+sdf.format(date), "7hc179", "2iw9PN");
			//Set the request's entity (body).
			RequestEntity entity = new StringRequestEntity(xmlString);
			putToHotel.setRequestEntity(entity);
			//Set the put method's headers
			putToHotel.addRequestHeader("Content-Type", "application/xml");
			putToHotel.addRequestHeader("Accept", "application/xml");
			//Create a client and the execute the put method.
      HttpClient client = new HttpClient();
      int responseHotel;
      int count=0;
      do{
        if(count!=0){
          try {
              // thread to sleep for 1000 milliseconds
              Thread.sleep(1000);
           } catch (Exception e) {
              System.out.println(e);
           }
        }
        responseHotel = client.executeMethod(putToHotel);
        count++;
      }while(responseHotel == HttpStatus.SC_NOT_FOUND || responseHotel == HttpStatus.SC_SERVICE_UNAVAILABLE);

      String hotel_uri = Response.getMsgURI(putToHotel.getResponseBodyAsString());
			 // Examine and print the response.
			if(responseHotel == HttpStatus.SC_OK) {
				System.out.println("Hotel-Message uri: " + hotel_uri);
			} else {
				System.out.println("Hotel-Error code:" + responseHotel);
				System.out.println("Hotel-Error message:" + hotel_uri);
			}

      GetMethod getMethod = new GetMethod(hotel_uri + "?username=7hc179&password=2iw9PN");

			getMethod.addRequestHeader("Content-Type", "application/xml");
			getMethod.addRequestHeader("Accept", "application/xml");

			HttpClient clientGet = new HttpClient();
			int responseGet;

      count=0;
      do{
        if(count!=0){
          try {
            // thread to sleep for 1000 milliseconds
            Thread.sleep(1000);
         } catch (Exception e) {
            System.out.println(e);
         }
        }
        responseGet = client.executeMethod(getMethod);
        count++;
      }while(responseGet == HttpStatus.SC_NOT_FOUND || responseGet == HttpStatus.SC_SERVICE_UNAVAILABLE);

			if (responseGet == HttpStatus.SC_OK){
				Response response = Response.parseGetResponse(getMethod.getResponseBodyAsString());
				System.out.println("Response type: " + response.getResponseType());
				System.out.println(response.toString());
			} else if (responseGet == HttpStatus.SC_UNAUTHORIZED) {
				System.out.println("Invalid username/password to retrieve message:" + responseGet);
				System.out.println("Error message:" + getMethod.getResponseBodyAsString());
			} else {
				System.out.println("Error code:" + responseGet);
				System.out.println("Error message:" + getMethod.getResponseBodyAsString());
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

  public static void booking(){
    Date date = new Date();
    SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
    PutMethod putToHotel = new PutMethod("http://jewel.cs.man.ac.uk:3010/queue/enqueue");
    //PutMethod putToHotel = new PutMethod("http://jewel.cs.man.ac.uk:3010/queue/enqueue");
		String xmlString;
		try {
			 /* Prepare the body of a put method. Create the string, in XML syntax, a request to
			   reserve (book) a slot.*/
			xmlString = XMLRequest.bookings("my_request_id_"+sdf.format(date), "7hc179", "2iw9PN");
			//Set the request's entity (body).
			RequestEntity entity = new StringRequestEntity(xmlString);
			putToHotel.setRequestEntity(entity);
			//Set the put method's headers
			putToHotel.addRequestHeader("Content-Type", "application/xml");
			putToHotel.addRequestHeader("Accept", "application/xml");
			//Create a client and the execute the put method.
      HttpClient client = new HttpClient();
      int responseHotel;
      int count=0;
      do{
        if(count!=0){
          try {
              // thread to sleep for 1000 milliseconds
              Thread.sleep(1000);
           } catch (Exception e) {
              System.out.println(e);
           }
        }
        responseHotel = client.executeMethod(putToHotel);
        count++;
      }while(responseHotel == HttpStatus.SC_NOT_FOUND || responseHotel == HttpStatus.SC_SERVICE_UNAVAILABLE);

      String hotel_uri = Response.getMsgURI(putToHotel.getResponseBodyAsString());
			 // Examine and print the response.
			if(responseHotel == HttpStatus.SC_OK) {
				System.out.println("Hotel-Message uri: " + hotel_uri);
			} else {
				System.out.println("Hotel-Error code:" + responseHotel);
				System.out.println("Hotel-Error message:" + hotel_uri);
			}

      GetMethod getMethod = new GetMethod(hotel_uri + "?username=7hc179&password=2iw9PN");

			getMethod.addRequestHeader("Content-Type", "application/xml");
			getMethod.addRequestHeader("Accept", "application/xml");

			HttpClient clientGet = new HttpClient();
			int responseGet;

      count=0;
      do{
        if(count!=0){
          try {
            // thread to sleep for 1000 milliseconds
            Thread.sleep(1000);
         } catch (Exception e) {
            System.out.println(e);
         }
        }
        responseGet = client.executeMethod(getMethod);
        count++;
      }while(responseGet == HttpStatus.SC_NOT_FOUND || responseGet == HttpStatus.SC_SERVICE_UNAVAILABLE);

			if (responseGet == HttpStatus.SC_OK){
				Response response = Response.parseGetResponse(getMethod.getResponseBodyAsString());
				System.out.println("Response type: " + response.getResponseType());
				System.out.println(response.toString());
			} else if (responseGet == HttpStatus.SC_UNAUTHORIZED) {
				System.out.println("Invalid username/password to retrieve message:" + responseGet);
				System.out.println("Error message:" + getMethod.getResponseBodyAsString());
			} else {
				System.out.println("Error code:" + responseGet);
				System.out.println("Error message:" + getMethod.getResponseBodyAsString());
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
	/**
	 * @param args
	 */
	public static void main(String[] args) {

    Scanner input = new Scanner(System.in);

    System.out.println("operation?\n1-book\n2-cancel\n3-availability\n4-booking check");
    int slotnum, choice = input.nextInt();
    switch(choice){
      case 1:
        System.out.println("Which slot you want to book?");
        slotnum = input.nextInt();
        reserve(slotnum);
        break;
      case 2:
        System.out.println("Which slot you want to cancel?");
        slotnum = input.nextInt();
        cancel(slotnum);
        break;
      case 3:
        availabilityCheck();
        break;
      case 4:
        booking();
        break;
    }
    //reserve a slot




	}

}
