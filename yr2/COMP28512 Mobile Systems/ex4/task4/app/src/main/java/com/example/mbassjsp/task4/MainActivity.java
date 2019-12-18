package com.example.mbassjsp.task4;

// Created by A Leeming
// Modified JSP
// Date 17-1-2018
// See https://developer.android.com ,for android classes, methods, etc


// Import classes
import android.Manifest;
import android.app.Activity;
import android.app.ActionBar;
import android.app.Fragment;
import android.content.pm.PackageManager;
import android.media.AudioFormat;
import android.media.AudioTrack;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.os.Build;
//Added imports for this app
import android.widget.Button;       import android.widget.ImageView;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;  import java.io.IOException;
import java.io.InputStream;           import java.io.OutputStream;
import android.graphics.Bitmap;   import android.graphics.BitmapFactory;

import android.media.MediaRecorder;   import android.media.AudioRecord;
import android.media.AudioFormat;
import android.media.AudioTrack;
import android.media.AudioManager;
import android.os.Bundle;
import android.support.v4.app.ActivityCompat;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import java.io.File;  import java.io.FileOutputStream;  import java.io.DataOutputStream;
import java.io.IOException;
import android.os.Environment;

        import android.media.AudioManager;
        import android.media.MediaPlayer;
        import android.content.Intent;
        import android.support.v7.app.AppCompatActivity;
        import android.os.Bundle;
        import android.text.method.ScrollingMovementMethod;
        import android.util.Log;
        import android.view.Menu;
        import android.view.MenuItem;
        import android.view.View;
        import android.widget.ArrayAdapter;
        import android.widget.Button;
        import android.widget.EditText;
        import android.widget.Spinner;
        import android.widget.TextView;


        import java.util.ArrayList;
        import java.util.Arrays;
        import java.util.List;

        import static android.provider.AlarmClock.EXTRA_MESSAGE;
import static com.example.mbassjsp.task4.R.raw.damagedpeppers;
import static com.example.mbassjsp.task4.R.raw.damagedpeppersjpg;
import static com.example.mbassjsp.task4.R.raw.peppers;
import static com.example.mbassjsp.task4.R.raw.peppersjpg;


// Android apps must have a MainActivity class that extends Activity or AppCompatActivity class
public class MainActivity extends AppCompatActivity {
    //Declare class variables
    private static final String LOGTAG = "Main UI"; //Logcat messages from UI are identified
    private NetworkConnectionAndReceiver networkConnectionAndReceiver = null;
    private String register, invite, accept, msg; //Transmitter data variable
    private String message;
    private Spinner spinner;
    private Spinner spinmode;
    private List<String> mode_list = new ArrayList<>(Arrays.asList("0", "1", "2", "3", "4", "5"));
    private ArrayAdapter<String> mode_adapter;
    char[] charmes;
    int [] intmes;

    int i,str_len,j,z_smh=0;
    private int Fs = 8000, wav_len = Fs*15;
    private short[] dam_spe = new short[wav_len];

    private MediaPlayer mySound1, mySound4;
    private int NS= Fs*10;// Number of samples
    private short soundSamples[] = new short [(int) NS], soundResult[] = new short [(int) NS];
    private String wavFileName = null, bitAudio="";
    private int recBufferSize = AudioRecord.getMinBufferSize(Fs,AudioFormat.CHANNEL_IN_MONO,
            AudioFormat.ENCODING_PCM_16BIT);
    private int recChunks = ((int) (NS/recBufferSize));
    private AudioRecord recorder = new AudioRecord(MediaRecorder.AudioSource.MIC,(int) Fs,AudioFormat.CHANNEL_IN_MONO,
            AudioFormat.ENCODING_PCM_16BIT,recBufferSize*2);
    //private AudioRecord recorder = findAudioRecord();
    private int[] wavHeader ={0x46464952, 44+NS*2, 0x45564157, 0x20746D66,16, 0x00010001,Fs, Fs*2,
            0x00100002,0x61746164, NS*2};
    private AudioTrack track = new AudioTrack(AudioManager.STREAM_MUSIC,Fs,
            AudioFormat.CHANNEL_OUT_MONO,AudioFormat.ENCODING_PCM_16BIT,NS*2,
            AudioTrack.MODE_STATIC);
    private AudioTrack track2 = new AudioTrack(AudioManager.STREAM_MUSIC,12000,
            AudioFormat.CHANNEL_OUT_MONO,AudioFormat.ENCODING_PCM_16BIT,NS*2,
            AudioTrack.MODE_STATIC);
    private String mess;

