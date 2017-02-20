package com.traffar.sudoku;
import org.libsdl.app.SDLActivity;

import java.io.File;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.content.pm.ResolveInfo;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.provider.MediaStore;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.VideoView;


public class activity extends SDLActivity
{
   File mediaStorageDir;
   private static activity instance = null;
   private String mCurrentPhotoPath;
   static private final String TAG = "SUDOKU";

   private static final String JPEG_FILE_PREFIX = "IMG_";
   private static final String JPEG_FILE_SUFFIX = ".jpg";

   /* Photo album for this application */
   private String getAlbumName() {
      return "sudoku";
   }


   private File createImageFile() throws IOException {
      // Create an image file name
      // String timeStamp = new SimpleDateFormat("yyyyMMdd_HHmmss").format(new Date());
      // String imageFileName = JPEG_FILE_PREFIX + timeStamp + "_";
      File albumF = mediaStorageDir;
      File imageF = File.createTempFile("sudoku", JPEG_FILE_SUFFIX, albumF);
      return imageF;
   }

   private File setUpPhotoFile() throws IOException {
      File f = createImageFile();
      mCurrentPhotoPath = f.getAbsolutePath();
      return f;
   }

   private void dispatchTakePictureIntent()
   {
      Intent takePictureIntent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
      File f = null;
      try {
         f = setUpPhotoFile();
         mCurrentPhotoPath = f.getAbsolutePath();
         Log.v(TAG, "Current Photo Path: "+ f.getAbsolutePath());
         takePictureIntent.putExtra(MediaStore.EXTRA_OUTPUT, Uri.fromFile(f));
      } catch (IOException e) {
         e.printStackTrace();
         f = null;
         mCurrentPhotoPath = null;
      }
      startActivityForResult(takePictureIntent, 0);
   }

   private void handleBigCameraPhoto() {
      Log.d(TAG, "calling C-API");
      onTakePhotoResult();
      Log.d(TAG, "done calling C-API");

      if (mCurrentPhotoPath != null) {
         mCurrentPhotoPath = null;
      }
   }


   /** Called when the activity is first created. */
   @Override
   public void onCreate(Bundle savedInstanceState) {
      instance = this;
      super.onCreate(savedInstanceState);

      // check the latest photos
      mediaStorageDir = new File(Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_PICTURES), TAG);
      if (!mediaStorageDir.exists() && !mediaStorageDir.mkdirs())
      {
         Log.d(TAG, "failed to create directory");
         return;
      }
   }

   @Override
   protected void onActivityResult(int requestCode, int resultCode, Intent data) {
      if (resultCode == RESULT_OK) {
         handleBigCameraPhoto();
      }
   }

   static public void take_photo()
   {
      Log.v(TAG, "take_photo");
      instance.dispatchTakePictureIntent();
      //return instance.mCurrentPhotoPath;
   }

   public static native void onTakePhotoResult();
}
