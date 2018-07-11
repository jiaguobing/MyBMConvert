using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

/*! \namespace hoops_panel Contains the HNPanel class */
namespace hoops_panel
{
    //! The HNPanel class provides a HOOPS/MVO-specific implementation of the Winforms Panel class.
    /*!
     * HNPanel creates and manages the connection of a HOOPS/3dGS driver instance to a Panel object, which includes
	 * redrawing the scene during exposes/resizes. It also encapsulates passing of mouse/keyboard events through 
     * to corresponding MVO operator methods.  
     * 
     * This is the base class from which a custom HNPanel class should be derived, and should be used when the application
     * is using both HOOPS/MVO and HOOPS/3dGS
    */
    public class HNPanel : Panel
    {
        /*! 
         * A reference to the HBaseModel object associated with the panel
        */
        public HBaseModel m_pHModel;

        /*! 
         * A reference to the HBaseView object associated with the panel
         */
        public HBaseView m_pHView;

        /*! 
         * A reference to the HSelectionSet object associated with the panel
         */
        public HSelectionSet m_pHSelection;

        /*! 
         * Used to create window-handle for the panel
         */
        public IntPtr winid;

        /*!
         * Represents the current HBaseOperator object associated with the Panel
         */
        protected HBaseOperator m_pHOperator;
        
        /*!
         * Flag to keep track of the last key that was pressed
         */
        protected int m_lastKeyPressed;

        /*!
         * Array to associate KeyDown key values with KeyPress characters
         */
        protected int[] m_keyPairs;


        /*!
         * Constructor which attaches paint, panel-resize and MouseEvents with corresponding EventHandlers
         */
        public HNPanel()
            : base()
        {
            winid = this.Handle;// Create window handle of the panel and assign it to winid              
            m_keyPairs = new int[255];
        }
      
        /*! 
         * The default implementation initializes the HBaseView object and the view key.
         */
        public void Init()
        {
            if (m_pHView == null)
                return;
            long key = m_pHView.GetViewKey();
            HCS.Open_Segment_By_Key(m_pHView.GetViewKey());
            HCS.Set_Driver_Options("debug = 0x00000040");
            HCS.Close_Segment();
        }
      
        /*!
          * Instructs HOOPS to redraw the scene
          *
          * \param sender A reference to an object which is the root of the type hierarchy 
          * \param e Provides data for the System.Windows.Forms.Control.paint event 
          */
        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);

