package com.groupsinc.NativeDialog;

import android.app.Activity;
import android.content.Intent;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.net.Uri;
import android.os.Bundle;
import android.provider.MediaStore;
import android.util.Log;

import java.io.*;
import java.util.ArrayList;

public class ImagePickerActivity extends Activity
{
    protected final String TAG = "ImagePickerActivity";
    protected final int REQUEST_CODE_FOR_PICK = 1;
    protected final int REQUEST_CODE_FOR_CAMERA = 2;

    private long m_id;

    public ImagePickerActivity() {}

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        Bundle extras = getIntent().getExtras();
        m_id = extras.getLong("id");

        if (extras.getBoolean("cameraMode"))
            openCamera();
        else
            openImagePicker();
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (requestCode == REQUEST_CODE_FOR_PICK) {
            if (resultCode == RESULT_OK && data != null) {
                Uri selectedImage = data.getData();
                String[] filePathColumn = { MediaStore.Images.Media.DATA };

                Cursor cursor = getContentResolver().query(selectedImage, filePathColumn, null, null, null);
                cursor.moveToFirst();

                int columnIndex = cursor.getColumnIndex(filePathColumn[0]);
                String imagePath = cursor.getString(columnIndex);
                cursor.close();

                Log.d(TAG, "Image selected: " + imagePath);

                processImage(imagePath);
            } else {
                Log.d(TAG, "Image selected failed.");
                imageSelectCancel(m_id);
            }
        } else if (requestCode == REQUEST_CODE_FOR_CAMERA) {
            if (resultCode == RESULT_OK && data != null) {
                Bitmap image = (Bitmap) data.getExtras().get("data");
                File outFile = new File(getCacheDir(), "Image.jpg");

                try {
                    FileOutputStream out = new FileOutputStream(outFile);
                    image.compress(Bitmap.CompressFormat.JPEG, 80, out);

                    imageSelected(m_id, outFile.getAbsolutePath());

                } catch (FileNotFoundException e) {
                    Log.d(TAG, "Image saving failed.");
                    e.printStackTrace();
                    imageError(m_id, e.getMessage());
                }
            } else {
                Log.d(TAG, "Image selected failed. There is no data.");
                imageSelectCancel(m_id);
            }
        }

        super.onActivityResult(requestCode, resultCode, data);
        finish();
    }

    public void openImagePicker() {
        Intent i = new Intent(Intent.ACTION_PICK, android.provider.MediaStore.Images.Media.EXTERNAL_CONTENT_URI);
        startActivityForResult(i, REQUEST_CODE_FOR_PICK);
    }

    public void openCamera() {
        Intent i = new Intent(android.provider.MediaStore.ACTION_IMAGE_CAPTURE);
        startActivityForResult(i, REQUEST_CODE_FOR_CAMERA);
    }

    private void processImage(String imagePath) {
        try {
            // copy to cache directory
            File inFile = new File(imagePath);
            InputStream in = new FileInputStream(inFile);

            File outFile = new File(getCacheDir(), inFile.getName());
            OutputStream out = new FileOutputStream(outFile);

            byte[] buf = new byte[1024];
            int len;
            while ((len = in.read(buf)) > 0)
                out.write(buf, 0, len);

            in.close();
            out.close();

            imagePath = outFile.getAbsolutePath();
            Log.d(TAG, "Selected image saved to cache: " + imagePath);

            imageSelectStarted(m_id);
            imageSelected(m_id, imagePath);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
            imageError(m_id, e.getMessage());
        } catch (IOException e) {
            e.printStackTrace();
            imageError(m_id, e.getMessage());
        }
    }

    public native void imageSelected(long id, String imagePath);
    public native void imageSelectStarted(long id);
    public native void imageSelectCancel(long id);
    public native void imageError(long id, String message);
}
