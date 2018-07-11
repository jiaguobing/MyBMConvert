/**
 * package com.techsoft.hoops Contains the HOOPS/Java integration classes 
 */
package com.techsoft.hoops;

 
import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.io.*;
import javax.swing.event.MouseInputAdapter;


/**
 * The HJCanvas class provides a HOOPS/MVO-specific implementation of the java.awt canvas class.
 *  
 *  HJCanvas encapsulates the connection of a HOOPS/3dGS driver instance to an AWT Canvas object, which includes
 *  redrawing the scene during exposes/resizes.  It also encapsulates passing of mouse/keyboard events through 
 *  to corresponding MVO operator methods.  
 *  
 *  This is the base class from which a custom HJCanvas class should be derived, and should be used as the canvas
 *  in applications that are using both HOOPS/MVO and HOOPS/3dGS.
 */
public class HJCanvas extends HCanvas
{
    /** 
     * A reference to the HBaseModel object associated with the canvas
    */
    protected HBaseModel m_HModel;

    /** 
     * A reference to the HBaseView object associated with the canvas
     */
    protected HBaseView m_HView;

    /** 
     * A reference to the HSelectionSet object associated with the canvas
     */
    protected HSelectionSet m_HSelectionSet;

    /**
     * Represents the current HBaseOperator object associated with the Panel
     */
    protected HBaseOperator m_HOperator;

    /**
     * Used to store the initialization state of the HOOPS/MVO view
     */
	protected boolean m_initialized;

    /**
     * Constructor which sets the value of initDone variable to false
     */
    public HJCanvas()
    {
		m_initialized = false;
        System.setProperty("sun.awt.noerasebackground", "true"); 
    }
    
    /**
     * Instructs HOOPS to redraw the scene
     * @param g The object representing the graphics context
     */
    public void paint(Graphics g)
    {
        if (m_HView != null)
        {
			HJ.Control_Update_By_Key(m_HView.GetViewKey(), "redraw everything");
			m_HView.Update();
        }
    }
     
    /**
    * Attaches KeyListener, Mouselistener and MouseMotionListener
    */       
    public void enable_listeners()
    {
        MyMouseInputAdapter mouse_adapter = new MyMouseInputAdapter();
        addMouseListener(mouse_adapter );
        addMouseMotionListener(mouse_adapter );
        addMouseWheelListener(mouse_adapter );
        addKeyListener(new MyKeyAdapter());
    }

    /** 
    * The default implementation initializes the HBaseView object and the view key.
    */
    public void Init()
    {
		if (!m_initialized)
		{
			enable_listeners();
			m_initialized = true;
		}
    }
      
    /**
     * Encapsulates mouse event handling
     *
     * This class extends the abstract class MouseInputAdapter which contains the default implementation of MouseEvents
     */
    class MyMouseInputAdapter extends MouseInputAdapter implements MouseWheelListener
    {
         /** 
          * Passes control to the appropriate mouse method depending on which mouse button is pressed
          *
          * @param e Provides data for the java.awt.event.MouseEvent event 
          */
        public void mousePressed(MouseEvent e)
        {
            if (e.getButton() == MouseEvent.BUTTON1)
                    OnLeftButtonDown(e);
            else if (e.getButton() == MouseEvent.BUTTON2)
                    OnMidButtonDown(e);
            else if (e.getButton() == MouseEvent.BUTTON3)
                    OnRightButtonDown(e);
        }

        /** 
         * Passes control to the appropriate mouse method depending on which mouse button is released
         *
         * @param e Provides data for the java.awt.event.MouseEvent event 
         */
        public void mouseReleased(MouseEvent e)
        {
            if (e.getButton() == MouseEvent.BUTTON1)
                    OnLeftButtonUp(e);
            else if (e.getButton() == MouseEvent.BUTTON2)
                    OnMidButtonUp(e);
            else if (e.getButton() == MouseEvent.BUTTON3)
                    OnRightButtonUp(e);
        }
             
