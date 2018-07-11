//
//  cocoa_common.h
//  hoops_3dgs
//
//  Created by Evan on 1/4/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef hoops_3dgs_cocoa_common_h
#define hoops_3dgs_cocoa_common_h

#define Point OSX_Point
#define Style OSX_Style
#define Marker OSX_Marker
#define Polygon OSX_Polygon
#define Button OSX_Button
#define Line OSX_Line

#undef null
#define null OSX_NULL

#undef local

#include <AppKit/AppKit.h>
#include <Cocoa/Cocoa.h>
#include <Foundation/Foundation.h>
#include <OpenGL/CGLCurrent.h>
#include <OpenGL/CGLTypes.h>
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#include <ApplicationServices/ApplicationServices.h>
#include <OpenGL/CGLRenderers.h>

#undef null
#define null nullptr

#undef local
#define local static

#undef Point
#undef Style
#undef Marker
#undef Polygon 
#undef Button
#undef Line

typedef int DC_Type;
#define DCT_UNDEFINED		((DC_Type)0)
#define DCT_OUR_WINDOW		((DC_Type)1)
#define DCT_THEIR_WINDOW	((DC_Type)2)
#define DCT_IMAGE			((DC_Type)3)
#define DCT_PBUFFER_IMAGE	((DC_Type)4)
#define DCT_FBO_IMAGE		((DC_Type)5)
#define DCT_OSMESA_IMAGE	((DC_Type)6)

#define Debug_FORCE_SOFTWARE		0x01000000

@interface HView : NSView
{
@public int mouseButtons;
@public bool mouseWasQueued;
@public int mouseX;
@public int mouseY;
}

- (void) drawRect: (NSRect) bounds;

- (NSPoint) convertMouseLocation: (NSEvent *) theEvent;
- (void) consumeMouse: (NSEvent *) theEvent;
- (void) mouseDown: (NSEvent *) theEvent;
- (void) mouseUp: (NSEvent *) theEvent;
- (void) mouseDragged: (NSEvent *) theEvent;
- (void) keyDown: (NSEvent *) theEvent;
- (void) reshape;

- (BOOL) acceptsFirstResponder;
- (BOOL) becomeFirstResponder;
- (BOOL) resignFirstResponder;

- (id)initWithCoder:(NSCoder *)decoder;
- (void)encodeWithCoder:(NSCoder *)encoder;

- (BOOL)acceptsFirstMouse:(NSEvent *)theEvent;
- (void) dealloc;

@end

static NSWindow * create_standalone_window(Display_Context const * dc) {
    
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    
    {
        BOOL error = NSApplicationLoad();
        ASSERT(error == YES);
    }
    
    // [NSApp activateIgnoringOtherApps : YES];
    
    NSRect frame = NSMakeRect(0, 0, dc->outer_extent.right - dc->outer_extent.left, dc->outer_extent.top - dc->outer_extent.bottom );
    
    NSWindow * window  = [[NSWindow alloc] initWithContentRect : frame
                                          styleMask : NSTitledWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask | NSResizableWindowMask
                                            backing : NSBackingStoreBuffered
                                              defer : false];
    
    NSString * title = nil;
    if (dc->options.title.length() > 0)
        title = [NSString stringWithUTF8String:dc->options.title.text()];
    else
        title = @"Unnamed window";
    
    [window setTitle: title];
    [window setReleasedWhenClosed: NO];
    //[window setAcceptsMouseMovedEvents: YES];
    
    NSView * view = [[HView alloc] initWithFrame : frame];
    
    NSUInteger mask = NSViewWidthSizable | NSViewHeightSizable;
    [view setAutoresizingMask:mask];
    
    [window	setContentView: view];
    {
        BOOL error = [window makeFirstResponder: view];
        ASSERT(error == YES);
    }
    
    [pool drain];
    
    return window;
}

struct CocoaData
{
    NSView *view;
	NSWindow *window;
	CGDirectDisplayID display_id;
    NSOpenGLContext * context;
    Display_Context * dc;
    
    CocoaData(Display_Context * dc) :
        dc(dc), 
        view(null), 
        window(null), 
        context(null) {
            
        display_id = CGMainDisplayID();        
    }
    