    private short[] shortrev = new short [20];
    private int[] intrev =new int[40];
    private char[]subs = new char[640];


    Button bt1_2;

    void msSpeech(){// Method for reading 16-bit samples from a wav file into array data
        int i;  int b1 =0;  int b2 = 0;
        try{// Make speechis an input stream object for accessing a wav file placed in R.raw
            InputStream speechis=getResources().openRawResource(R.raw.damagedspeech);
            // Read&discard the 44 byte header of the wav file:
            for ( i = 0 ; i < 44 ; i++ ) {b1 = speechis.read(); }
            // Read rest of 16-bit samples from wav file byte-by-byte:
            for ( i = 0 ; i< wav_len ; i++ ){
                b1 = speechis.read();// Get first byte of 16-bit sample in least sig 8 bits of b1
                if(b1 == -1) {b1 = 0;}//b1 becomes -1 if we try to read past End of File
                b2 = speechis.read();// Get second byte  of sample value in b2
                if(b2 == -1) {b2 = 0;} //trying to read past EOF
                b2 = b2<<8; // shift b2 left by 8 binary places
                dam_spe[i] = (short) (b1 | b2); //Concat 2 bytes to make 16-bit sample value
            }// end of for loop
            speechis.close();
        } catch (FileNotFoundException e) {Log.e("tag", "wav file not found", e);}
        catch (IOException e) {Log.e("tag", "Failed to close input stream", e);}
    }// end of msSpeech metho

    void arrayPlay(boolean smh_flag){
        int CONF = AudioFormat.CHANNEL_OUT_MONO;
        int FORMAT = AudioFormat.ENCODING_PCM_16BIT;
        int MDE = AudioTrack.MODE_STATIC; //Need static mode.
        int STRMTYP = AudioManager.STREAM_ALARM;
        AudioTrack track = new AudioTrack(STRMTYP, Fs, CONF, FORMAT, wav_len*2, MDE);
        msSpeech();
        if(smh_flag==true){
            smooth();
        }
        track.write(dam_spe, 0, wav_len);
        track.play();
        while(track.getPlaybackHeadPosition() != wav_len) {};  //Wait before playing more
        track.stop();
        track.setPlaybackHeadPosition(0);
        while(track.getPlaybackHeadPosition() != 0) {}; // wait for head position
    }// end of arrayplay method

