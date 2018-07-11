using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

#if _M_X64
using HLONG = System.Int64;
#else
using HLONG = System.Int32;
#endif

/*! \namespace hoops_panel_3dgs_only Contains the HPanel class */
namespace hoops_panel_3dgs_only
{
    //! The HPanel class provides a HOOPS/3dgs-only implementation of the Winforms Panel class.
    /*!
     * HPanel creates and manages the connection of a HOOPS/3dGS driver instance to a Panel object, which includes
	 * redrawing the scene during exposes/resizes. 
     * 
     * This class should only be directly derived from if you specifically do NOT want to use HOOPS/MVO
    */
    public class HPanel : Panel
    {
        public HPanel()
            : base()
        {
            IntPtr winid = this.Handle;
            //Do some HOOPS prep stuff

            HCS.Delete_Segment("?picture");
            HCS.UnDefine_Alias("?picture");
            HCS.Define_Alias("?picture", "?driver/dx9/window0");

            HCS.Open_Segment("?picture");
            HCS.Set_Driver_Options("use window ID = " + winid);
            HCS.Close_Segment();
        }

        /*! Instructs HOOPS/3dGS to redraw the scene */
        override protected void OnResize(System.EventArgs e)
        {
            base.OnResize(e);
            HCS.Update_Display();
        }

        /*! Instructs HOOPS/3dGS to redraw the scene */
        override protected void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);            
            HLONG key = HCS.Open_Segment("?picture");
            HCS.Control_Update_By_Key(key, "redraw everything");
            HCS.Update_Display();
            HCS.Close_Segment();
        }
    }
}
