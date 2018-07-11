package com.techsoft.hoops;

import org.eclipse.swt.SWT;
import org.eclipse.swt.events.KeyEvent;
import org.eclipse.swt.events.KeyListener;
import org.eclipse.swt.events.MouseEvent;
import org.eclipse.swt.events.MouseListener;
import org.eclipse.swt.events.MouseMoveListener;
import org.eclipse.swt.events.MouseWheelListener;
import org.eclipse.swt.events.PaintEvent;
import org.eclipse.swt.events.PaintListener;
import org.eclipse.swt.widgets.Canvas;
import org.eclipse.swt.widgets.Composite;

public class HoopsSwtCanvas extends Canvas {

	private final class MouseWheelListenerImplementation implements MouseWheelListener {
		public void mouseScrolled(final MouseEvent e) {

		}
	}

	private final class KeyListenerImplementation implements KeyListener {
		public void keyPressed(final KeyEvent e) {
			final HEventInfo hevent = new HEventInfo(m_HView);
			final int flags = mapKeyFlags(e);

			hevent.SetKey(HEventType.HE_KeyDown, e.character, 1, flags);
			m_HView.InjectEvent(hevent);
		}

		public void keyReleased(final KeyEvent e) {
			final HEventInfo hevent = new HEventInfo(m_HView);
			final int flags = mapKeyFlags(e);

			hevent.SetKey(HEventType.HE_KeyUp, e.character, 1, flags);
			m_HView.InjectEvent(hevent);
		}
	}

	private final class MouseListenerImplementation implements MouseListener {
		public void mouseDoubleClick(final MouseEvent e) {
			final HEventInfo hevent = new HEventInfo(m_HView);
			final int flags = mapFlags(e);
			HEventType eventType = null;
			switch (e.button) {
			case 1:
				eventType = HEventType.HE_LButtonDblClk;
				break;
			case 2:
				eventType = HEventType.HE_MButtonDblClk;
				break;
			case 3:
				eventType = HEventType.HE_RButtonDblClk;
				break;
			default:
				break;
			}
			hevent.SetPoint(eventType, e.x, e.y, flags);
			m_HView.InjectEvent(hevent);
		}

		public void mouseDown(final MouseEvent e) {
			final HEventInfo hevent = new HEventInfo(m_HView);
			final int flags = mapFlags(e);
			HEventType eventType = null;
			switch (e.button) {
			case 1:
				eventType = HEventType.HE_LButtonDown;
				break;
			case 2:
				eventType = HEventType.HE_MButtonDown;
				break;
			case 3:
				eventType = HEventType.HE_RButtonDown;
				break;
			default:
				break;
			}
			hevent.SetPoint(eventType, e.x, e.y, flags);
			m_HView.InjectEvent(hevent);
		}

		public void mouseUp(final MouseEvent e) {
			final HEventInfo hevent = new HEventInfo(m_HView);
			final int flags = mapFlags(e);
			HEventType eventType = null;
			switch (e.button) {
			case 1:
				eventType = HEventType.HE_LButtonUp;
				break;
			case 2:
				eventType = HEventType.HE_MButtonUp;
				break;
			case 3:
				eventType = HEventType.HE_RButtonUp;
				break;
			default:
				break;
			}
			hevent.SetPoint(eventType, e.x, e.y, flags);
			m_HView.InjectEvent(hevent);
		}
	}

	private final class MouseMoveListenerImplementation implements MouseMoveListener {
		public void mouseMove(final MouseEvent e) {
			final HEventInfo hevent = new HEventInfo(m_HView);
			final int flags = mapFlags(e);
			hevent.SetPoint(HEventType.HE_MouseMove, e.x, e.y, flags);
			m_HView.InjectEvent(hevent);
		}
	}

	private final class PaintListenerImplementation implements PaintListener 
	{
		public void paintControl(final PaintEvent e) 
		{

			if (m_HView != null)
			{
				HJ.Control_Update_By_Key(m_HView.GetViewKey(), "redraw everything");
				m_HView.Update();
			}

		}
	}

	protected final HBaseModel m_HModel;

	/**
	 * A reference to the HBaseView object associated with the canvas
	 */
	protected HBaseView m_HView;

	protected HSelectionSet m_HSelectionSet;

	protected Object m_HOperator;



	/**
	 * Constructor which sets the value of initDone variable to false
	 */
	public HoopsSwtCanvas(final Composite parent, final int style) {
		super(parent, style);
		addPaintListener(new PaintListenerImplementation());
		addMouseListener(new MouseListenerImplementation());
		addMouseMoveListener(new MouseMoveListenerImplementation());
		addMouseWheelListener(new MouseWheelListenerImplementation());
		addKeyListener(new KeyListenerImplementation());
		m_HModel = createDefaultModel();
		m_HView = createView(m_HModel);
		init();
	}

	/**
	 * Factory method for the default HOOPS/MVO model. The model has to be
	 * initialized.
	 *
	 * @return A newly created and initialized model.
	 */
	protected HBaseModel createDefaultModel() {
		final HBaseModel model = new HBaseModel();
		model.Init();
		return model;
	}

