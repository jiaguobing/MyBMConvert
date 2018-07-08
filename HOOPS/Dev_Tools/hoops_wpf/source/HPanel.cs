using System;
using System.Collections.Generic;
using System.Text;

using System.Windows.Forms;
using System.Windows.Controls;
using System.Windows;
using System.Windows.Input;
using System.Runtime.InteropServices;
using System.Diagnostics;

using System.Windows.Interop;

#if _M_X64
using HCS_KEY = System.Int64;
#else
using HCS_KEY = System.Int32;
#endif

/*! \namespace hoops_base Contains the HPanel class */
namespace hoops_base
{
    //! The HPanel class provides a HOOPS/MVO-specific implementation of the UserControl class from Winforms.
    /*!
     * HPanel creates and manages the connection of a HOOPS/3dGS driver instance to a Panel object, which includes
	 * redrawing the scene during exposes/resizes. It also encapsulates passing of mouseevents through 
     * to corresponding MVO operator methods.  
     * 
     * This is the base class from which a custom HPanel class should be derived, and should be used when the application
     * is using both HOOPS/MVO and HOOPS/3dGS
    */
    public partial class HPanel : System.Windows.Forms.UserControl
    {
        //Public Data Members
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
        protected object m_pHOperator;

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
        public HPanel() : base()
        {
            this.Paint += PanelPaint;
            this.Resize += PanelResize;

            this.MouseDown += PanelMouseDown;
            this.MouseDoubleClick += PanelMouseDoubleClick;
            this.MouseUp += PanelMouseUp;
            this.MouseMove += PanelMouseMove;
            this.MouseWheel += PanelMouseWheel;

            this.KeyDown += PanelKeyDown;
            this.KeyUp += PanelKeyUp;
            this.KeyPress += PanelKeyPress;     

            m_keyPairs = new int[255];
        }

        

        /*! 
         * The default implementation initializes the HBaseView object and the view key.
         */
        public void Init()
        {
            if (m_pHView == null)
                return;
            HCS.Open_Segment_By_Key(m_pHView.GetViewKey());
            HCS.Set_Driver_Options("debug = 0x00000040");
            HCS.Close_Segment();
        }

        /*!
        We do not want to paint a background, as HOOPS will do all the drawing
        
        \param e Provides data for paint event
        */
        protected override void OnPaintBackground(PaintEventArgs e)
        {
            // we do not want to paint a background, as HOOPS will do all the drawing
        }

        /*!
          * Instructs HOOPS to redraw the scene
          *
          * \param sender A reference to an object which is the root of the type hierarchy 
          * \param e Provides data for the System.Windows.Forms.UserControl.paint event 
          */
        protected void PanelPaint(object sender, PaintEventArgs e)
        {

            if (!this.Disposing && (m_pHView != null))
            {
                HCS.Control_Update_By_Key(m_pHView.GetViewKey(), "redraw everything");
                m_pHView.Update();
            }
        }