    template <class RECT_TYPE> RECT_TYPE getWindowBounds() {	
        
        NSRect frame = [view frame];
        RECT_TYPE rect;
        
        rect.left = frame.origin.x;
        rect.right = frame.origin.x + frame.size.width;
        rect.top = frame.origin.y;
        rect.bottom = frame.origin.y + frame.size.height;
        
        return rect;
    }    
    
    template <class OGLDATA> NSOpenGLPixelFormat * choosePixelFormat(NSOpenGLContext * shareCtx = nil) {
                
        OGLDATA *ogldata = ((OGLDATA *)((dc)->data2));        
        NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
        
        int i = 0;
        GLuint attrs[32];
        
        if (ANYBIT(dc->options.debug, Debug_FORCE_SOFTWARE)) {            
            attrs[i++] = NSOpenGLPFARendererID;
            attrs[i++] = kCGLRendererGenericFloatID;
        }
        
        if (ogldata->dc_type == DCT_PBUFFER_IMAGE) {
            attrs[i++] = NSOpenGLPFAPixelBuffer;
        }
        else {
            attrs[i++] = NSOpenGLPFAWindow;
        }
        
        attrs[i++] = NSOpenGLPFAColorSize;
        attrs[i++] = 24;	
        attrs[i++] = NSOpenGLPFADepthSize;
        attrs[i++] = 24;
        
        if (dc->options.double_buffering) {
            attrs[i++] = NSOpenGLPFADoubleBuffer;
            attrs[i++] = NSOpenGLPFABackingStore;
        }
        
        
        if (dc->options.anti_alias_requested && dc->options.anti_alias_samples != 0) {
            
            attrs[i++] = NSOpenGLPFAMultisample;
            attrs[i++] = NSOpenGLPFASampleBuffers;
            attrs[i++] = (NSOpenGLPixelFormatAttribute)1;
            attrs[i++] = NSOpenGLPFASamples;			
            
            if (dc->options.anti_alias_samples < 0)
                attrs[i++] = (NSOpenGLPixelFormatAttribute)2;
            else
                attrs[i++] = (NSOpenGLPixelFormatAttribute)dc->options.anti_alias_samples;
        }
        
        if (ANYBIT (dc->options.bits, Drivo_STENCIL_REQUESTED)) {
            attrs[i++] = NSOpenGLPFAStencilSize;
            attrs[i++] = 8;
        }
        
        attrs[i++] = NSOpenGLPFAAccumSize;
        attrs[i++] = 32;	
         
        
        // terminate
        attrs[i++] = 0;	
        ASSERT(i <= countof(attrs));
        
        NSOpenGLPixelFormat* pixFmt = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];	
        
        // we really need to try some fallback pixel formats here but for now just die
        if(pixFmt == nil) {		
            
            HE_ERROR (HEC_OPENGL_DRIVER, HES_CANNOT_SET_ATTRIBUTE, "Couldn't create pixel format in CocoaData::choosePixelFormat");
            HD_Kill_Driver (dc);
            return nil;
        }	
        
        context = [[NSOpenGLContext alloc] initWithFormat: pixFmt shareContext: shareCtx];            
        
        if (ogldata->dc_type == DCT_OUR_WINDOW || ogldata->dc_type == DCT_THEIR_WINDOW) {
            [context setView:view];    
        }    
        
        {
            GLint backing_store = 0;
            [pixFmt getValues : &backing_store forAttribute : NSOpenGLPFABackingStore forVirtualScreen : 0];
            
            if (backing_store)
                dc->physical.double_buffer_method = Double_Buffer_SWAP;
            else
                dc->physical.double_buffer_method = Double_Buffer_COPY;		
        }        
        
        
        [pool drain];	
        
        // not exactly sure how this would happen...
        if (!context) {
            HE_ERROR (HEC_OPENGL_DRIVER, HES_CANNOT_SET_ATTRIBUTE, "no valid context at the end of choose_pixel_format");
            HD_Kill_Driver (dc);		
        }
        
        return pixFmt;
    }
    
};

