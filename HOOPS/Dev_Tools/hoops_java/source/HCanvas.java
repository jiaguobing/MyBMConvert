/*! \package com.techsoft.hoops Contains the HOOPS/Java integration classes */
package com.techsoft.hoops;

import java.awt.*;

/** The HCanvas class provides a HOOPS/3dGS-specific implementation of the java.awt canvas class.
 * HCanvas manages the connection between HOOPS/3dGS driver instance and a java.awt canvas, and is meant to 
 * be used as the base canvas in HOOPS-Java applications that are only using HOOPS/3dGS
 * It contains native entry points for the paint and get_window_handle methods
 */
public class HCanvas extends Canvas
{
    static
    {
        // Load the library that contains the paint code.
        System.loadLibrary("java_awt_canvas2120");
    }
	
    /**
     * Native entry point for painting. It will use only HOOPS/3dGS calls to redraw the scene
     *    
     * @param g Provides the graphics context for the painting
     */
    public native void paint(Graphics g);
        
    /**
     * Native entry point which gets the underlying native window handle associated with the awt canvas.
     * This native window handle is required by HOOPS/3dGS in order to connect to the canvas
     *    
     * 
     * @param g Provides the graphics context for the painting
     * @return handle of a window for hoops to draw into
     */
    public native long get_window_handle(Graphics g);
}


 
                   
   