         /** 
          * Passes control to the appropriate mouse method depending on which mouse button is double-clicked
          *
          * @param e Provides data for the java.awt.event.MouseEvent event 
          */
        public void mouseClicked(MouseEvent e)
        {
			if (e.getClickCount() == 2)
			{
				if (e.getButton() == MouseEvent.BUTTON1)
						OnLeftButtonDoubleClick(e);
				else if (e.getButton() == MouseEvent.BUTTON2)
						OnMidButtonDoubleClick(e);
				else if (e.getButton() == MouseEvent.BUTTON3)
						OnRightButtonDoubleClick(e);
			}
        }

            /** 
            *  Passes the mouse move info to OnMouseMove
            *
            *  @param e Provides data for the java.awt.event.MouseEvent event 
            */
            public void mouseMoved(MouseEvent e)
            {
               OnMouseMove(e);
            }

            /** 
             *  Passes the mouse drag info to OnMouseMove
             *
             * @param e Provides data for the java.awt.event.MouseEvent event 
             */
            public void mouseDragged(MouseEvent e)
            {
               OnMouseMove(e);
            }
            
            /** 
             *  Passes the mouse wheel info to mouseWheelMoved
             *
             * @param e Provides data for the java.awt.event.MouseWheelEvent event 
             */
            public void mouseWheelMoved(MouseWheelEvent e)            
            {
            	OnMouseWheel(e);            
            }
            
            /**
             * Dispatches the OnLeftButtonDown event to HOOPS/MVO, which will in turn dispatch it to the current HBaseOperator object
             * The state flags are mapped to HOOPS/MVO abstracted flags
             *
             * @param e Provides data for the java.awt.event.MouseEvent event  
             */
            public void OnLeftButtonDown(MouseEvent e)
            {
                HEventInfo hevent = new HEventInfo(m_HView);
                int flags = MapFlags(e);
                hevent.SetPoint(HEventType.HE_LButtonDown, e.getX(), e.getY(), flags);
                m_HView.InjectEvent(hevent);
            }

            /**
             * Dispatches the OnMidButtonDown event to HOOPS/MVO, which will in turn dispatch it to the current HBaseOperator object
             * The state flags are mapped to HOOPS/MVO abstracted flags
             *
             * @param e Provides data for the java.awt.event.MouseEvent event  
             */
            public void OnMidButtonDown(MouseEvent e)
            {
                HEventInfo hevent = new HEventInfo(m_HView);
                int flags = MapFlags(e);
                hevent.SetPoint(HEventType.HE_MButtonDown, e.getX(), e.getY(), flags);
                m_HView.InjectEvent(hevent);
            }

            /**
             * Dispatches the OnRightButtonDown event to HOOPS/MVO, which will in turn dispatch it to the current HBaseOperator object
             * The state flags are mapped to HOOPS/MVO abstracted flags
             *
             * @param e Provides data for the java.awt.event.MouseEvent event  
             */
            public void OnRightButtonDown(MouseEvent e)
            {
                HEventInfo hevent = new HEventInfo(m_HView);
                int flags = MapFlags(e);
                hevent.SetPoint(HEventType.HE_RButtonDown, e.getX(), e.getY(), flags);
                m_HView.InjectEvent(hevent);
            }

            /**
             * Dispatches the OnLeftButtonUp event to HOOPS/MVO, which will in turn dispatch it to the current HBaseOperator object
             * The state flags are mapped to HOOPS/MVO abstracted flags
             *
             * @param e Provides data for the java.awt.event.MouseEvent event  
             */
            public void OnLeftButtonUp(MouseEvent e)
            {
                HEventInfo hevent = new HEventInfo(m_HView);
                int flags = MapFlags(e);
                hevent.SetPoint(HEventType.HE_LButtonUp, e.getX(), e.getY(), flags);
                m_HView.InjectEvent(hevent);
            }

            /**
             * Dispatches the OnMidButtonUp event to HOOPS/MVO, which will in turn dispatch it to the current HBaseOperator object
             * The state flags are mapped to HOOPS/MVO abstracted flags
             *
             * @param e Provides data for the java.awt.event.MouseEvent event  
             */
            public void OnMidButtonUp(MouseEvent e)
            {
                HEventInfo hevent = new HEventInfo(m_HView);
                int flags = MapFlags(e);
                hevent.SetPoint(HEventType.HE_MButtonUp, e.getX(), e.getY(), flags);
                m_HView.InjectEvent(hevent);
            }

