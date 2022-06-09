/*****************************************************************************
*
* Demo program for Sentinel LDK update process
*
*
* Copyright (C) 2022 Thales Group. All rights reserved.
*
*
* Sentinel DEMOMA key required
*
*****************************************************************************/
import java.nio.*;
import java.io.*;

import Aladdin.Hasp;
import Aladdin.HaspTime;
import Aladdin.HaspStatus;


class hasp_update {

    public static final String vendorCode = new String(
      "AzIceaqfA1hX5wS+M8cGnYh5ceevUnOZIzJBbXFD6dgf3tBkb9cvUF/Tkd/iKu2fsg9wAysYKw7RMA" +
      "sVvIp4KcXle/v1RaXrLVnNBJ2H2DmrbUMOZbQUFXe698qmJsqNpLXRA367xpZ54i8kC5DTXwDhfxWT" +
      "OZrBrh5sRKHcoVLumztIQjgWh37AzmSd1bLOfUGI0xjAL9zJWO3fRaeB0NS2KlmoKaVT5Y04zZEc06" +
      "waU2r6AU2Dc4uipJqJmObqKM+tfNKAS0rZr5IudRiC7pUwnmtaHRe5fgSI8M7yvypvm+13Wm4Gwd4V" +
      "nYiZvSxf8ImN3ZOG9wEzfyMIlH2+rKPUVHI+igsqla0Wd9m7ZUR9vFotj1uYV0OzG7hX0+huN2E/Id" +
      "gLDjbiapj1e2fKHrMmGFaIvI6xzzJIQJF9GiRZ7+0jNFLKSyzX/K3JAyFrIPObfwM+y+zAgE1sWcZ1" +
      "YnuBhICyRHBhaJDKIZL8MywrEfB2yF+R3k9wFG1oN48gSLyfrfEKuB/qgNp+BeTruWUk0AwRE9XVMU" +
      "uRbjpxa4YA67SKunFEgFGgUfHBeHJTivvUl0u4Dki1UKAT973P+nXy2O0u239If/kRpNUVhMg8kpk7" +
      "s8i6Arp7l/705/bLCx4kN5hHHSXIqkiG9tHdeNV8VYo5+72hgaCx3/uVoVLmtvxbOIvo120uTJbuLV" +
      "TvT8KtsOlb3DxwUrwLzaEMoAQAFk6Q9bNipHxfkRQER4kR7IYTMzSoW5mxh3H9O8Ge5BqVeYMEW36q" +
      "9wnOYfxOLNw6yQMf8f9sJN4KhZty02xm707S7VEfJJ1KNq7b5pP/3RjE0IKtB2gE6vAPRvRLzEohu0" +
      "m7q1aUp8wAvSiqjZy7FLaTtLEApXYvLvz6PEJdj4TegCZugj7c8bIOEqLXmloZ6EgVnjQ7/ttys7VF" +
      "ITB3mazzFiyQuKf4J6+b/a/Y");
    
