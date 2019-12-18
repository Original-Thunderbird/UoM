package uk.ac.manchester.cs.comp28112.lab2;

import java.io.IOException;

import org.apache.commons.httpclient.methods.PutMethod;
import org.apache.commons.httpclient.methods.RequestEntity;
import org.apache.commons.httpclient.methods.StringRequestEntity;

import org.apache.commons.httpclient.*;

import java.util.regex.Matcher;
import java.util.regex.Pattern;
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

  public static boolean reserve(PutMethod putMethod, int slotnum){
    Date date = new Date();
    SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
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

      String res_uri = Response.getMsgURI(putMethod.getResponseBodyAsString());
      GetMethod getMethod = new GetMethod(res_uri + "?username=7hc179&password=2iw9PN");

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
        if(response.toString().contains("Reserved slot "))  return true;
			} else if (responseGet == HttpStatus.SC_UNAUTHORIZED) {
				System.out.println("Invalid username/password to retrieve message:" + responseCode);
				System.out.println("Error message:" + getMethod.getResponseBodyAsString());
			} else {
				System.out.println("Error code:" + responseCode);
				System.out.println("Error message:" + getMethod.getResponseBodyAsString());
			}
      System.out.println(responseGet);
      return false;
		} catch (RequestException e) {
			e.printStackTrace();
		} catch (HttpException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		} catch (ParseException e) {
			e.printStackTrace();
		}
    return false;
	}

  public static void cancel(PutMethod putMethod, int slotnum){
    Date date = new Date();
    SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
		String xmlString;
		try {
			 /* Prepare the body of a put method. Create the string, in XML syntax, a request to
			   reserve (book) a slot.*/
			xmlString = XMLRequest.cancel("my_request_id_"+sdf.format(date), "7hc179", "2iw9PN", slotnum);
			//Set the request's entity (body).
			RequestEntity entity = new StringRequestEntity(xmlString);
			putMethod.setRequestEntity(entity);
			//Set the put method's headers
			putMethod.addRequestHeader("Content-Type", "application/xml");
			putMethod.addRequestHeader("Accept", "application/xml");
			//Create a client and the execute the put method.
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

			 // Examine and print the response.
      String res_uri = Response.getMsgURI(putMethod.getResponseBodyAsString());

			if(responseCode == HttpStatus.SC_OK) {
				System.out.println("Hotel-Message uri: " + res_uri);
			} else {
				System.out.println("Hotel-Error code:" + responseCode);
				System.out.println("Hotel-Error message:" + res_uri);
			}

      GetMethod getMethod = new GetMethod(res_uri + "?username=7hc179&password=2iw9PN");

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

  public static String availabilityCheck(PutMethod putMethod){
    Date date = new Date();
    SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
		String xmlString;
		try {
			 /* Prepare the body of a put method. Create the string, in XML syntax, a request to
			   reserve (book) a slot.*/
			xmlString = XMLRequest.availability("my_request_id_"+sdf.format(date), "7hc179", "2iw9PN");
			//Set the request's entity (body).
			RequestEntity entity = new StringRequestEntity(xmlString);
			putMethod.setRequestEntity(entity);
			//Set the put method's headers
			putMethod.addRequestHeader("Content-Type", "application/xml");
			putMethod.addRequestHeader("Accept", "application/xml");
			//Create a client and the execute the put method.
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

      String res_uri = Response.getMsgURI(putMethod.getResponseBodyAsString());
			 // Examine and print the response.
			if(responseCode == HttpStatus.SC_OK) {
				System.out.println("Hotel-Message uri: " + res_uri);
			} else {
				System.out.println("Hotel-Error code:" + responseCode);
				System.out.println("Hotel-Error message:" + res_uri);
			}

      GetMethod getMethod = new GetMethod(res_uri + "?username=7hc179&password=2iw9PN");

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
			} else {
				System.out.println("Error code:" + responseGet);
				System.out.println("Error message:" + getMethod.getResponseBodyAsString());
			}
      return getMethod.getResponseBodyAsString();
		} catch (RequestException e) {
			e.printStackTrace();
		} catch (HttpException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		} catch (ParseException e) {
			e.printStackTrace();
		}
    return "";
  }

  public static String booking(PutMethod putMethod){
    Date date = new Date();
    SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
		String xmlString;
		try {
			 /* Prepare the body of a put method. Create the string, in XML syntax, a request to
			   reserve (book) a slot.*/
			xmlString = XMLRequest.bookings("my_request_id_"+sdf.format(date), "7hc179", "2iw9PN");
			//Set the request's entity (body).
			RequestEntity entity = new StringRequestEntity(xmlString);
			putMethod.setRequestEntity(entity);
			//Set the put method's headers
			putMethod.addRequestHeader("Content-Type", "application/xml");
			putMethod.addRequestHeader("Accept", "application/xml");
			//Create a client and the execute the put method.
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

      String res_uri = Response.getMsgURI(putMethod.getResponseBodyAsString());
			 // Examine and print the response.
			if(responseCode == HttpStatus.SC_OK) {
				System.out.println("Hotel-Message uri: " + res_uri);
			} else {
				System.out.println("Hotel-Error code:" + responseCode);
				System.out.println("Hotel-Error message:" + res_uri);
			}

      GetMethod getMethod = new GetMethod(res_uri + "?username=7hc179&password=2iw9PN");

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
			} else {
				System.out.println("Error code:" + responseGet);
				System.out.println("Error message:" + getMethod.getResponseBodyAsString());
			}
      return getMethod.getResponseBodyAsString();
		} catch (RequestException e) {
			e.printStackTrace();
		} catch (HttpException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		} catch (ParseException e) {
			e.printStackTrace();
		}
    return "";
  }

  public static LinkedList<String> toIntList(String raw){
    LinkedList<String> numbers = new LinkedList<String>();
    Pattern p = Pattern.compile("\\d+");
    Matcher m = p.matcher(raw);
    while (m.find()) {
       numbers.add(m.group());
    }
    return numbers;
  }


  public static void automata(){
    PutMethod checkToBookHotel = new PutMethod("http://jewel.cs.man.ac.uk:3010/queue/enqueue");
    PutMethod checkToBookBand = new PutMethod("http://jewel.cs.man.ac.uk:3020/queue/enqueue");
    boolean res_hotel, res_band;
    //Sort out current bookings, if already booked then quit
    LinkedList<String> bookedHotel = toIntList(booking(checkToBookHotel));
    LinkedList<String> bookedBand = toIntList(booking(checkToBookBand));
    if (bookedHotel.size()>=2 && bookedBand.size()>=2){
      System.out.println("Already booked a slot");
      return ;
    }

    System.out.println("No bookings currently, carry on...");
    LinkedList<String> common;
    int cur;  //current best slot
    PutMethod checkToAvailHotel = new PutMethod("http://jewel.cs.man.ac.uk:3010/queue/enqueue");
    PutMethod checkToAvailBand = new PutMethod("http://jewel.cs.man.ac.uk:3020/queue/enqueue");

    PutMethod addToHotel = new PutMethod("http://jewel.cs.man.ac.uk:3010/queue/enqueue");
    PutMethod addToBand = new PutMethod("http://jewel.cs.man.ac.uk:3020/queue/enqueue");

    PutMethod delToHotel = new PutMethod("http://jewel.cs.man.ac.uk:3010/queue/enqueue");
    PutMethod delToBand = new PutMethod("http://jewel.cs.man.ac.uk:3020/queue/enqueue");
    //get available slots in common
    do{
      do{
        System.out.println("Check available slot");
        LinkedList<String> availHotel = toIntList(availabilityCheck(checkToAvailHotel));  //get available slots respectively
        LinkedList<String> availBand = toIntList(availabilityCheck(checkToAvailBand));
        common = new LinkedList<String>();
        if (availHotel.isEmpty() || availBand.isEmpty())  continue;
        for(String i:availHotel){ //get common availabel slots
          if(availBand.contains(i)){
            common.add(i);
          }
        }
      }while(common.isEmpty());
      cur = Integer.valueOf(common.get(1)).intValue();   //book the first slot, if fialed then assume available slot has been out dated, get available slot again.

      System.out.println("Try to book slot"+cur);
      res_hotel=reserve(addToHotel,cur);
      res_band=reserve(addToBand,cur); //get result of reservation in variable, if use method in if() directly, then the function will be called many times.
      if(res_hotel==false)                  cancel(delToBand,cur);
      if(res_band==false)                   cancel(delToHotel,cur);
    }while(res_band==false || res_hotel==false);
    System.out.println("Current booked slot:"+cur);

    PutMethod putToHotel = new PutMethod("http://jewel.cs.man.ac.uk:3010/queue/enqueue");
    PutMethod putToBand = new PutMethod("http://jewel.cs.man.ac.uk:3020/queue/enqueue");
    LinkedList<String> common_re;
    //check for better slot (at least) once
    int bet;
    do{
        do{
          System.out.println("Search for better slot");
          LinkedList<String> availHotel = toIntList(availabilityCheck(putToHotel));  //get available slots respectively
          LinkedList<String> availBand = toIntList(availabilityCheck(putToBand));
          common_re = new LinkedList<String>();
          if (availHotel.isEmpty() || availBand.isEmpty())  continue;
          for(String i:availHotel){ //get common availabel slots
            if(availBand.contains(i)){
              common_re.add(i);
            }
          }
        }while(common_re.isEmpty());
        bet = Integer.valueOf(common_re.get(1)).intValue();   //get first slot id
        if(bet>cur){
          System.out.println("no better slot.");
          return ;
        }
      System.out.println("Try to book slot"+bet);
      putToHotel = new PutMethod("http://jewel.cs.man.ac.uk:3010/queue/enqueue");
      putToBand = new PutMethod("http://jewel.cs.man.ac.uk:3020/queue/enqueue");
      res_hotel=reserve(putToHotel,bet);
      if(res_hotel==false){
        System.out.println("already booked");
        continue;
      }
      res_band=reserve(putToBand,bet); //get result of reservation in variable, if use method in if() directly, then the function will be called many times.
      putToHotel = new PutMethod("http://jewel.cs.man.ac.uk:3010/queue/enqueue");
      putToBand = new PutMethod("http://jewel.cs.man.ac.uk:3020/queue/enqueue");
      if(res_band==false)                   cancel(putToHotel,bet);
    }while(res_band==false || res_hotel==false);
    putToHotel = new PutMethod("http://jewel.cs.man.ac.uk:3010/queue/enqueue");
    putToBand = new PutMethod("http://jewel.cs.man.ac.uk:3020/queue/enqueue");
    cancel(putToBand,cur);  //release previous slot;
    cancel(putToHotel,cur);
    System.out.println("Current slot:"+bet);
    return ;
  }

	public static void main(String[] args) {
    Scanner input = new Scanner(System.in);
    System.out.println("operation?\n1-book\n2-cancel");
    int slotnum, choice = input.nextInt();
    switch(choice){
      case 1:
        automata();
        break;
      case 2:
        PutMethod putToHotel = new PutMethod("http://jewel.cs.man.ac.uk:3010/queue/enqueue");
        PutMethod putToBand = new PutMethod("http://jewel.cs.man.ac.uk:3020/queue/enqueue");
        System.out.println("Which slot you want to cancel?");
        slotnum = input.nextInt();
        cancel(putToHotel,slotnum);
        cancel(putToBand,slotnum);
        break;
    }
	}

}