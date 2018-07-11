// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// This header file is a completely self-contained class that defines all of the
// member functions that it needs inside the declaration.  It is just a utility class
// to stuff unaligned data values into bytes.
//

#include <assert.h>
#define ASSERT(x) assert(x)

//! Handles transfer of certain unaligned bit lengths via unsigned char arrays
/*! 
    We determined that visual artifacts introduced in sending 8-bit
    normals were too noticeable, and that higher accuracy was required.
    At the same time, however, we wanted better compression than 16-bit.
    Thus, the HByteStream class was born.

    For efficiency, this class makes certain assumptions about the
    number of bits per pixel: 1) that samples are at least 1 byte 
    (see logic controlling byteplace incrementing); 2) that samples
    never span 3 bytes in the character array (ruling out lengths of
    11, 13, 14, 15, and 17+.  Additionally, the bit shifting operations
    here are needless overhead for any length divisible by 8, meaning
    that this class is not the best way to do it.  These
    assumptions and observations restrict us (at least for the moment)
    to 9, 10, and 12 as allowable sample lengths.

    This class is a state machine that is intended to match the semantics
    of a FILE* (with the obvious exception that it goes into a uchar*
    rather than to a real file.  With that in mind, it does not support
    random access through the data.  Only a linear walk from front to back
    is allowed.
*/
class HByteStream {
  private:
    int byteplace;	    /*!< Current position from which to read/write */
    int bitplace;	    /*!< Distance, in bits, from the rightmost bit of the second byte. range:0..7 */
    int bitsperval;	    /*!< Number of bits per sample.  Must be 9, 10, or 12 */
    int allocated;	    /*!< Size of data.  Used for bounds checking in debug mode. */
    unsigned char *data;    /*!< The raw data to read from or write to. */
    unsigned short mask;    /*!< For convenience, a mask to remove garbage above the most significant bit. */
  public:
    HByteStream(int size, unsigned char *cptr, int bpv) {
	ASSERT( bpv == 9 || bpv == 10 || bpv == 12 );
	allocated = size;
	data = cptr; 
	bitsperval = bpv;
	byteplace = 0;
	bitplace = 16 - bitsperval; 
	mask = ((1<<bitsperval)-1);
    };
    HByteStream() {};
    /*< write a sample */
    void put(unsigned short value) {
	data[byteplace] |= (value >> (8 - bitplace));
	data[byteplace+1] |= (value << bitplace);
	bitplace -= bitsperval - 8;
	if( bitplace < 0 ) {
	    byteplace += 2; // consume two bytes
	    bitplace += 8;
	}
	else
	    byteplace++; // consume one byte
	ASSERT( byteplace <= allocated );
    };	    
    /*!< read a sample */
    void get(unsigned short &value) {
	value = (data[byteplace] << 8) | (data[byteplace + 1]);
	value = ((value >> bitplace) & mask );
	bitplace -= bitsperval - 8;
	if( bitplace < 0 ) {
	    byteplace += 2; // consume two bytes
	    bitplace += 8;
	}
	else
	    byteplace++; // consume one byte
	ASSERT( byteplace <= allocated );
    }; 

}; //end declaration of class HByteStream