    public static void main(String argv[]) throws java.io.IOException
    {
      String infos, fname,h2r,h2h;
      String recipient= null;
      int input = 0;
      int status;
      Hasp hasp = new Hasp(Hasp.HASP_DEFAULT_FID);
      char[] filename = new char[256];
      byte[] update_buffer;
      RandomAccessFile f=null,f1=null;
      InputStreamReader reader = new InputStreamReader(System.in);
      BufferedReader in = new BufferedReader(reader);
      String ack_data;
      String updatedata;

      System.out.println("\nThis is a simple demo program for Sentinel LDK update functions");
      System.out.println("Copyright (C) Thales Group. All rights reserved.\n");

      System.out.println("Please choose whether to retrieve Sentinel key");
      System.out.println("(i) information or to  ");
      System.out.println("(u) update a key or to ");
      System.out.println("(d) detach a license or to ");
      System.out.println("(r) rehost a license  (i/u/d/r) or to");
      System.out.println("retrieve (f)ingerprint: ");

      while ((input != 'i') && (input != 'u') && (input!='d') && (input!='r') && (input!='f'))
        input = in.read();

      /*
       * consume the stream
       */
      fname = in.readLine();

      if (input == 'i') {
        /******************************************************************
         * hasp.getInfo        HASP_UPDATEINFO
         *   generate update information on customer site
         */
        String scope = new String("<haspscope>"+
                                  "<license_manager hostname=\"localhost\" />"+
                                  "</haspscope>\n");
        infos = hasp.getInfo(scope, Hasp.HASP_UPDATEINFO, vendorCode);
        status = hasp.getLastError();
    
        switch (status) {
          case HaspStatus.HASP_STATUS_OK:
            System.out.println("\nUpdate information successfully retrieved from key");
            System.out.println("\nPlease enter CustomerToVendor file name (or <enter> for stdout):");

            fname = in.readLine();

            if (fname.length()==0) {
                System.out.println(infos);
            }
            else {
              try {
                f = new RandomAccessFile(fname, "rw");
              }
              catch (IOException e) {
                System.out.println("Error: " + e.getMessage());
                return;
              }
              f.write(infos.getBytes());
              System.out.println("Sentinel key information stored into file " + fname);
            }
            break;
          case HaspStatus.HASP_TOO_MANY_KEYS:
            System.out.println("Too many keys connected\n");
            break;
          case HaspStatus.HASP_INV_FORMAT:
            System.out.println("Invalid XML info format\n");
            break;
          case HaspStatus.HASP_INV_SCOPE:
            System.out.println("Invalid XML scope\n");
            break;
          default:
            System.out.println("hasp_get_info failed with status " + status);
        } /* switch (status) */
      }
      else if(input=='u') 
      {
        /* input was 'u' for update */

        /******************************************************************
        * hasp.update
        *   update Sentinel key
        */

        /* read update information from file */
        System.out.println("Please enter name of available VendorToCustomer file: ");
        /* VendorToCustomer files are generated with help of Sentinel LDK Factory tool */
        fname = in.readLine();
        
        if (fname != null) {
          try {
            f = new RandomAccessFile(fname, "r");
          }
          catch (IOException e) {
            System.out.println("Error: " + e.getMessage());
            return;
          }
          update_buffer = new byte[(int)f.length()];
          if (update_buffer != null) {
            f.read(update_buffer);
            updatedata = new String(update_buffer);
            ack_data = hasp.update(updatedata);
            status = hasp.getLastError();

            switch (status) {
              case HaspStatus.HASP_STATUS_OK:
                System.out.println("Key successfully updated");
                if (ack_data != null) {
                  /* save acknowledge data in file */
                  f1 = new RandomAccessFile("hasp_ack.c2v", "rw");
                  f1.write(ack_data.getBytes());
                  System.out.println("acknowledge data written to file hasp_ack.c2v\n");
                }  
                break;
              case HaspStatus.HASP_INV_HND:
                System.out.println("handle not active\n"); 
                break;
              case HaspStatus.HASP_INV_FORMAT:
                System.out.println("unrecognized format\n");
                break;
              case HaspStatus.HASP_INV_UPDATE_DATA:    
                System.out.println("invalid update data\n");
                break;
              case HaspStatus.HASP_INV_UPDATE_NOTSUPP: 
                System.out.println("key does not support updates\n");
                break;
              case HaspStatus.HASP_INV_UPDATE_CNTR:    
                System.out.println("invalid update counter\n");
                break;
              default:
                System.out.println("hasp_update failed with status " + status);
            }
          }
        } 
        else {
            System.out.println("could not open file\n");
        }
      }
        /* input was 'd' for detach */

        /******************************************************************
        * hasp.transfer
        *   detach the license
        */
       /*  using hasp_transfer function for detaching a license*/


      else if (input == 'd')
      {
        String scope = new String("<haspscope>"+
                                  "<license_manager hostname=\"localhost\" />"+
                                  "</haspscope>\n");

        String action= new String("<detach>"+
                                  "<duration>120</duration>" +
                                  "</detach>\n");

        String detach_scope=new String("<haspscope>" +
                                       "<product id=\"123\" />" + 
                                       "</haspscope> \n");
        /******************************************************************
        * hasp.getInfo        HASP_RECIPIENT
        *   generate information of the recipient 
        */
        
        infos = hasp.getInfo(scope, Hasp.HASP_RECIPIENT, vendorCode);
        status = hasp.getLastError();
        
        /* check if operation was successful */
        if (status != HaspStatus.HASP_STATUS_OK)
        {
          switch (status)
          {
            case HaspStatus.HASP_TOO_MANY_KEYS:
                System.out.println("Too many keys connected\n");
                break;
            case HaspStatus.HASP_INV_FORMAT:
                System.out.println("Invalid XML info format\n");
                break;
            case HaspStatus.HASP_INV_SCOPE:
                System.out.println("Invalid XML scope\n");
                break;
            default:
                System.out.println("hasp_get_info failed with status " + status);
          } /* switch (status) */
          return;
        }

        System.out.println("Receipent information successfully retreived from key");
        recipient= new String(infos);
        /*************************************************************************************************
         Use of hasp.transfer function to detach a license successfully
        */
       
        h2r = hasp.transfer(action,detach_scope,vendorCode,recipient);
        status = hasp.getLastError();
        switch (status)
        {
          case HaspStatus.HASP_STATUS_OK:
            {
               System.out.println("Successfully retrieving the h2r file");
               System.out.println("\nPlease enter h2r file name (or <enter> for stdout):");

               fname = in.readLine();

               if (fname.length() == 0)
               {
                   System.out.println(h2r);
               }
               else
               {
                   try
                   {
                       f = new RandomAccessFile(fname, "rw");
                   }
                   catch (IOException e)
                   {
                       System.out.println("Error: " + e.getMessage());
                       return;
                   }
                   f.write(h2r.getBytes());
                   System.out.println("Sentinel key information stored into file " + fname);
               }
               break;

            }

          case HaspStatus.HASP_INV_DETACH_ACTION:
            System.out.println("Invalid XML detach_action parameter\n");
            break;

          case HaspStatus.HASP_INV_RECIPIENT:
            System.out.println("Invalid XML recipient parameter\n");
            break;

          case HaspStatus.HASP_TOO_MANY_PRODUCTS:
            System.out.println("Scope for hasp_detach does not select a unique Parameter\n");
            break;

          case HaspStatus.HASP_ACCESS_DENIED:
            System.out.println("Request not possible because of ACC restrictions\n");
            break;

          case HaspStatus.HASP_INV_PRODUCT:
            System.out.println("Invalid Product information\n");
            break;

          case HaspStatus.HASP_INSUF_MEM:
            System.out.println("Out of memory\n");
            break;

          case HaspStatus.HASP_DEVICE_ERR:
            System.out.println("Input/Output error\n");
            break;

          case HaspStatus.HASP_LOCAL_COMM_ERR:
            System.out.println("Communication error\n");
            break;

          case HaspStatus.HASP_REMOTE_COMM_ERR:
            System.out.println("Remote communication error\n");
            break;

          case HaspStatus.HASP_INV_SCOPE:
            System.out.println("Unrecognized scope string\n");
            break;

          default:
            System.out.println("hasp_transfer detach failed with status " + status);
        
        }/* switch (status) */
        
      }

      /* input was 'r' for rehost */

        /******************************************************************
        * hasp.transfer
        *   rehost the license
        */
       /*  using hasp_transfer function for rehosting a license*/




      else if(input=='r')
      {

        String scope = new String("<haspscope>"+
                                  " <license_manager hostname=\"localhost\" />"+
                                  "</haspscope>\n");

        String action= new String("<rehost>"+
                                      "<hasp id=\"1234567\"/>"+
                                  "</rehost>\n");

        String rehost_scope=new String("<haspscope>"+
                                       "<hasp id=\"1234567\"/>"+
                                       "</haspscope> \n");
        /******************************************************************
         * hasp.getInfo        HASP_RECIPIENT
         *   generate information of the recipient 
         */
        
        infos = hasp.getInfo(scope, Hasp.HASP_RECIPIENT, vendorCode);
        status = hasp.getLastError();
    
        /* check if operation was successful */
        if (status != HaspStatus.HASP_STATUS_OK)
        {
          switch (status)
          {
            case HaspStatus.HASP_TOO_MANY_KEYS:
                System.out.println("Too many keys connected\n");
                break;
            case HaspStatus.HASP_INV_FORMAT:
                System.out.println("Invalid XML info format\n");
                break;
            case HaspStatus.HASP_INV_SCOPE:
                System.out.println("Invalid XML scope\n");
                break;
            default:
                System.out.println("hasp_get_info failed with status " + status);
          } /* switch (status) */

          return;
        }
        
        System.out.println("\nReceipent information successfully retreived from key\n");
        recipient = new String(infos);
           
     
        /*************************************************************************************************
        Usuage of hasp.transfer function to rehost a license successfully
        */
       
        h2h = hasp.transfer(action,rehost_scope,vendorCode,recipient);
        status = hasp.getLastError();
        switch(status)
        {
          case HaspStatus.HASP_STATUS_OK:
          {
            System.out.println("\nSuccessfully retrieving the h2h file");
            System.out.println("\nPlease enter h2h file name (or <enter> for stdout):");
            fname = in.readLine();
            if (fname.length()==0) {
              System.out.println(h2h);
            }
            else {
              try {
                f = new RandomAccessFile(fname, "rw");
              }
              catch (IOException e) {
                System.out.println("Error: " + e.getMessage());
                return;
              }
              f.write(h2h.getBytes());
              System.out.println("\nSentinel key information stored into file " + fname);
            }
            break;
          }
          case HaspStatus.HASP_INV_RECIPIENT:
            System.out.println("\nInvalid XML recipient parameter");
            break;

          case HaspStatus.HASP_ACCESS_DENIED:
            System.out.println("Request not possible because of ACC restrictions\n");
            break;

          case HaspStatus.HASP_INSUF_MEM:
            System.out.println("Out of memory\n");
            break;

          case HaspStatus.HASP_DEVICE_ERR:
            System.out.println("Input/Output error\n");
            break;

          case HaspStatus.HASP_LOCAL_COMM_ERR:
            System.out.println("Communication error\n");
            break;

          case HaspStatus.HASP_REMOTE_COMM_ERR:
            System.out.println("Remote communication error\n");
            break;

          case HaspStatus.HASP_INV_SCOPE:
            System.out.println("Unrecognized scope string\n");
            break;

          case HaspStatus.HASP_REHOST_NOT_ALLOWED:
            System.out.println("Rehost not allowed\n");
            break;

          case HaspStatus.HASP_TOO_MANY_KEYS:
            System.out.println("Scope for hasp_transfer does not specify a unique Parameter\n");
            break;

          case HaspStatus.HASP_OLD_LM:
            System.out.println("LMS not supported to SL-AdminMode/SL-UserMode licenses\n");
            break;

          case HaspStatus.HASP_HASP_INACTIVE:
            System.out.println("SL-AM/SL-UM container is inactive\n");
            break;

          case HaspStatus.HASP_CLONE_DETECTED:
            System.out.println("Sentinel Clone Detected\n");
            break;

          case HaspStatus.HASP_REQ_NOT_SUPP:
              System.out.println("Rehost request supports  for SL-AdminMode/SL-UserMode licenses\n");
            break;

          default:
            System.out.println("hasp_transfer() rehost failed with status " + status);
         
        }/* switch (status) */
      }

      if (input == 'f') {
        /******************************************************************
         * hasp.getInfo        HASP_FINGERPRINT
         *   retrieve host fingerprint information on customer site
         */
        String scope = new String("<haspscope>"+
                                  "<license_manager hostname=\"localhost\" />"+
                                  "</haspscope>\n");
        infos = hasp.getInfo(scope, Hasp.HASP_FINGERPRINT, vendorCode);
        status = hasp.getLastError();
    
        switch (status) {
          case HaspStatus.HASP_STATUS_OK:
            System.out.println("\nFingerprint information successfully retrieved from host");
            System.out.println("\nPlease enter CustomerToVendor file name (or <enter> for stdout):");

            fname = in.readLine();

            if (fname.length()==0) {
                System.out.println(infos);
            }
            else {
              try {
                f = new RandomAccessFile(fname, "rw");
              }
              catch (IOException e) {
                System.out.println("Error: " + e.getMessage());
                return;
              }
              f.write(infos.getBytes());
              System.out.println("Fingerprint information stored into file " + fname);
            }
            break;
          case HaspStatus.HASP_TOO_MANY_KEYS:
            System.out.println("Too many keys connected\n");
            break;
          case HaspStatus.HASP_INV_FORMAT:
            System.out.println("Invalid XML info format\n");
            break;
          case HaspStatus.HASP_INV_SCOPE:
            System.out.println("Invalid XML scope\n");
            break;
          default:
            System.out.println("hasp_get_info failed with status " + status);
        } /* switch (status) */
      }
      return;
  }
}
