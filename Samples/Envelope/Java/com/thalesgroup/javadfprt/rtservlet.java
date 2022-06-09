package com.thalesgroup.javadfprt;
import javax.servlet.http.HttpServlet;

public class rtservlet extends HttpServlet {
	
	static{	   
	    String arch = System.getProperty("os.arch");
	    try
	    {
	        if (arch.equals("x86")) {
	          System.loadLibrary("javadfprt");
	        } else if ((arch.equals("x86_64")) || (arch.equals("amd64"))) {
	          System.loadLibrary("javadfprt_x86_64");
	        } 	     
	    }
	    catch (UnsatisfiedLinkError localUnsatisfiedLinkError)
	    {
	      if (localUnsatisfiedLinkError.getMessage().indexOf("already loaded in another classloader") == -1) {
	        throw localUnsatisfiedLinkError;
	      }
	    }
	  }
        
    }
