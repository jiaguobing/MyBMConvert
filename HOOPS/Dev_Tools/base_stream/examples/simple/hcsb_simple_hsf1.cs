using System;
using System.IO;

#if _M_X64
using HLONG = System.Int64;
#else
using HLONG = System.Int32;
#endif

public class hcsb_simple_hsf1
{
    static void Main() 
    {
		// char[] mycharbuffer = new char [4096];
        byte[] mybuffer = new byte[4096];
		int size=4096;
		const string filename = "testfile1.hsf";

		// create a new file toolkit object
		BStreamFileToolkit mytoolkit = new BStreamFileToolkit();

		// open a binary file for output 
		// FileStream fs = new FileStream(filename, FileMode.OpenOrCreate, FileAccess.Write);			
		BinaryWriter binWriter = new BinaryWriter( File.Open(filename, FileMode.OpenOrCreate) );			

		// tell the toolkit where to begin writing data; the size of the 'utility' buffer
		// can be anything; for this example, we use a 4k buffer
		mytoolkit.PrepareBuffer(mybuffer, size);

		// create a TK_Header object and export it's associated HSF objects,
		// which include the TKE_Comment and TKE_File_Info objects
		// TK_Header automatically formats the TKE_Comment to contain file version 
		// information.  
		BBaseOpcodeHandler header_handler = new TK_Header();
		WriteObject(header_handler, mytoolkit, binWriter, mybuffer, size);
		header_handler.Dispose();

		// export a TKE_Start_Compression object
		BBaseOpcodeHandler comp_handler = new TK_Compression( (char)(TKE_Object_Types.TKE_Start_Compression) );
		WriteObject(comp_handler, mytoolkit, binWriter, mybuffer, size);
		comp_handler.Dispose();

		// export a TKE_Open_Segment object (the TK_Open_Segment opcode handler
		// automatically supports this opcode, so it does not need to be passed
		// into the constructor
		TK_Open_Segment seghandler = new TK_Open_Segment();
		seghandler.SetSegment("point");
		WriteObject(seghandler, mytoolkit, binWriter, mybuffer, size);
		seghandler.Dispose();

		// export a TKE_Marker object
        TK_Point pointhandler = new TK_Point( (byte)(TKE_Object_Types.TKE_Marker) );
		pointhandler.SetPoint(0.0f, 0.0f, 0.0f);
		WriteObject(pointhandler, mytoolkit, binWriter, mybuffer, size);
		pointhandler.Dispose();

		TK_Visibility vishandler = new TK_Visibility();
        vishandler.SetGeometry( (int)(TKO_Geometry_Bits.TKO_Geo_Marker | TKO_Geometry_Bits.TKO_Geo_Line) );
        vishandler.SetValue( (int) (TKO_Geometry_Bits.TKO_Geo_Marker | TKO_Geometry_Bits.TKO_Geo_Line) );
		WriteObject(vishandler, mytoolkit, binWriter, mybuffer, size);
		vishandler.Dispose();

		TK_Color_RGB colorhandler = new TK_Color_RGB();
        colorhandler.SetGeometry( (int) (TKO_Geometry_Bits.TKO_Geo_Marker | TKO_Geometry_Bits.TKO_Geo_Line) );
		colorhandler.SetRGB(0.0f,0.0f,1.0f);
		WriteObject(colorhandler, mytoolkit, binWriter, mybuffer, size);
		colorhandler.Dispose();

		// export a TKE_Close_Segment object (the TK_Close_Segment opcode handler
		// automatically supports this opcode, so it does not need to be passed
		// into the constructor
		BBaseOpcodeHandler close_handler = new TK_Close_Segment();
		WriteObject(close_handler, mytoolkit, binWriter, mybuffer, size);
		close_handler.Dispose();

		// export a TKE_Stop_Compression object
		BBaseOpcodeHandler stopcomp_handler = new TK_Compression( (char) (TKE_Object_Types.TKE_Stop_Compression) );
		WriteObject(stopcomp_handler, mytoolkit, binWriter, mybuffer, size);
		stopcomp_handler.Dispose();

		// export a TKE_Termination object
		BBaseOpcodeHandler term_handler = new TK_Terminator( (char) (TKE_Object_Types.TKE_Termination) );
		WriteObject(term_handler, mytoolkit, binWriter, mybuffer, size);
		term_handler.Dispose();

		// ask the toolkit how much of the final buffer is filled
		int count = mytoolkit.CurrentBufferLength();

		// write out the final buffer
		// fwrite(mybuffer, sizeof (char), count, flptr);
        binWriter.Write(mybuffer);

		// close the file
		binWriter.Close();

		// clean up
		//binWriter.Dispose();
		mytoolkit.Dispose();

        return;
	}

	static void WriteObject
	(
		BBaseOpcodeHandler handler,
		BStreamFileToolkit tk,
		BinaryWriter wb,
		byte[] buffer,
		int size
	)
	{
		// ask the toolkit to write out the object; the toolkit will write out as much as
		// the current buffer can hold
		TK_Status status = handler.Write(tk);

		// if there is still more data left to write; we have to loop through 
		// generating and writing buffers of data until we're done
		while (status == TK_Status.TK_Pending) 
		{
			// due to compression, preparing a new buffer might immediately fill that buffer
			// so we need an extra check to possibly export that buffer and prepare a new buffer
			while (status == TK_Status.TK_Pending) 
			{
				// ask the toolkit how much of the buffer is currently filled
				// this will normally be the same as the buffer size that we passed in
				int count = tk.CurrentBufferLength();

				// write the current out to our file since this example deals with 
				// exporting a file; however, instead of writing to a file, the buffer of
				// data could be sent to an application data-structure, or transmitted
				// over a network ('streamed'), etc...
				// fwrite(buffer, sizeof (char), count, fp);
				wb.Write(buffer);

				// tell the toolkit where it can resume writing data; in this case, we
				// can reuse the same buffer once we're done with it
				status = tk.PrepareBuffer(buffer, size);
			}

			// ask the toolkit to continue trying to write out the object's data
			status = handler.Write(tk);
		}
	}



}