	/**
	 * Factory method for the HOOPS/MVO m_HView on the given model. The m_HView has to
	 * be initialized.
	 *
	 * @return A newly created and initialized m_HView.
	 */
	protected HBaseView createView(final HBaseModel model) {
		final int swig = (int)handle;
		final HBaseView m_HView = new HBaseView(model, "", "opengl", "", swig);
		m_HView.Init();

		m_HSelectionSet = new HSelectionSet(m_HView);
		m_HSelectionSet.Init();
		m_HView.SetSelection(m_HSelectionSet);


		//Set the default operator
		m_HOperator = new HOpCameraManipulate(m_HView);
		m_HView.SetCurrentOperator((HBaseOperator)m_HOperator);

		//Setup the m_HView
		m_HView.CameraPositionChanged(true);
		m_HView.RenderGouraud();
		m_HView.SetHandedness(HandednessMode.HandednessRight);

		// set the gradiant window background
		HPoint window_top_color = new HPoint(0.0000f, 0.501961f, 0.501961f);
		HPoint window_bottom_color = new HPoint(1.000f, 0.984314f, 0.941176f);
		m_HView.SetWindowColor(window_top_color, window_bottom_color);

		HJ.Open_Segment_By_Key(m_HView.GetSceneKey());
		HJ.Set_Color_By_Index("faces", 2);
		HJ.Set_Color_By_Index("text, lights", 1);
		HJ.Set_Color_By_Index("edges, lines", 1);
		HJ.Set_Color_By_Index("markers", 1);
		HJ.Set_Rendering_Options("color interpolation, color index interpolation");
		HJ.Set_Visibility("lights = (faces = on, edges = off, markers = off), markers = off, faces=on, edges=off, lines=off, text = on");
		HJ.Set_Selectability("everything = off, geometry = on");
		HJ.Close_Segment();

		// for quickmoves
		HJ.Open_Segment_By_Key(m_HView.GetWindowspaceKey());
		HJ.Set_Color_By_Index("geometry", 3);
		HJ.Set_Color_By_Index("window contrast", 1);
		HJ.Set_Color_By_Index("windows", 1);

		HJ.Set_Visibility("markers=on");
		HJ.Set_Color("markers = green, lines = green");
		HJ.Set_Marker_Symbol("+");
		HJ.Set_Selectability("off");
		HJ.Close_Segment();

		HJ.Open_Segment_By_Key(m_HView.GetSceneKey());
		HJ.Set_Text_Font("transforms = off");
		HJ.Close_Segment();

		// first set up the window, then set the axis triad options
		m_HView.Update();
		m_HView.SetAxisMode(AxisMode.AxisOn);
		m_HView.AdjustAxisWindow();
		return m_HView;
	}

	@Override
	public void dispose() {
		m_HView.delete();
		m_HView = null;
		super.dispose();
	}

	public HBaseModel getModel() {
		if (m_HView != null)
			return m_HView.GetModel();
		else
			return m_HModel;
	}

	/**
	 * @return The HBaseView object associated with the canvas. The object is
	 *         null until first paint.
	 */
	public HBaseView getView() {
		return m_HView;
	}

	/**
	 * Initializes the HBaseView object and the m_HView key. When overridden, the
	 * inherited method must be called.
	 *
	 * Should not be called directly.
	 */
	public void init() {

		HJ.Open_Segment_By_Key(m_HView.GetViewKey());
		{
			HJ.Set_Driver_Options("debug = 0x00000040");
		}
		HJ.Close_Segment();

	}

	/**
	 *
	 * Maps the GUI-specific mouse/key state flags to HOOPS/MVO abstracted flags
	 *
	 * @param e
	 *            Provides data for the org.eclipse.swt.events.MouseEvent event
	 * @return variable indicating which MouseButton and Modifier Key is pressed
	 */
	public int mapFlags(final MouseEvent e) {
		int flags = 0;

		// mouse state
		final int state = e.stateMask;

		if (SWT.BUTTON1 == (state & SWT.BUTTON1))
			flags |= HJMVOConstants.MVO_LBUTTON;

		if (SWT.BUTTON2 == (state & SWT.BUTTON2))
			flags |= HJMVOConstants.MVO_MBUTTON;

		if (SWT.BUTTON3 == (state & SWT.BUTTON3))
			flags |= HJMVOConstants.MVO_RBUTTON;

		if (SWT.ALT == (state & SWT.ALT))
			flags |= HJMVOConstants.MVO_ALT;

		if (SWT.CTRL == (state & SWT.CTRL))
			flags |= HJMVOConstants.MVO_CONTROL;

		if (SWT.SHIFT == (state & SWT.SHIFT))
			flags |= HJMVOConstants.MVO_SHIFT;

		return flags;
	}

	/**
	 *
	 * Maps the GUI-specific key state flags to HOOPS/MVO abstracted flags
	 *
	 * @param e
	 *            Provides data for the org.eclipse.swt.events.KeyEvent event
	 * @return variable indicating which modifier key is pressed
	 */
	public int mapKeyFlags(final KeyEvent e) {
		int flags = 0;
		final int state = e.stateMask;

		if (SWT.ALT == (state & SWT.ALT))
			flags |= HJMVOConstants.MVO_ALT;

		if (SWT.CTRL == (state & SWT.CTRL))
			flags |= HJMVOConstants.MVO_CONTROL;

		if (SWT.SHIFT == (state & SWT.SHIFT))
			flags |= HJMVOConstants.MVO_SHIFT;

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
		m_HView.SetCurrentOperator((HBaseOperator)m_HOperator);
	}

}
