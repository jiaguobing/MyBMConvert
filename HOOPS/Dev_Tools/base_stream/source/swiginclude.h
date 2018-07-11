#ifndef HBINFILETKHEADER
#define HBINFILETKHEADER

#ifndef __cplusplus
#error C++ compiler required
#endif

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#ifdef _MSC_VER
    #ifndef HSTREAM_STATIC_LIB
        #ifdef HBINFILETK_EXPORTS
            #define HBINFILETK_API2  __declspec (dllexport)
        #else
            #define HBINFILETK_API2  __declspec (dllimport)
        #endif
    #else
        #ifndef BSTREAM_STATIC_LIB
            #define BSTREAM_STATIC_LIB
        #endif
        #define HBINFILETK_API2
    #endif
#else
    #define HBINFILETK_API2
#endif


#ifdef TEST_RIG_1
    #define HBINFILETK_API
#else
    #define HBINFILETK_API HBINFILETK_API2
#endif


#ifdef HSTREAM_READONLY
    #ifndef BSTREAM_READONLY
        #define BSTREAM_READONLY
    #endif
#endif


#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#endif


#ifndef HOPCODE_HANDLER
#define HOPCODE_HANDLER

class HTK_PolyCylinder : public TK_PolyCylinder {};

#endif //HOPCODE_HANDLER