local void check_events(Display_Context const * dc) {
	
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];		
	
	NSUInteger mask = NSAnyEventMask;		
	NSEvent *event = 0;
    
    HView * view = (HView*)dc->current.window_ID;    
    ASSERT([view isKindOfClass:[HView class]]); 	
    
	view->mouseWasQueued = false;
    
	
	while ((event = [NSApp nextEventMatchingMask:mask 
                                       untilDate:nil 
                                          inMode:NSDefaultRunLoopMode
                                         dequeue:YES])) {		
        
        [NSApp sendEvent : event]; 	
	}
	
	[pool drain];
}

local bool request_location(Display_Context const * dc, int * button, int * x, int * y) {   
    
    HView * view = (HView*)dc->current.window_ID;    
    ASSERT([view isKindOfClass:[HView class]]); 
        
    check_events(dc);     
        
    /*
     * This function must always report a mouse "state",
     * so if check_events did not report an activity via
     * Queue_Pixel_Location_Event, we must report one here.
     * We will just re-report the last mouse state that
     * we saw.
     */
    if (view->mouseWasQueued)
        return false;
    else {
        *button = view->mouseButtons;
        *x = view->mouseX;
        *y = view->mouseY;
        return true;
    }    
    
    return false;    
    
}

local bool request_keyboard (Display_Context const * dc, int * button, int * status) {    

    check_events(dc);    
    
    /*
     * check_events will always have reported any keystrokes
     * to Hoops via Queue_Keyboard_Event.  Since Hoops doesn't
     * need to know the "state" of the keyboard, this function
     * can safely return false.
     */
    
    return false;
}

local NSOpenGLPixelBuffer * create_pbuffer(int width, int height) {
	
	NSOpenGLPixelBuffer  *pbuffer = [[NSOpenGLPixelBuffer alloc] initWithTextureTarget: GL_TEXTURE_RECTANGLE_ARB
																 textureInternalFormat:GL_RGBA
																 textureMaxMipMapLevel: 0
																			pixelsWide:width
																			pixelsHigh:height];	
	
    return pbuffer;
}	

template <class OGLDATA>
local void cocoa_get_current_info(Display_Context * dc) {
    
    OGLDATA *ogldata = ((OGLDATA *)((dc)->data2));    
    CocoaData * cocoaData = ogldata->cocoaData;
	
	if (dc->options.use_window_ID != 0) {
		dc->options.subscreen_moving = true;
		dc->options.subscreen_resizing = true;
		dc->options.subscreen_stretching = true;
		dc->options.subscreen_by_program = false;
	}
    
	/* OpenGL always takes 8 bits per rgb */
	dc->current.number_of_colors = 1L << 24;
	dc->current.first_color = 0;
	
	if (ogldata->dc_type == DCT_IMAGE || 
        ogldata->dc_type == DCT_PBUFFER_IMAGE) {
        
		dc->current.extent.left = (float) 0;
		dc->current.extent.right = (float) dc->physical.number_of_pixels.x - 1;
		dc->current.extent.bottom = (float) 0;
		dc->current.extent.top = (float) dc->physical.number_of_pixels.y - 1;
	}
	else if (cocoaData->view) {		
		
		Rect rect = cocoaData->getWindowBounds<Rect>();
		
		dc->current.extent.left = (float) rect.left;
		dc->current.extent.right = (float) rect.right - 1;
		dc->current.extent.bottom = (float) (ogldata->y_screen_fudge - (rect.bottom - 1));
		dc->current.extent.top = (float) (ogldata->y_screen_fudge - rect.top);
        
	}
	else {	
		
		dc->current.extent.left = 0.0f;
		dc->current.extent.right = CGDisplayPixelsWide(cocoaData->display_id);
		dc->current.extent.bottom = 0.0f;
		dc->current.extent.top = CGDisplayPixelsHigh(cocoaData->display_id);
	}
}

local unsigned int ogl_bundle_users = 0;
local CFBundleRef ogl_bundle_ref = 0;