            if (!this.Disposing && (m_pHView != null))
            {
                HCS.Control_Update_By_Key(m_pHView.GetViewKey(), "redraw everything");
                m_pHView.Update();
            }
        }

        protected override void OnPaintBackground(PaintEventArgs e)
        {
            
        }

        /*! 
         * Ensures that view in the Panel is redraw whenever it is resized
         *
         * \param sender A reference to an object  which is the root of the type hierarchy 
         * \param e Provides data for the event 
         */
        protected override void OnResize(EventArgs eventargs)
        {
            base.OnResize(eventargs);

            if (!this.Disposing && (m_pHView != null))
            {
                HCS.Control_Update_By_Key(m_pHView.GetViewKey(), "redraw everything");

                m_pHView.AdjustAxisWindow();
                m_pHView.Update();
            }
        }

        /*! 
         * Passes control to the method depending on which Mouse Button is pressed
         *
         * \param sender A reference to an object which is the root of the type hierarchy 
         * \param e Provides data for the System.Windows.Forms.Control.MouseDown event 
         */
        protected override void OnMouseDown(MouseEventArgs e)
        {
            base.OnMouseDown(e);

            if (e.Button == MouseButtons.Left)
                OnLeftButtonDown(e);
            else if (e.Button == MouseButtons.Right)
                OnRightButtonDown(e);
            else if (e.Button == MouseButtons.Middle)
                OnMidButtonDown(e);
        }        

        /*! 
         * Passes control to the method depending on which Mouse Button is double clicked
         *
         * \param sender A reference to an object which is the root of the type hierarchy 
         * \param e Provides data for the System.Windows.Forms.Control.MouseDoubleClick event 
         */

        protected override void OnMouseDoubleClick(MouseEventArgs e)
        {
            base.OnMouseDoubleClick(e);

            if (e.Button == MouseButtons.Left)
                OnLeftButtonDoubleClick(e);
            else if (e.Button == MouseButtons.Right)
                OnRightButtonDoubleClick(e);
            else if (e.Button == MouseButtons.Middle)
                OnMidButtonDoubleClick(e);
        }     

        /*! 
         * Passes control to the method depending on which Mouse Button is Released
         *
         * \param sender A reference to an object which is the root of the type hierarchy
         * \param e Provides data for the System.Windows.Forms.Control.MouseUp event 
         */
        protected override void OnMouseUp(MouseEventArgs e)
        {
            base.OnMouseUp(e);

            if (e.Button == MouseButtons.Left)
                OnLeftButtonUp(e);
            else if (e.Button == MouseButtons.Right)
                OnRightButtonUp(e);
            else if (e.Button == MouseButtons.Middle)
                OnMidButtonUp(e);
        }

        /*!
         * Responds to MouseMove events and passes particular co-ordinates to hoops
         *
         * \param sender A reference to an object which is the root of the type hierarchy
         * \param e Provides data for the System.Windows.Forms.Control.MouseMove event 
         */
        protected override void OnMouseMove(MouseEventArgs e)
        {
            base.OnMouseMove(e);

            HEventInfo hevent = new HEventInfo(m_pHView);

            uint flags = MapFlags_Mouse(e);
            hevent.SetPoint(HEventType.HE_MouseMove, e.X, e.Y, flags);

            m_pHView.InjectEvent(hevent);
        }


        /*!
         * Dispatches the OnLeftButtonDown event to HOOPS/MVO, which will in turn dispatch it to the current HBaseOperator object
         * The state flags are mapped to HOOPS/MVO abstracted flags
         *
         * \param e Provides data for the System.Windows.Forms.Control.MouseDown event 
         */
        protected virtual void OnLeftButtonDown(MouseEventArgs e)
        {
            HEventInfo hevent = new HEventInfo(m_pHView);
            uint flags = MapFlags_Mouse(e);
            hevent.SetPoint(HEventType.HE_LButtonDown, e.X, e.Y, flags);
            m_pHView.InjectEvent(hevent);
        }

        /*!
         * Dispatches the OnLeftButtonUp event to HOOPS/MVO, which will in turn dispatch it to the current HBaseOperator object
         * The state flags are mapped to HOOPS/MVO abstracted flags
         *
         * \param e Provides data for the System.Windows.Forms.Control.MouseUp event 
         */
        protected virtual void OnLeftButtonUp(MouseEventArgs e)
        {
            HEventInfo hevent = new HEventInfo(m_pHView);
            uint flags = MapFlags_Mouse(e);
            hevent.SetPoint(HEventType.HE_LButtonUp, e.X, e.Y, flags);
            m_pHView.InjectEvent(hevent);
        }

        /*!
         * Dispatches the OnLeftButtonDoubleClick event to HOOPS/MVO, which will in turn dispatch it to the current HBaseOperator object
         * The state flags are mapped to HOOPS/MVO abstracted flags
         *
         * \param e Provides data for the System.Windows.Forms.Control.MouseDoubleClick event 
         */
        protected virtual void OnLeftButtonDoubleClick(MouseEventArgs e)
        {
            HEventInfo hevent = new HEventInfo(m_pHView);
            uint flags = MapFlags_Mouse(e);
            hevent.SetPoint(HEventType.HE_LButtonDblClk, e.X, e.Y, flags);
            m_pHView.InjectEvent(hevent);
        }

        /*!
         * Dispatches the OnRightButtonDown event to HOOPS/MVO, which will in turn dispatch it to the current HBaseOperator object
         * The state flags are mapped to HOOPS/MVO abstracted flags
         *
         * \param e Provides data for the System.Windows.Forms.Control.MouseDown event 
        */
        protected virtual void OnRightButtonDown(MouseEventArgs e)
        {
            HEventInfo hevent = new HEventInfo(m_pHView);
            uint flags = MapFlags_Mouse(e);
            hevent.SetPoint(HEventType.HE_RButtonDown, e.X, e.Y, flags);
            m_pHView.InjectEvent(hevent);
        }

        /*!
         * Dispatches the OnRightButtonUp event to HOOPS/MVO, which will in turn dispatch it to the current HBaseOperator object
         * The state flags are mapped to HOOPS/MVO abstracted flags
         *
         * \param e Provides data for the System.Windows.Forms.Control.MouseUp event 
         */
        protected virtual void OnRightButtonUp(MouseEventArgs e)
        {
            HEventInfo hevent = new HEventInfo(m_pHView);
            uint flags = MapFlags_Mouse(e);
            hevent.SetPoint(HEventType.HE_RButtonUp, e.X, e.Y, flags);
            m_pHView.InjectEvent(hevent);
        }

        /*!
         * Dispatches the OnRightButtonDoubleClick event to HOOPS/MVO, which will in turn dispatch it to the current HBaseOperator object
         * The state flags are mapped to HOOPS/MVO abstracted flags
         *
         * \param e Provides data for the System.Windows.Forms.Control.MouseDoubleClick event 
         */
        protected virtual void OnRightButtonDoubleClick(MouseEventArgs e)
        {
            HEventInfo hevent = new HEventInfo(m_pHView);
            uint flags = MapFlags_Mouse(e);
            hevent.SetPoint(HEventType.HE_RButtonDblClk, e.X, e.Y, flags);
            m_pHView.InjectEvent(hevent);
        }

        /*!
         * Dispatches the OnMidButtonDown event to HOOPS/MVO, which will in turn dispatch it to the current HBaseOperator object
         * The state flags are mapped to HOOPS/MVO abstracted flags
         *
         * \param e Provides data for the System.Windows.Forms.Control.MouseDown event 
         */
        protected virtual void OnMidButtonDown(MouseEventArgs e)
        {
            HEventInfo hevent = new HEventInfo(m_pHView);
            uint flags = MapFlags_Mouse(e);
            hevent.SetPoint(HEventType.HE_MButtonDown, e.X, e.Y, flags);
            m_pHView.InjectEvent(hevent);
        }

        /*!
         * Dispatches the OnMidButtonUp event to HOOPS/MVO, which will in turn dispatch it to the current HBaseOperator object
         * The state flags are mapped to HOOPS/MVO abstracted flags
         *
         * \param e Provides data for the System.Windows.Forms.Control.MouseUp event 
         */
        protected virtual void OnMidButtonUp(MouseEventArgs e)
        {
            HEventInfo hevent = new HEventInfo(m_pHView);
            uint flags = MapFlags_Mouse(e);
            hevent.SetPoint(HEventType.HE_MButtonUp, e.X, e.Y, flags);
            m_pHView.InjectEvent(hevent);
        }

        /*!
         * Dispatches the OnMidButtonDoubleClick event to HOOPS/MVO, which will in turn dispatch it to the current HBaseOperator object
         * The state flags are mapped to HOOPS/MVO abstracted flags
         *
         * \param e Provides data for the System.Windows.Forms.Control.MouseDoubleClick event 
         */
        protected virtual void OnMidButtonDoubleClick(MouseEventArgs e)
        {
            HEventInfo hevent = new HEventInfo(m_pHView);
            uint flags = MapFlags_Mouse(e);
            hevent.SetPoint(HEventType.HE_MButtonDblClk, e.X, e.Y, flags);
            m_pHView.InjectEvent(hevent);
        }

        /*! 
         * Maps the GUI-specific state flags to HOOPS/MVO abstracted flags
         *
         * \param e Provides data for the System.Windows.Forms.Control.MouseUp, MouseDown, MouseMove event 
         */
        protected uint MapFlags_Mouse(MouseEventArgs e)
        {
            uint flags = new uint();

            if (e.Button == MouseButtons.Left) flags |= (uint)HCSMVO.MVO_LBUTTON;
            if (e.Button == MouseButtons.Right) flags |= (uint)HCSMVO.MVO_RBUTTON;
            if (e.Button == MouseButtons.Middle) flags |= (uint)HCSMVO.MVO_MBUTTON;

            return MapModifierKeys(flags);
        }

        /*! 
         * Returns appropriate flag depending on whether shift, control or alt key is pressed
         *
         * \param flags Provides information about which key is pressed or released 
         * \return uint value which contains the information about which mouse button is pressed or released              
         *              and which modifier key is pressed
         */
        protected uint MapModifierKeys(uint flags)
        {
            if ((Control.ModifierKeys & Keys.Shift) != 0) flags |= (1 << 2);
            if ((Control.ModifierKeys & Keys.Control) != 0) flags |= (1 << 1);
            if ((Control.ModifierKeys & Keys.Alt) != 0) flags |= (1 << 3);

            return flags;
        }


        /*!
         * This method is called from HNForm OnKeyUp method
         *
         * It dispatches the OnKeyUp event to HOOPS/MVO, which will in turn dispatch it to the current HBaseOperator object
         * The state flags are mapped to HOOPS/MVO abstracted flags
         *
         * \param e Provides data for System.Windows.Forms.Control.KeyUp event
         */
        public new void OnKeyUp(KeyEventArgs e)
        {
            if (m_pHView != null)
            {
                HEventInfo hevent = new HEventInfo(m_pHView);
                uint flags = MapModifierKeys(0);
                hevent.SetKey(HEventType.HE_KeyUp, (uint)m_keyPairs[e.KeyValue], 1, flags);
                m_pHView.InjectEvent(hevent);

            }
        }

        /*!
         * This method is called from HNForm OnKeyDown method
         *
         * It dispatches the OnKeyDown event to HOOPS/MVO, which will in turn dispatch it to the current HBaseOperator object
         * The state flags are mapped to HOOPS/MVO abstracted flags
         *
         * \param e Provides data for System.Windows.Forms.Control.KeyDown event
         */
        public new void OnKeyDown(KeyEventArgs e)
        {
            if (m_pHView != null)
            {
                m_lastKeyPressed = e.KeyValue;

                if (e.KeyCode == Keys.ShiftKey || e.KeyCode == Keys.ControlKey)
                {
                    HEventInfo hevent = new HEventInfo(m_pHView);
                    uint flags = MapModifierKeys(0);
                    hevent.SetKey(HEventType.HE_KeyDown, 0, 1, flags);
                    m_pHView.InjectEvent(hevent);
                }
            }
        }


        /*!
         * This method is called from HNForm OnKeyPress method
         *
         * It dispatches the OnKeyDown event to HOOPS/MVO, which will in turn dispatch it to the current HBaseOperator object
         * The state flags are mapped to HOOPS/MVO abstracted flags
         *
         * \param e Provides data for System.Windows.Forms.Control.KeyPress event
         */
        public new void OnKeyPress(KeyPressEventArgs e)
        {
            if (m_pHView != null)
            {
                m_keyPairs[m_lastKeyPressed] = e.KeyChar;

                HEventInfo hevent = new HEventInfo(m_pHView);
                uint flags = MapModifierKeys(0);
                hevent.SetKey(HEventType.HE_KeyDown, (uint)e.KeyChar, 1, flags);
                m_pHView.InjectEvent(hevent);
            }
        }

        /*!
         * This method is called from HNForm OnMouseWheel method
         * 
         * It dispatches the OnMouseWheel event to HOOPS/MVO, which will in turn dispatch it to the current HBaseOperator object
         * The state flags are mapped to HOOPS/MVO abstracted flags
         *
         * \param e Provides data for System.Windows.Forms.Control.MouseWheel
         */
        public new void OnMouseWheel(MouseEventArgs e)
        {
            HEventInfo hevent = new HEventInfo(m_pHView);

            hevent.SetMouseWheelDelta(e.Delta);
            uint flags = MapFlags_Mouse(e);
            hevent.SetPoint(HEventType.HE_MouseWheel, e.X, e.Y, flags);

            m_pHView.InjectEvent(hevent);
        }

        /*! 
         * Deletes the current operator associated with the HBaseView object (if any) and resets the HBaseView's
	     * current operator to the new one
         * \param new_operator A reference to an HBaseOperator object to be used as the new operator
         */
        public virtual void SetCurrentOperator(HBaseOperator new_operator)
        {
            HBaseOperator current_operator = m_pHView.GetCurrentOperator();
            if (current_operator != null)
                current_operator.Dispose();

            if (m_pHOperator != null)
                ((HBaseOperator)m_pHOperator).Dispose();

            m_pHOperator = new_operator;
            m_pHView.SetCurrentOperator((HBaseOperator)m_pHOperator);

        }

        protected override void Dispose(bool disposing)
        {
            if (m_pHOperator != null)
            {
                m_pHOperator.Dispose();
                m_pHOperator = null;
            }

            if (m_pHSelection != null)
            {
                m_pHSelection.Dispose();
                m_pHSelection = null;
                m_pHView.SetSelection((HSelectionSet)null);
            }

            if (m_pHView != null)
            {
                m_pHView.Dispose();
                m_pHView = null;
            }

            if (m_pHModel != null)
            {
                m_pHModel.Dispose();
                m_pHModel = null;
            }

            base.Dispose(disposing);
        }       

        /*! 
         *  Called when focus leaves the Panel.  Resets the HBaseView object's key state.
         */
        public new void LostFocus()
        {
            if (!this.Disposing && (m_pHView != null))
                m_pHView.ResetKeyState();
        }
    }


}