    void smooth(){
        int i=0;
        while(i*200<wav_len){
            //see if this is an empty pack
            for(j=i*200;j<(i+1)*200;j++){
                if(dam_spe[j]!=0){
                    break;
                }
            }
            //copy last pack to current pack
            if(j==(i+1)*200 && i!=0){
                for(j=i*200;j<(i+1)*200;j++){
                    dam_spe[j]=(short)(dam_spe[j-200]/2);
                }
            }
            if(i!=0){
                for(j=i*200;j<i*200+11;j++){
                    //dam_spe[j]=(short)((Math.pow(Math.E,j-i*200)*dam_spe[j]+z_smh)/(Math.pow(Math.E,j-i*200)+1));
                    dam_spe[j]=(short)((28*dam_spe[j]+24*dam_spe[j-1]+20*dam_spe[j-2]+16*dam_spe[j-3]
                            +12*dam_spe[j-4]+8*dam_spe[j-5]+4*dam_spe[j-6])/(112));
                }
            }
            j=0;
            while ((j+1)*3<wav_len){
                if(Math.abs(dam_spe[j+1]-(dam_spe[j]+dam_spe[j+2])/2)>10000){
                    dam_spe[j+1] = (short)((dam_spe[j]+dam_spe[j+2])/2);
                }
                else if(Math.abs(dam_spe[j]-(dam_spe[j+1]+dam_spe[j+2])/2)>10000){
                    dam_spe[j] = (short)((dam_spe[j+1]+dam_spe[j+2])/2);
                }
                else if(Math.abs(dam_spe[j+2]-(dam_spe[j+1]+dam_spe[j])/2)>10000){
                    dam_spe[j+2] = (short)((dam_spe[j+1]+dam_spe[j])/2);
                }
                j++;
            }
            i++;
        }
    }
    // Class methods
    @Override
    //Extend the onCreate method, called whenever an activity is started
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState); // Extend the onCreate method
        // Set up the view using xml description res>layout>activity_main.xml
        setContentView(R.layout.activity_main);

        bt1_2 = (Button) findViewById(R.id.btnchg);
        Button.OnClickListener listener = new Button.OnClickListener() {

            public void onClick(View v) {
                Intent intent = new Intent(MainActivity.this, Main2Activity.class);
                startActivity(intent);
                MainActivity.this.finish();
            }

        };
        bt1_2.setOnClickListener(listener);

        mySound1 = MediaPlayer.create(this, R.raw.narrobandspeech);
        mySound4 = MediaPlayer.create(this, R.raw.damagedspeech);

        wavFileName = getExternalFilesDir(null) + "/Barry.wav";
        Log.e(LOGTAG, "Wav file is " + wavFileName);
        // Get the following buttons as defined in Res dir xml code
        final Button recButton = findViewById(R.id.btnRecordAu);
        final Button playButton = findViewById(R.id.btnPlayAu);
        final Button plazButton = findViewById(R.id.btnPlayAv);

        for(i=0;i<640;i++){
            subs[i]='0';
        }

        Log.i(LOGTAG, "Starting task4 app"); // Report to Logcat
        spinner = findViewById(R.id.spinnerOnline);
        spinmode = findViewById(R.id.spinmodeOnline);
        TextView buddyReceiver = findViewById(R.id.onlineBuddyView);
        buddyReceiver.setMovementMethod(new ScrollingMovementMethod());

        mode_adapter = new ArrayAdapter<String>(this,R.layout.support_simple_spinner_dropdown_item,mode_list);
        mode_adapter.setDropDownViewResource(R.layout.support_simple_spinner_dropdown_item);
        spinmode.setAdapter(mode_adapter);

        // Instantiate the network connection and receiver object
        networkConnectionAndReceiver = new NetworkConnectionAndReceiver(this,R.id.txtServerResponse, R.id.onlineBuddyView);
        networkConnectionAndReceiver.start();    // Start socket-receiver thread

        // Get the receiving text area as defined in the Res dir xml code
        TextView receiverTextArea = findViewById(R.id.txtServerResponse);
        // Make the receiving text area scrollable
        receiverTextArea.setMovementMethod(new ScrollingMovementMethod());
        // Get the kill button as defined in the Res dir xml code
        Button killButton = findViewById(R.id.btnKill);
        // Make the kill button receptive to being clicked
        // Button click handler
        killButton.setOnClickListener(new View.OnClickListener() {
            // onClick method implementation for the killButton object
            public void onClick(View v) {
                // OnClick actions here
                Transmitter transmitter = new Transmitter(networkConnectionAndReceiver.getStreamOut(), "DISCONNECT", 1);
                Log.i(LOGTAG, "disconnect"); // Report to Logcat
                transmitter.start();        // Run on its own thread
                try {
                    // thread to sleep for 1000 milliseconds
                    Thread.sleep(50);
                } catch (Exception e) {
                    System.out.println(e);
                }
                // Exit app
                System.exit(0);

            }
        });


        // Get the text area for commands to be transmitted as defined in the Res dir xml code
        register = "REGISTER ";
        // Get the send button as defined in the Res dir xml code
        Button sendButton =  findViewById(R.id.btnSendCmd);
        // Make the kill button receptive to being clicked
        // Button click handler
        sendButton.setOnClickListener(new View.OnClickListener() {
            // onClick method implementation for the sendButton object
            public void onClick(View v) {
            EditText editText = (EditText) findViewById(R.id.editText);
            message = editText.getText().toString();
            // OnClick actions here
            // Instantiate the transmitter passing the output stream and text to it
            if(networkConnectionAndReceiver.getStreamOut() != null) { // Check that output stream has be setup
                networkConnectionAndReceiver.setFlag(0);
                Transmitter transmitter = new Transmitter(networkConnectionAndReceiver.getStreamOut(), register+message,1);
                transmitter.start();        // Run on its own thread
            }
            }
        });

        // Get the send button as defined in the Res dir xml code
        Button getButton =  findViewById(R.id.btnGet);
        // Make the kill button receptive to being clicked
        // Button click handler
        getButton.setOnClickListener(new View.OnClickListener() {
            // onClick method implementation for the sendButton object
            public void onClick(View v) {
                networkConnectionAndReceiver.setFlag(1);
            Transmitter transmitter = new Transmitter(networkConnectionAndReceiver.getStreamOut(), "who",1);
            transmitter.start();        // Run on its own thread
            }
        });

        invite = "INVITE ";
        // Get the send button as defined in the Res dir xml code
        Button inviteButton =  findViewById(R.id.btnInvite);
        // Make the kill button receptive to being clicked
        // Button click handler
        inviteButton.setOnClickListener(new View.OnClickListener() {
            // onClick method implementation for the sendButton object
            public void onClick(View v) {
                EditText editText = (EditText) findViewById(R.id.editText);
                message = editText.getText().toString();
                // OnClick actions here
                // Instantiate the transmitter passing the output stream and text to it
                if(networkConnectionAndReceiver.getStreamOut() != null) { // Check that output stream has be setup
                    networkConnectionAndReceiver.setFlag(0);
                    Transmitter transmitter = new Transmitter(networkConnectionAndReceiver.getStreamOut(), invite+spinner.getSelectedItem(),1);
                    transmitter.start();        // Run on its own thread
                }
            }
        });

        accept = "ACCEPT ";
        // Get the send button as defined in the Res dir xml code
        Button acceptButton =  findViewById(R.id.btnAccept);
        // Make the kill button receptive to being clicked
        // Button click handler
        acceptButton.setOnClickListener(new View.OnClickListener() {
            // onClick method implementation for the sendButton object
            public void onClick(View v) {
                EditText editText = (EditText) findViewById(R.id.editText);
                message = editText.getText().toString();
                // OnClick actions heretoString()
                // Instantiate the transmitter passing the output stream and text to it
                if(networkConnectionAndReceiver.getStreamOut() != null) { // Check that output stream has be setup
                    networkConnectionAndReceiver.setFlag(0);
                    Transmitter transmitter = new Transmitter(networkConnectionAndReceiver.getStreamOut(), accept+spinner.getSelectedItem(),1);
                    transmitter.start();        // Run on its own thread
                }
            }
        });

        msg = "MSG ";
        // Get the send button as defined in the Res dir xml code
        Button msgButton =  findViewById(R.id.btnMsg);
        // Make the kill button receptive to being clicked
        // Button click handler
        msgButton.setOnClickListener(new View.OnClickListener() {
            // onClick method implementation for the sendButton object
            public void onClick(View v) {
                EditText editText = (EditText) findViewById(R.id.editText);
                message = editText.getText().toString();
                if(message.toLowerCase().equals("encrypt")==false){
                    charmes = message.toCharArray();
                    str_len = message.length()+1;
                    i = 0;
                    intmes =new int[str_len];
                    for(char c:charmes){

                        intmes[i] = c;
                        //Log.e(LOGTAG, "inted:"+intmes[i]);
                        i++;
                    }
                    //Log.e(LOGTAG, "message sent:"+IntA2bitS(intmes, len));
                }
                // OnClick actions here
                // Instantiate the transmitter passing the output stream and text to it
                if(networkConnectionAndReceiver.getStreamOut() != null) { // Check that output stream has be setup
                    if(message.toLowerCase().equals("encrypt")==false){
                        networkConnectionAndReceiver.setFlag(2);
                        networkConnectionAndReceiver.len=str_len;
                        Transmitter transmitter = new Transmitter(networkConnectionAndReceiver.getStreamOut(), spinmode.getSelectedItem()+msg+spinner.getSelectedItem()+' '+IntA2bitS(intmes, str_len),1);
                        transmitter.start();        // Run on its own thread
                    }
                    else {
                        networkConnectionAndReceiver.setFlag(3);
                        Transmitter transmitter = new Transmitter(networkConnectionAndReceiver.getStreamOut(), spinmode.getSelectedItem()+msg+spinner.getSelectedItem()+' '+message,1);
                        transmitter.start();        // Run on its own thread
                    }
                }
            }
        });

        Button playarray =  findViewById(R.id.btnStt4_at);
        // Make the kill button receptive to being clicked
        // Button click handler
        playarray.setOnClickListener(new View.OnClickListener() {
            // onClick method implementation for the sendButton object
            public void onClick(View v) {
                arrayPlay(false);
            }
        });

        Button playarray_smh =  findViewById(R.id.btnStt4_sm);
        // Make the kill button receptive to being clicked
        // Button click handler
        playarray_smh.setOnClickListener(new View.OnClickListener() {
            // onClick method implementation for the sendButton object
            public void onClick(View v) {
                arrayPlay(true);
            }
        });

        recButton.setOnClickListener(new View.OnClickListener(){
            public void onClick(View v){
                checkRecordPermission();
                recButton.setText("RECORDING");
                recorder.startRecording();
                int i;
                for(i = 0; i < recChunks; i++ ){
                    recorder.read(soundSamples, i*recBufferSize, (int) recBufferSize);
                }
                recorder.read(soundSamples,i*recBufferSize, NS-i*recBufferSize);
                recorder.stop();
                Log.e(LOGTAG, "Finished recording");
                try {
                    File wavFile = new File(wavFileName);
                    FileOutputStream wavOutputStream = new FileOutputStream(wavFile);
                    DataOutputStream wavDataOutputStream = new DataOutputStream(wavOutputStream);
                    for(i = 0; i < wavHeader.length; i++){
                        wavDataOutputStream.writeInt(Integer.reverseBytes(wavHeader[i]));
                    }
                    for (i = 0 ; i < soundSamples.length ; i++ ){
                        wavDataOutputStream.writeShort(Short.reverseBytes(soundSamples[i]));
                    }
                    wavOutputStream.close();
                    Log.e(LOGTAG, "Wav file saved");
                } catch (IOException e) { Log.e(LOGTAG, "Wavfile write error");}
                recButton.setText("DONE");
            }// end of onClick method
        });// end ofrecButton.setOnClickListener

        playButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v){
                playButton.setText("PLAYING");
                Log.e(LOGTAG, "start playing");
//                for(i=0;i<12000;i++)
//                    Log.e(LOGTAG, String.valueOf(soundResult[i]));

                track.write(soundResult, 0, (int) NS);
                track.play();
                while(track.getPlaybackHeadPosition() < NS) {};  //Wait before playing more
                track.stop();
                track.setPlaybackHeadPosition(0);
                while(track.getPlaybackHeadPosition() != 0) {}; // wait for head position
                playButton.setText("PLAY");// for next time
            }
        });//end of playButton.setOnClickListener

        plazButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v){
                playButton.setText("PLAYING");
                Log.e(LOGTAG, "start playing");
//                for(i=0;i<12000;i++)
//                    Log.e(LOGTAG, String.valueOf(soundResult[i]));

                track2.write(soundResult, 0, (int) NS);
                track2.play();
                while(track2.getPlaybackHeadPosition() < NS) {};  //Wait before playing more
                track2.stop();
                track2.setPlaybackHeadPosition(0);
                while(track2.getPlaybackHeadPosition() != 0) {}; // wait for head position
                playButton.setText("PLAY");// for next time
            }
        });//end of playButton.setOnClickListener

        final Button sendAuButton = findViewById(R.id.btnsendAu);
        // end of recButton.setOnClickListener
        sendAuButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v){
                if(networkConnectionAndReceiver.getStreamOut() != null) { // Check that output stream has be setup
                    networkConnectionAndReceiver.setFlag(4);
                    networkConnectionAndReceiver.len=NS;
                    i=0;
                    intmes = new int[40];

                    for(i=0;i<2000;i++){
                        for(j=i*40;j<(i+1)*40;j++){
                            intmes[j-i*40]=soundSamples[j];
                        }
                        mess = IntA2bitS(intmes, 40);
                        Transmitter transmitter = new Transmitter(networkConnectionAndReceiver.getStreamOut(),
                                spinmode.getSelectedItem()+msg+spinner.getSelectedItem()+' '+mess, 4);
                        //Log.e(LOGTAG, "Send version:"+mess);
                        transmitter.start();        // Run on its own thread
                        do{
                            bitAudio = networkConnectionAndReceiver.getMessage();
                        }while(bitAudio==null);
                        if(bitAudio.length()<640&&(spinmode.getSelectedItem().toString().equals("4")||
                                spinmode.getSelectedItem().toString().equals("5"))){
                            bitAudio=new String(subs);
                        }
                        networkConnectionAndReceiver.setMessage();
                        //Log.e(LOGTAG, "string-"+bitAudio);
                        //Log.e(LOGTAG, "length"+bitAudio.length());
                        bitS2IntA(bitAudio,40,intrev);
                        for(j=0;j<40;j++){
                            //Log.e(LOGTAG, "intrev-j="+intrev[j]);
                            soundResult[i*40+j] = (short)intrev[j];
                        }
                        Log.e(LOGTAG, "i="+i);
                    }
                }

            }//end ofonClick method
        });//end of playButton.setOnClickListener
    } //End of app onCreate method

    private static int[] mSampleRates = new int[] { 8000, 11025, 22050, 44100 };
    public AudioRecord findAudioRecord() {
        for (int rate : mSampleRates) {
            for (short audioFormat : new short[] { AudioFormat.ENCODING_PCM_8BIT, AudioFormat.ENCODING_PCM_16BIT }) {
                for (short channelConfig : new short[] { AudioFormat.CHANNEL_IN_MONO, AudioFormat.CHANNEL_IN_STEREO }) {
                    try {
                        Log.e(LOGTAG, "Attempting rate " + rate + "Hz, bits: " + audioFormat + ", channel: "
                                + channelConfig);
                        int bufferSize = AudioRecord.getMinBufferSize(rate, channelConfig, audioFormat);

                        if (bufferSize != AudioRecord.ERROR_BAD_VALUE) {
                            // check if we can instantiate and have a success
                            AudioRecord recorder = new AudioRecord(MediaRecorder.AudioSource.DEFAULT, rate, channelConfig, audioFormat, bufferSize);

                            if (recorder.getState() == AudioRecord.STATE_INITIALIZED)
                                return recorder;
                        }
                    } catch (Exception e) {
                        Log.e(LOGTAG, rate + "Exception, keep trying.",e);
                    }
                }
            }
        }
        return null;
    }

    private void checkRecordPermission() {

        if (ActivityCompat.checkSelfPermission(this, Manifest.permission.RECORD_AUDIO)
                != PackageManager.PERMISSION_GRANTED) {

            ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.RECORD_AUDIO},
                    123);
        }
    }

    public static String IntA2bitS(int[] IntA, int L){
        // Converts an array IntA of L positive 16 bit integers to a bit string bitS
        String bitS = "";
        for (int i = 0; i < L; i++){
            int d = IntA[i];
            for (int j=0; j<16; j++){
                int k =i*16+j;
                bitS = bitS + String.valueOf(d & 1);d = d >> 1;
            }
        }
        return bitS;
    } // end of method

    public void bitS2IntA(String bitS, int L, int[] IntA){
        Log.e(LOGTAG, "string-"+bitS);
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

    public void playSound1(View view) {
        mySound1.start();
    }

    public void stopSound1(View view) {
        mySound1.release();
    }

    public void playSound4(View view) {
        mySound4.start();
    }

    public void stopSound4(View view) {
        mySound4.release();
    }


//  // Following code used when using basic activity
//    @Override
//    //Create an options menu
//    public boolean onCreateOptionsMenu(Menu menu) {
//        // Inflate the menu; this adds items to the action bar if it is present.
//        // Uses res>menu>main.xml
//        getMenuInflater().inflate(R.menu.menu_main, menu);
//        return true;
//    } //End of app onCreateOptionsMenu
//
//    @Override
//    //Called when an item is selected from the options menu
//    public boolean onOptionsItemSelected(MenuItem item) {
//        // Handle action bar item clicks here. The action bar will
//        // automatically handle clicks on the Home/Up button, so long
//        // as you specify a parent activity in AndroidManifest.xml.
//        int id = item.getItemId();
//        if (id == R.id.action_settings) {
//            return true;
//        }
//        return super.onOptionsItemSelected(item);
//    } //End of app onOptionsItemSelected method


}//End of app MainActivity class