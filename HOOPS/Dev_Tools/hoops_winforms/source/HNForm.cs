using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

/*! \namespace hoops_panel Contains the HNForm class */
namespace hoops_panel
{
    //! HNForm hooks up KeyBoard and Mousewheel events of the Form and passes them to the HNPanel class
    /*!
     * The HNForm member functions overload the Form OnKeyPress, OnKeyUp and MouseWheel 
     * methods and then passes control to the HNPanel OnKeyPress, OnKeyUp and OnMouseWheel methods for custom
     * handling.
     *
     * This is the base class from which a custom HNForm class should be derived
     */
    public class HNForm : Form
    {
        /*!
         * A reference to the HNPanel object.  A custom HNPanel should be created in an overloaded HNPanel::Init method 
         * (i.e.  MyCustomForm::Init)
         */
        public HNPanel m_pHNPanel;   
      
        /*!
         * Default Constructor
         */
        public HNForm()
        {
            this.LostFocus += new EventHandler(HNForm_LostFocus);
        }

        /*!
         *  Handles the LostFocus event by dispatching to the HNPanel object
         */
        void HNForm_LostFocus(object sender, EventArgs e)
        {
            if (!this.Disposing && (m_pHNPanel != null))
                m_pHNPanel.LostFocus();
        }

        /*! 
         * Maps the OnKeyDown event to HNPanel::OnKeyDown
         *
         * \param e Provides data for System.Windows.Forms.Control.KeyDown event
         */
        protected override void OnKeyDown(KeyEventArgs e)
		{
			if (m_pHNPanel != null)
				m_pHNPanel.OnKeyDown(e);
		}


        /*! 
         * Maps the OnKeyUp event to HNPanel::OnKeyUp
         *
         * \param e Provides data for System.Windows.Forms.Control.KeyUp event
         */
        protected override void OnKeyUp(KeyEventArgs e)
        {
            if (m_pHNPanel != null)
                m_pHNPanel.OnKeyUp(e);
        }


        /*! 
         * Maps the OnKeyPress event to HNPanel::OnKeyPress
         *
         * \param e Provides data for System.Windows.Forms.Control.KeyPress event
         */
        protected override void OnKeyPress(KeyPressEventArgs e)
        {
            if (m_pHNPanel != null)
                m_pHNPanel.OnKeyPress(e);
        }


        /*! 
         * Maps the OnMouseWheel event to HNPanel::OnMouseWheel
         *
         * \param e Provides data for System.Windows.Forms.Control.MouseWheel event
         */
        protected override void OnMouseWheel(MouseEventArgs e)
        {
            if (m_pHNPanel != null)
                m_pHNPanel.OnMouseWheel(e);
        }

		private void InitializeComponent()
		{
			this.SuspendLayout();
			// 
			// HNForm
			// 
			this.ClientSize = new System.Drawing.Size(292, 260);
			this.Name = "HNForm";
			this.ResumeLayout(false);

		}

  
    }

}


