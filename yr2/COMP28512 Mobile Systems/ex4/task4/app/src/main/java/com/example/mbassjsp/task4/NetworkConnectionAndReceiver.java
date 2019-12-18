package com.example.mbassjsp.task4;

// Created by A Leeming
// Modified JSP
// Date 17-1-2018
// See https://developer.android.com ,for android classes, methods, etc
// Code snippets from http://examples.javacodegeeks.com/android/core/socket-core/android-socket-example

// import classes
import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.widget.Button;
import android.widget.Spinner;
import android.widget.TextView;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;
import android.widget.ArrayAdapter;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class NetworkConnectionAndReceiver extends Thread{
    //Declare class variables
    private Socket socket = null;
    private static final int SERVERPORT = 9999; // This is the port that we are connecting to
    // Channel simulator is 9998
    private static final String SERVERIP = "10.0.2.2";  // This is the host's loopback address
    private static final String LOGTAG = "Network and receiver"; // Identify logcat messages

    private boolean terminated = false; // When FALSE keep thread alive and polling

    private PrintWriter streamOut = null; // Transmitter stream
    private BufferedReader streamIn = null; // Receiver stream
    private AppCompatActivity parentRef; // Reference to main user interface(UI)
    private TextView receiverDisplay; // Receiver display
    private String message = null; //Received message
    private String save;
    private int viewID=0;
    private int supportID=0;
    public int flag=0, Fs = 8000, len, i, NS = Fs*10;
    private List<String> data_list;
    private ArrayAdapter<String> arr_adapter;
    private Spinner spinner,spinmode;
    private String online;
    private int[] intrev = new int[40];
    private short[] shortrev=new short[40];
    private String bitAudio = "", strrev;
    private boolean decrypted = false, found = false;
    private String[] temp = new String[20];
    private int count = 0;
    private String S1, A1="Sent by BarryBot, School of Computer Science, The University of Manchester",S2, A2;
    private char[] chrrev, S1_chr, S2_chr, A2_chr,A1_chr,subs=new char[640];

    private AudioTrack track = new AudioTrack(AudioManager.STREAM_MUSIC,Fs,
            AudioFormat.CHANNEL_OUT_MONO,AudioFormat.ENCODING_PCM_16BIT,NS*2,
            AudioTrack.MODE_STATIC);



    //class constructor
    public NetworkConnectionAndReceiver(AppCompatActivity parentRef, int viewID, int supportID)
    {
        this.parentRef=parentRef; // Get reference to UI
        this.viewID = viewID;
        this.supportID=supportID;
    }

    public void setFlag(int flag){
        this.flag=flag;
    }
    // Start new thread method
    public void run()
    {
        temp[0]="";

        Log.i(LOGTAG,"Running in new thread");

        //Create socket and input output streams
        try {   //Create socket
            //InetAddress svrAddr = InetAddress.getByName(SERVERIP);
            InetAddress svrAddr = InetAddress.getLocalHost();
            socket = new Socket(svrAddr, SERVERPORT);

            //Setup i/o streams
            streamOut = new PrintWriter(new OutputStreamWriter(socket.getOutputStream()), true);
            streamIn = new BufferedReader(new InputStreamReader(socket.getInputStream()));
        }
        catch (UnknownHostException uhe) {
            Log.e(LOGTAG, "Unknownhost\n" + uhe.getStackTrace().toString());
            terminated = true;
        }
        catch (Exception e) {
            Log.e(LOGTAG, "Socket failed\n" + e.getMessage());
            e.printStackTrace();
            terminated = true;
        }
        //receiver
        i=0;
        while(!terminated) // Keep thread running
        {
            try {
                message = streamIn.readLine(); // Read a line of text from the input stream
                // If the message has text then display it
                if (message != null && message != "") {
                    Log.i(LOGTAG, "MSG recv : " + message);
                    if(flag!=1){
                        receiverDisplay = parentRef.findViewById(viewID);
                    }
                    else{
                        receiverDisplay = parentRef.findViewById(supportID);
                    }
                    if(flag==4){
                        save = message.substring(24);
                    }
                    //Run code in run() method on UI thread
                    parentRef.runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            // Display message, and old text in the receiving text area


                            if(flag==2||flag==3){
                                if(flag==3)
                                    len = message.length()/8;
                                intrev = new int[len];
                                chrrev = new char[len];
                                //message = new StringBuilder(message).reverse().toString();
                                //Log.e(LOGTAG, "message sent:"+message.substring(24));
                                switch (flag) {
                                    case 2:
                                        bitS2IntA(message.substring(24), len, intrev);
                                        break;
                                    case 3:
                                        bitS2IntA2(message, len, intrev);
                                }
                                i = 0;
                                Log.e(LOGTAG, "len:"+len);
                                for(int n:intrev){
                                    //Log.e(LOGTAG, "re-inted:"+intrev[i]);
                                    chrrev[i] = (char)n;
                                    i++;
                                }
                                strrev = new String(chrrev);
                                if(flag==3&&found==false)
                                    receiverDisplay.setText("encrypted:"+strrev + "\n" + receiverDisplay.getText());
                                if(flag==2)
                                    receiverDisplay.setText(strrev + "\n" + receiverDisplay.getText());
                                if(flag==3){
                                    if(found==false){
                                        if(count==0){
                                            temp[count]=strrev;
                                            count++;
                                        }
                                        else {
                                            for(i=0;i<count;i++){
                                                if(temp[i].equals(strrev)==true){
                                                    S1=strrev;  found=true; S1_chr = S1.toCharArray();  A1_chr = A1.toCharArray();
                                                    receiverDisplay.setText("decrypted" + "\n" + receiverDisplay.getText());
                                                    break;
                                                }
                                            }
                                            if(i==count){
                                                temp[count]=strrev;
                                                count++;
                                            }
                                        }
                                    }
                                    else {
                                        S2=strrev;
                                        S2_chr = S2.toCharArray();
                                        A2_chr = new char[S1.length()];
                                        Log.e(LOGTAG, "S1_len:"+S1.length());
                                        Log.e(LOGTAG, "S2_len:"+S2.length());
                                        for(i=0;i<S1.length();i++){
                                            A2_chr[i] = (char)(S1_chr[i]^S2_chr[i]^A1_chr[i]);
                                        }
                                        A2 = new String(A2_chr);
                                        receiverDisplay.setText("decrypted:"+A2 + "\n" + receiverDisplay.getText());
                                    }
                                }
                            }
                            if(flag==0)
                                receiverDisplay.setText(message + "\n" + receiverDisplay.getText());
                            if(flag!=4)
                                message = message.substring(6,message.length()-2);
                            Log.v(LOGTAG, "before split"+message);
                            if(flag==1){
                                online = "Currently Online:\n";

                                spinner = parentRef.findViewById(R.id.spinnerOnline);
                                Log.v(LOGTAG, "after split");
                                String temp[] = message.split("', '");
                                data_list = Arrays.asList(temp);
                                for(String str:data_list)
                                    Log.v(LOGTAG, str);
                                arr_adapter = new ArrayAdapter<String>(parentRef,R.layout.support_simple_spinner_dropdown_item,data_list);
                                arr_adapter.setDropDownViewResource(R.layout.support_simple_spinner_dropdown_item);
                                spinner.setAdapter(arr_adapter);

                                for (String str:data_list){
                                    online = online.concat(str+'\n');
                                }
                                receiverDisplay.setText("");
                                receiverDisplay.setText(online + "\n" + receiverDisplay.getText());
                            }
                        }
                    });
                }
            }
            catch (Exception e) {
                Log.e(LOGTAG, "Receiver failed\n" + e.getMessage());
                e.printStackTrace();
            }

        }
        //Call disconnect method to close i/o streams and socket
        disconnect();
        Log.i(LOGTAG,"Thread now closing");
    }


    //  Method for closing socket and i/o streams
    public void disconnect()
    {
        Log.i(LOGTAG, "Closing socket and io streams");
        try {
            streamIn.close();
            streamOut.close();
        }
        catch(Exception e)
        {/*do nothing*/}

        try {
            socket.close();
        }
        catch(Exception e)
        {/*do nothing*/}
    }
    // Getter method for returning the output stream for the transmitter to use
    public PrintWriter getStreamOut() {return this.streamOut;}
    // Setter method for terminating this thread
    // Set value to true to close thread
    public void closeThread(boolean value) {this.terminated = value;}

    public static void bitS2IntA(String bitS, int L, int[] IntA){
        // Converts a bit string bitS representing 16*L bits to L positive 16-bit integers
        for (int i=0; i<L; i++){
            int d=0;
            for (int j=0; j<16; j++){
                int k = i*16+j;
                d = d + (Character.getNumericValue(bitS.charAt(k))<< j);
            }
            IntA[i]=d;
        }
    } // end of met


    public static void bitS2IntA2(String bitS, int L, int[] IntA){
        // Converts a bit string bitS representing 16*L bits to L positive 16-bit integers
        for (int i=0; i<L; i++){
            int d=0;
            for (int j=0; j<8; j++){
                d<<=1;
                int k = i*8+j;
                d = d + Character.getNumericValue(bitS.charAt(k));
            }
            IntA[i]=d;
        }
    } // end of met

    public String getMessage(){
            return save;
    }
    public void setMessage(){save=null;}
}
