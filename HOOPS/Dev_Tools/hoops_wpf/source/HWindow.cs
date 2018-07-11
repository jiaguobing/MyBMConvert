using System;
using System.Collections.Generic;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

using System.Windows.Interop;
using System.Windows.Forms.Integration;


/*! \namespace hoops_base Contains the HForm class */

namespace hoops_base
{
    //! HForm hooks up MouseEvents of the Window and passes them to the HPanel class
    /*!
     * This is the base class from which a custom HForm class should be derived
     */
    public partial class HWindow : Window
    {
        /*!
         * A reference to the HPanel object.  A custom HPanel should be created in an overloaded HPanel::Init method 
         */
        public HPanel m_pHPanel;

        /*! Reference to HOOPS database */
        public HDB m_pHDB;

        /*! Reference to the WindowsFormsHost to host the UserControl in WPF application */
        public WindowsFormsHost m_pWinFormsHost;

        /*!
         * Default Constructor
         */
        public HWindow()
            : base()
        {
            //this.LostFocus += new RoutedEventHandler(HForm_LostFocus);
        }

#if false

        /*!
         *  Handles the LostFocus event by dispatching to the HPanel object
         */
        void HForm_LostFocus(object sender, RoutedEventArgs e)
        {
            if (m_pHPanel != null && m_pHPanel.m_pHView != null)
                m_pHPanel.LostFocus();
        }


        /*! 
         * Maps the MouseLeftButtonDown event to HPanel::MouseLeftButtonDown
         *
         * \param e Provides data for System.Windows.Forms.Control.MouseLeftButtonDown event
         */
        protected void Window_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            if (m_pHPanel != null && m_pHPanel.m_pHView != null)
            {
                Point position = e.GetPosition(this);
                int pX = (int)position.X;
                int pY = (int)position.Y;

                HEventInfo hevent = new HEventInfo(m_pHPanel.m_pHView);
                uint flags = MapFlags_Mouse(e);
                hevent.SetPoint(HEventType.HE_LButtonDown, pX, pY, flags);
                m_pHPanel.m_pHView.InjectEvent(hevent);
            }

        }

        /*! 
        * Maps the MouseLeftButtonUp event to HPanel::MouseLeftButtonUp
        *
        * \param e Provides data for System.Windows.Forms.Control.MouseLeftButtonUp event
        */
        protected void Window_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            if (m_pHPanel != null && m_pHPanel.m_pHView != null)
            {
                Point position = e.GetPosition(this);
                int pX = (int)position.X;
                int pY = (int)position.Y;

                HEventInfo hevent = new HEventInfo(m_pHPanel.m_pHView);
                uint flags = MapFlags_Mouse(e);
                hevent.SetPoint(HEventType.HE_LButtonUp, pX, pY, flags);
                m_pHPanel.m_pHView.InjectEvent(hevent);
            }
        }

        /*! 
        * Maps the MouseRightButtonDown event to HPanel::MouseRightButtonDown
        *
        * \param e Provides data for System.Windows.Forms.Control.MouseRightButtonDown event
        */
        protected void Window_MouseRightButtonDown(object sender, MouseButtonEventArgs e)
        {
            if (m_pHPanel != null && m_pHPanel.m_pHView != null)
            {
                Point position = e.GetPosition(this);
                int pX = (int)position.X;
                int pY = (int)position.Y;

                HEventInfo hevent = new HEventInfo(m_pHPanel.m_pHView);
                uint flags = MapFlags_Mouse(e);
                hevent.SetPoint(HEventType.HE_LButtonDown, pX, pY, flags);
                m_pHPanel.m_pHView.InjectEvent(hevent);
            }
        }

        /*! 
        * Maps the MouseMove event to HPanel::MouseMove
        *
        * \param e Provides data for System.Windows.Forms.Control.MouseMove event
        */
        protected void Window_MouseMove(object sender, MouseEventArgs e)
        {
            if (m_pHPanel != null && m_pHPanel.m_pHView != null)
            {
                Point position = e.GetPosition(this);
                int pX = (int)position.X;
                int pY = (int)position.Y;

                HEventInfo hevent = new HEventInfo(m_pHPanel.m_pHView);

                uint flags = MapFlags_Mouse(e);
                hevent.SetPoint(HEventType.HE_MouseMove, pX, pY, flags);

                m_pHPanel.m_pHView.InjectEvent(hevent);
            }
        }

        /*! 
        * Maps the MouseWheel event to HPanel::MouseWheel
        *
        * \param e Provides data for System.Windows.Forms.Control.MouseWheel event
        */
        protected void Window_MouseWheel(object sender, MouseWheelEventArgs e)
        {
            if (m_pHPanel != null && m_pHPanel.m_pHView != null)
            {
                Point position = e.GetPosition(this);
                int pX = (int)position.X;
                int pY = (int)position.Y;

                HEventInfo hevent = new HEventInfo(m_pHPanel.m_pHView);

                hevent.SetMouseWheelDelta(e.Delta);
                uint flags = MapFlags_Mouse(e);
                hevent.SetPoint(HEventType.HE_MouseWheel, pX, pY, flags);

                m_pHPanel.m_pHView.InjectEvent(hevent);
            }
        }

        /*! 
         * Maps the GUI-specific state flags to HOOPS/MVO abstracted flags
         *
         * \param e Provides data for the System.Windows.Forms.Control.MouseUp, MouseDown, MouseMove event 
         */
        protected uint MapFlags_Mouse(MouseEventArgs e)
        {
            uint flags = new uint();

            if (e.LeftButton == MouseButtonState.Pressed) flags |= (uint)HCSMVO.MVO_LBUTTON;
            if (e.RightButton == MouseButtonState.Pressed) flags |= (uint)HCSMVO.MVO_RBUTTON;
            if (e.MiddleButton == MouseButtonState.Pressed) flags |= (uint)HCSMVO.MVO_MBUTTON;

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
            if (Keyboard.Modifiers == ModifierKeys.Shift) flags |= (1 << 2);
            if (Keyboard.Modifiers == ModifierKeys.Control) flags |= (1 << 1);
            if (Keyboard.Modifiers == ModifierKeys.Alt) flags |= (1 << 3);

            return flags;
        }

        ///////////////////////////////////////////
#endif

    }
}