            /**
             * Dispatches the OnRightButtonUp event to HOOPS/MVO, which will in turn dispatch it to the current HBaseOperator object
             * The state flags are mapped to HOOPS/MVO abstracted flags
             *
             * @param e Provides data for the java.awt.event.MouseEvent event  
             */
            public void OnRightButtonUp(MouseEvent e)
            {
                HEventInfo hevent = new HEventInfo(m_HView);
                int flags = MapFlags(e);
                hevent.SetPoint(HEventType.HE_RButtonUp, e.getX(), e.getY(), flags);
                m_HView.InjectEvent(hevent);
            }

            /**
             * Dispatches the OnLeftButtonDoubleClick event to HOOPS/MVO, which will in turn dispatch it to the current HBaseOperator object
             * The state flags are mapped to HOOPS/MVO abstracted flags
             *
             * @param e Provides data for the java.awt.event.MouseEvent event  
             */
            public void OnLeftButtonDoubleClick(MouseEvent e)
            {
                HEventInfo hevent = new HEventInfo(m_HView);
                int flags = MapFlags(e);
                hevent.SetPoint(HEventType.HE_LButtonDblClk, e.getX(), e.getY(), flags);
                m_HView.InjectEvent(hevent);
            }

            /**
             * Dispatches the OnMidButtonDoubleClick event to HOOPS/MVO, which will in turn dispatch it to the current HBaseOperator object
             * The state flags are mapped to HOOPS/MVO abstracted flags
             *
             * @param e Provides data for the java.awt.event.MouseEvent event  
             */
            public void OnMidButtonDoubleClick(MouseEvent e)
            {
                HEventInfo hevent = new HEventInfo(m_HView);
                int flags = MapFlags(e);
                hevent.SetPoint(HEventType.HE_MButtonDblClk, e.getX(), e.getY(), flags);
                m_HView.InjectEvent(hevent);
            }

            /**
             * Dispatches the OnRightButtonDoubleClick event to HOOPS/MVO, which will in turn dispatch it to the current HBaseOperator object
             * The state flags are mapped to HOOPS/MVO abstracted flags
             *
             * @param e Provides data for the java.awt.event.MouseEvent event  
             */
            public void OnRightButtonDoubleClick(MouseEvent e)
            {
                HEventInfo hevent = new HEventInfo(m_HView);
                int flags = MapFlags(e);
                hevent.SetPoint(HEventType.HE_RButtonDblClk, e.getX(), e.getY(), flags);
                m_HView.InjectEvent(hevent);
            }

            /**
             * Dispatches the OnMouseMove event to HOOPS/MVO, which will in turn dispatch it to the current HBaseOperator object
             * The state flags are mapped to HOOPS/MVO abstracted flags
             *
             * @param e Provides data for the java.awt.event.MouseEvent event  
             */
            public void OnMouseMove(MouseEvent e)
            {
                HEventInfo hevent = new HEventInfo(m_HView);
                int flags = MapFlags(e);
                hevent.SetPoint(HEventType.HE_MouseMove, e.getX(), e.getY(), flags);
                m_HView.InjectEvent(hevent);
            }          
            
            /**
             * Dispatches the OnMouseWheel event to HOOPS/MVO, which will in turn dispatch it to the current HBaseOperator object
             * The state flags are mapped to HOOPS/MVO abstracted flags
             *
             * @param e Provides data for the java.awt.event.MouseWheelEvent event  
             */
            public void OnMouseWheel(MouseWheelEvent e)
            {                                  
            	int mult = 40;
            
            	HEventInfo hevent = new HEventInfo(m_HView);
            	int flags = MapFlags(e);
            	hevent.SetMouseWheelDelta(mult*e.getUnitsToScroll());             	
            	hevent.SetPoint(HEventType.HE_MouseWheel, e.getX(), e.getY(), flags);
            	m_HView.InjectEvent(hevent);            	            
           
            }        
     }
     