        /*! 
         * Ensures that view in the Panel is redraw whenever it is resized
         *
         * \param sender A reference to an object  which is the root of the type hierarchy 
         * \param e Provides data for the event 
         */
        private void PanelResize(object sender, EventArgs e)
        {
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
         * \param e Provides data for the System.Windows.Forms.UserControl.MouseDown event 
         */
        protected void PanelMouseDown(object sender, System.Windows.Forms.MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
                OnLeftButtonDown(e);
            else if (e.Button == MouseButtons.Right)
                OnRightButtonDown(e);
            else if (e.Button == MouseButtons.Middle)
                OnMidButtonDown(e);
        }

        /*!
        * Dispatches the OnLeftButtonDown event to HOOPS/MVO, which will in turn dispatch it to the current HBaseOperator object
        * The state flags are mapped to HOOPS/MVO abstracted flags
        *
        * \param e Provides data for the System.Windows.Forms.UserControl.MouseDown event 
        */
        protected void OnLeftButtonDown(System.Windows.Forms.MouseEventArgs e)
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
         * \param e Provides data for the System.Windows.Forms.UserControl.MouseUp event 
         */
        protected void OnLeftButtonUp(System.Windows.Forms.MouseEventArgs e)
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
         * \param e Provides data for the System.Windows.Forms.UserControl.MouseDoubleClick event 
         */
        protected void OnLeftButtonDoubleClick(System.Windows.Forms.MouseEventArgs e)
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
         * \param e Provides data for the System.Windows.Forms.UserControl.MouseDown event 
        */
        protected void OnRightButtonDown(System.Windows.Forms.MouseEventArgs e)
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
         * \param e Provides data for the System.Windows.Forms.UserControl.MouseUp event 
         */
        protected void OnRightButtonUp(System.Windows.Forms.MouseEventArgs e)
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
         * \param e Provides data for the System.Windows.Forms.UserControl.MouseDoubleClick event 
         */
        protected void OnRightButtonDoubleClick(System.Windows.Forms.MouseEventArgs e)
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
         * \param e Provides data for the System.Windows.Forms.UserControl.MouseDown event 
         */
        protected void OnMidButtonDown(System.Windows.Forms.MouseEventArgs e)
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
        * \param e Provides data for the System.Windows.Forms.UserControl.MouseUp event 
        */
        protected void OnMidButtonUp(System.Windows.Forms.MouseEventArgs e)
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
         * \param e Provides data for the System.Windows.Forms.UserControl.MouseDoubleClick event 
         */
        protected void OnMidButtonDoubleClick(System.Windows.Forms.MouseEventArgs e)
        {
            HEventInfo hevent = new HEventInfo(m_pHView);
            uint flags = MapFlags_Mouse(e);
            hevent.SetPoint(HEventType.HE_MButtonDblClk, e.X, e.Y, flags);
            m_pHView.InjectEvent(hevent);
        }

        /*! 
         * Passes control to the method depending on which Mouse Button is double clicked
         *
         * \param sender A reference to an object which is the root of the type hierarchy 
         * \param e Provides data for the System.Windows.Forms.UserControl.MouseDoubleClick event 
         */
        protected void PanelMouseDoubleClick(object sender, System.Windows.Forms.MouseEventArgs e)
        {
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
         * \param e Provides data for the System.Windows.Forms.userControl.MouseUp event 
         */
        protected void PanelMouseUp(object sender, System.Windows.Forms.MouseEventArgs e)
        {
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
         * \param e Provides data for the System.Windows.Forms.UserControl.MouseMove event 
         */
        protected void PanelMouseMove(object sender, System.Windows.Forms.MouseEventArgs e)
        {
            HEventInfo hevent = new HEventInfo(m_pHView);

            uint flags = MapFlags_Mouse(e);
            hevent.SetPoint(HEventType.HE_MouseMove, e.X, e.Y, flags);

            m_pHView.InjectEvent(hevent);
        }

        /*!
         * This method is called from HForm OnMouseWheel method
         * 
         * It dispatches the OnMouseWheel event to HOOPS/MVO, which will in turn dispatch it to the current HBaseOperator object
         * The state flags are mapped to HOOPS/MVO abstracted flags
         *
         * \param e Provides data for System.Windows.Forms.UserControl.MouseWheel
         */
        protected void PanelMouseWheel(object sender, System.Windows.Forms.MouseEventArgs e)
        {
            HEventInfo hevent = new HEventInfo(m_pHView);

            hevent.SetMouseWheelDelta(e.Delta);
            uint flags = MapFlags_Mouse(e);
            hevent.SetPoint(HEventType.HE_MouseWheel, e.X, e.Y, flags);

            m_pHView.InjectEvent(hevent);
        }

        protected void PanelKeyDown(object sender, System.Windows.Forms.KeyEventArgs e)
        {
            OnKeyDown(e);
        }

        protected void PanelKeyUp(object sender, System.Windows.Forms.KeyEventArgs e)
        {
            OnKeyUp(e);
        }

        protected void PanelKeyPress(object sender, System.Windows.Forms.KeyPressEventArgs e)
        {
            OnKeyPress(e);
        }

        /*! 
         * Maps the GUI-specific state flags to HOOPS/MVO abstracted flags
         *
         * \param e Provides data for the System.Windows.Forms.userControl.MouseUp, MouseDown, MouseMove event 
         */
        protected uint MapFlags_Mouse(System.Windows.Forms.MouseEventArgs e)
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
         * \param flags Provides information about which mouse button is pressed or released 
         * \return uint value which contains the information about which mouse button is pressed or released              
         *              and which modifier key is pressed
         */
        protected uint MapModifierKeys(uint flags)
        {
            if ((System.Windows.Forms.Control.ModifierKeys & Keys.Shift) != 0)
            {
                flags |= (1 << 2);
            }
            if ((System.Windows.Forms.Control.ModifierKeys & Keys.Control) != 0)
            {
                flags |= (1 << 1);
            }
            if ((System.Windows.Forms.Control.ModifierKeys & Keys.Alt) != 0)
            {
                flags |= (1 << 3);
            }
            return flags;
        }


        /*! 
         * Deletes the current operator associated with the HBaseView object (if any) and resets the HBaseView's
	     * current operator to the new one
         * \param new_operator A reference to an HBaseOperator object to be used as the new operator
         */
        public void SetCurrentOperator(HBaseOperator new_operator)
        {
            HBaseOperator current_operator = m_pHView.GetCurrentOperator();
            if (current_operator != null)
                current_operator.Dispose();

            if (m_pHOperator != null)
                ((HBaseOperator)m_pHOperator).Dispose();

            m_pHOperator = new_operator;
            m_pHView.SetCurrentOperator((HBaseOperator)m_pHOperator);

        }


        /*!
         * Disposes HOOPS/MVO components and then Disposes panel
         */
        public virtual void DisposePanel()
        {
            ((HBaseOperator)m_pHOperator).Dispose();
            m_pHSelection.Dispose();
            m_pHView.SetSelection((HSelectionSet)null);
            m_pHView.Dispose();
            m_pHModel.Dispose();
            this.Dispose();
        }

        /*!
         * It dispatches the OnKeyUp event to HOOPS/MVO, which will in turn dispatch it to the current HBaseOperator object
         * The state flags are mapped to HOOPS/MVO abstracted flags
         *
         * \param e Provides data for System.Windows.Forms.UserControl.KeyUp event
         */
        public new void OnKeyUp(System.Windows.Forms.KeyEventArgs e)
        {
            if (m_pHView != null)
            {
                if (e.KeyValue < 0 || e.KeyValue >= m_keyPairs.Length)
                {
                    // do nothing because array out of bounds will occur.
                }
                else
                {
                    try
                    {
                        HEventInfo hevent = new HEventInfo(m_pHView);
                        uint flags = MapModifierKeys(0);
                        hevent.SetKey(HEventType.HE_KeyUp, (uint)m_keyPairs[e.KeyValue], 1, flags);
                        m_pHView.InjectEvent(hevent);
                    }
                    catch
                    {
                        Trace.WriteLine("HPanel.OnKeyUp exception:key=" + e.KeyValue.ToString());
                    }
                }
            }
        }

        /*!
         * It dispatches the OnKeyDown event to HOOPS/MVO, which will in turn dispatch it to the current HBaseOperator object
         * The state flags are mapped to HOOPS/MVO abstracted flags
         *
         * \param e Provides data for System.Windows.Forms.UserControl.KeyDown event
         */
        public new void OnKeyDown(System.Windows.Forms.KeyEventArgs e)
        {
            if (m_pHView != null)
                m_lastKeyPressed = e.KeyValue;
        }


        /*!
         * It dispatches the OnKeyPress event to HOOPS/MVO, which will in turn dispatch it to the current HBaseOperator object
         * The state flags are mapped to HOOPS/MVO abstracted flags
         *
         * \param e Provides data for System.Windows.Forms.UserControl.KeyPress event
         */
        public new void OnKeyPress(KeyPressEventArgs e)
        {
            if (m_pHView != null)
            {
                if (m_lastKeyPressed < 0 || m_lastKeyPressed >= m_keyPairs.Length)
                {
                    // do nothing because array out of bounds will occur.           
                }
                else
                {
                    try
                    {
                        m_keyPairs[m_lastKeyPressed] = e.KeyChar;

                        HEventInfo hevent = new HEventInfo(m_pHView);
                        uint flags = MapModifierKeys(0);
                        hevent.SetKey(HEventType.HE_KeyDown, (uint)e.KeyChar, 1, flags);
                        m_pHView.InjectEvent(hevent);
                    }
                    catch
                    {
                        Trace.WriteLine("HPanel.OnKeyPress exception:key=" + m_lastKeyPressed.ToString());
                    }
                }
            }
        }

        /*! 
         *  Called when focus leaves the Panel. Resets the HBaseView object's key state.
         */
        public new void LostFocus()
        {
            if (!this.Disposing && (m_pHView != null))
                m_pHView.ResetKeyState();
        }

    }//End of HPanel Class
}