local void * GetFunctionPointerFromBundle(const char *name, CFBundleRef theBundle)
{
	void *function = 0;
	
	if (theBundle) {
		CFStringRef strRef = CFStringCreateWithCString(CFAllocatorGetDefault(), name, kCFStringEncodingMacRoman);
		function = CFBundleGetFunctionPointerForName(theBundle, strRef);
		CFRelease(strRef);
	}
	return function;
}

local void * MyCocoaGetProcAddress(const char *function_name)
{
	
	if (!ogl_bundle_ref) {
		return 0;
	}
	
	return GetFunctionPointerFromBundle(function_name, ogl_bundle_ref);	
}

local CFBundleRef LoadSystemFrameWorkBundle(const char *frameworkName)
{
	FSRef folderRef, dstRef;
	CFBundleRef bRef = NULL;
	
	if (FSFindFolder(kSystemDomain, kFrameworksFolderType, false, &folderRef) == noErr) {
		
		HFSUniStr255 hfs;
		CFStringRef cfStr;
		
		/* Convert C string to hfs [unicode] data struct */
		cfStr = CFStringCreateWithCString(CFAllocatorGetDefault(), frameworkName, kCFStringEncodingMacRoman);
		hfs.length = CFStringGetLength(cfStr);
		CFStringGetCharacters(cfStr, CFRangeMake(0,hfs.length), hfs.unicode);
		CFRelease(cfStr);
		
		if (FSMakeFSRefUnicode(&folderRef, hfs.length, hfs.unicode, CFStringGetSystemEncoding(), &dstRef) == noErr) {
			
			CFURLRef urlRef = CFURLCreateFromFSRef(CFAllocatorGetDefault(), &dstRef);
			
			if (urlRef) {
				bRef=CFBundleCreate(CFAllocatorGetDefault(), urlRef);
				CFRelease(urlRef);
			}
		}
	}  
	/* remember to relase this when you are done!! */
	return bRef;	  
}


/*****************************************************************************
 *****************************************************************************
 FONT_ACTIONS
 *****************************************************************************
 *****************************************************************************/

/*
 * Font actions.
 */


/*
 * ACTION
 */
local void
draw_dc_text (Net_Rendition const &  nr,
			  DC_Point const * where,
			  int count, Karacter const * kstring) {
	
	printf("draw_dc_text \n");
	
}


/*
 * ACTION
 */
local void draw_font (Net_Rendition const &	nr,
                      Font_Instance const		*instance,
                      Point const				*position,
                      int							count,
                      Karacter const			*kp,
                      Vector const				*scale) {
	
	HD_Draw_Stenciled_Font (nr, instance, position, count, kp, scale);
	
}


/*
 *	* ACTION
 *	 */
local bool measure_char(
						Net_Rendition const & nr,
						Font_Instance const * instance,
						Karacter kar,
						float *size_out,
						bool *kar_missing,
						void ** per_kar_data) {
	
	return true;
}


struct OGLFontData {
	GLuint list[0xFFFF];
	CGFontRef family;
	OSX_Style face;
};

/*
 * ACTION
 */
local void * 
load_font (Display_Context const * dc,
		   char const * name,
		   Font_Instance * instance) {
	
	OGLFontData *fontdata;
	ZALLOC (fontdata, OGLFontData);	
	
	CFStringRef cfName = CFStringCreateWithCString(0,name,kCFStringEncodingUTF8);
    
	fontdata->family = CGFontCreateWithFontName(cfName);	
	
	if (fontdata->family == 0) {
		FREE (fontdata, OGLFontData);
		return 0;
	}	
	
	fontdata->face = 0;
	
	SET_MEMORY(fontdata->list, sizeof(GLuint)*0xFFFF,0);
	
	return (void *) fontdata;
}


/*
 * ACTION
 */
local void
unload_font (Display_Context const * dc, Font_Instance *instance) {
	
}


/*
 * ACTION
 */
local void
find_all_fonts (Display_Context * dc) {
	
    /*
     const char * font_names[] = {
     "applFont", "los Angeles", "athens","monaco", "Futura",
     "cairo", "sanFran", "courier","times" ,
     "Geneva","symbol", "helvetica", "systemFont",
     "mobile","toronto", "new York","venice", "london",""}; 
     */
	
}


#endif