    /** Encapsulates keyboard events 
    *
    * This class extends abstract class KeyAdapter which contains the default implementation of keyboard events
    */
    class MyKeyAdapter extends KeyAdapter
    {
        /**
         * Dispatches the key press events to HOOPS/MVO, which will in turn dispatch them to the current HBaseOperator object
         * The state flags are mapped to HOOPS/MVO abstracted flags
         *
         * @param e Provides data for the java.awt.event.KeyEvent event  
         */
        public void keyTyped(KeyEvent e)
        {
            HEventInfo hevent = new HEventInfo(m_HView);
            HBaseOperator m_HOperator = m_HView.GetCurrentOperator();
            int flags = MapKeyFlags(e);

            hevent.SetKey(HEventType.HE_KeyDown, e.getKeyChar(),1, flags);
            m_HView.InjectEvent(hevent);


            hevent.SetKey(HEventType.HE_KeyUp, e.getKeyChar(),1, flags);
            m_HView.InjectEvent(hevent);
        }    
    }

    /**
    * 
    * Maps the GUI-specific mouse/key state flags to HOOPS/MVO abstracted flags
    *   
    * @param e Provides data for the java.awt.event.MouseEvent event
    * @return variable indicating which MouseButton and Modifier Key is pressed
    */
    public int MapFlags(MouseEvent e)
    {
        int flags = 0;

        // mouse state
        int state = e.getModifiersEx();

        if ( InputEvent.BUTTON1_DOWN_MASK == (state & InputEvent.BUTTON1_DOWN_MASK) ) 
            flags |= HJMVO.MVO_LBUTTON;

        if ( InputEvent.BUTTON2_DOWN_MASK == (state & InputEvent.BUTTON2_DOWN_MASK) ) 
            flags |= HJMVO.MVO_MBUTTON;
        
        if ( InputEvent.BUTTON3_DOWN_MASK == (state & InputEvent.BUTTON3_DOWN_MASK) ) 
            flags |= HJMVO.MVO_RBUTTON;
        
        if ( InputEvent.ALT_DOWN_MASK  == (state & InputEvent.ALT_DOWN_MASK ) ) 
            flags |= HJMVO.MVO_ALT;

         if ( InputEvent.CTRL_DOWN_MASK  == (state & InputEvent.CTRL_DOWN_MASK ) ) 
            flags |= HJMVO.MVO_CONTROL;
        
         if ( InputEvent.SHIFT_DOWN_MASK  == (state & InputEvent.SHIFT_DOWN_MASK ) ) 
            flags |= HJMVO.MVO_SHIFT;    
      
        return flags;
    }
    
       
    /**
     * 
     * Maps the GUI-specific key state flags to HOOPS/MVO abstracted flags
     *  
     * @param e Provides data for the java.awt.event.KeyEvent event
     * @return variable indicating which modifier key is pressed
     */
    public int MapKeyFlags(KeyEvent e)
    {
        int flags =0;
        int state = e.getModifiersEx();

        if ( InputEvent.ALT_DOWN_MASK  == (state & InputEvent.ALT_DOWN_MASK ) ) 
        flags |= HJMVO.MVO_ALT;

        if ( InputEvent.CTRL_DOWN_MASK  == (state & InputEvent.CTRL_DOWN_MASK ) ) 
        flags |= HJMVO.MVO_CONTROL;

        if ( InputEvent.SHIFT_DOWN_MASK  == (state & InputEvent.SHIFT_DOWN_MASK ) ) 
        flags |= HJMVO.MVO_SHIFT;        

        return flags;
     }
        
    /**
     * 
     * Deletes the current operator associated with the HBaseView object (if any) and resets the HBaseView's
     * current operator to the new one
     * 
     * @param new_operator A reference to an HBaseOperator object to be used as the new operator
     */
    public void SetCurrentOperator(HBaseOperator new_operator)
    {
        HBaseOperator current_operator = m_HView.GetCurrentOperator();

        if (current_operator != null)
			current_operator.delete();

        m_HOperator = new_operator;
        m_HView.SetCurrentOperator(m_HOperator);
    }
}

 
    
