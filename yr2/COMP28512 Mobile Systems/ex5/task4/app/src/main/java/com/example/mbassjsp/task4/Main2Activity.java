package com.example.mbassjsp.task4;

import android.annotation.SuppressLint;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.text.method.ScrollingMovementMethod;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import static com.example.mbassjsp.task4.R.raw.damagedpeppers;
import static com.example.mbassjsp.task4.R.raw.damagedpeppersjpg;
import static com.example.mbassjsp.task4.R.raw.peppers;
import static com.example.mbassjsp.task4.R.raw.peppersjpg;
import static com.example.mbassjsp.task4.R.raw.verydamagedpeppers;

public class Main2Activity extends AppCompatActivity {

    private static final String LOGTAG = "Minor UI"; //Logcat messages from UI are identified
    Button bt2_1;

    void msImage(String filename, int id){ // Reads from an image file into an array for image processing in Java.
        try {
            int res_id = getResources().getIdentifier(filename, "raw", getPackageName() );
            InputStream image_is = getResources().openRawResource(res_id);
            int filesize = image_is.available(); //Get image file size in bytes
            byte[ ] image_array = new byte[filesize]; //Create array to hold image
            image_is.read(image_array);                 //Load image into array
            image_is.close();
            // Close in-out file streamR.id.imageView1
            // Add your code here to process image_array & save processed version to a file.
            File newImageFile = new File(getFilesDir(), filename);
            OutputStream image_os = new FileOutputStream(newImageFile);
            image_os.write(image_array, 0, filesize);
            image_os.flush();
            image_os.close();
            //Display the processed image-file
            Bitmap newBitmap = BitmapFactory.decodeFile(newImageFile.getAbsolutePath());
            // Create ImageView object
            ImageView image = (ImageView) findViewById(id);
            image.setImageBitmap(newBitmap);
        } // end of try
        catch (FileNotFoundException e) { Log.e("tag", "File not found", e);}
        catch (IOException e) { Log.e("tag", "Failed for stream", e); }
    } //end of msImage me

    @SuppressLint("ResourceType")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main2);

        //Get image display area as set in the xml file//
        ImageView image1 = (ImageView) findViewById(R.id.imageView1);
        //Display peppers.bmp from res/raw directory//Math.pow(Math.E,j-i*200)
        image1.setImageResource(peppers);
        //Call msImage method to process the image:
        msImage("peppers",R.id.imageView1);

        //Get image display area as set in the xml file//
        ImageView image2 = (ImageView) findViewById(R.id.imageView2);
        //Display peppers.bmp from res/raw directory//
        image2.setImageResource(peppersjpg);
        //Call msImage method to process the image:
        msImage("peppersjpg",R.id.imageView2);

        ImageView image3 = (ImageView) findViewById(R.id.imageView3);
        //Display peppers.bmp from res/raw directory//
        image3.setImageResource(damagedpeppers);
        //Call msImage method to process the image:
        msImage("damagedpeppers",R.id.imageView3);

        ImageView image4 = (ImageView) findViewById(R.id.imageView4);
        //Display peppers.bmp from res/raw directory//
        image4.setImageResource(damagedpeppersjpg);
        //Call msImage method to process the image:
        msImage("damagedpeppersjpg",R.id.imageView4);

        ImageView image5 = (ImageView) findViewById(R.id.imageView5);
        //Display peppers.bmp from res/raw directory//
        image5.setImageResource(verydamagedpeppers);
        //Call msImage method to process the image:
        msImage("verydamagedpeppers",R.id.imageView5);

        bt2_1 = (Button) findViewById(R.id.btnchg);
        Button.OnClickListener listener = new Button.OnClickListener() {

            public void onClick(View v) {
                Intent intent = new Intent(Main2Activity.this, MainActivity.class);
                startActivity(intent);
                Main2Activity.this.finish();

            }

        };
        bt2_1.setOnClickListener(listener);

    }
}
