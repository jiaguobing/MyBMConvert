#ifndef __HP_BRIDGE_INTERNAL_H_
#define __HP_BRIDGE_INTERNAL_H_

extern "C" long HP_Compute_Geometry_Keys_internal(PK_ENTITY_t tagID, HC_KEY* keys, unsigned long geomTypes, long max_count);
extern "C" long HP_Compute_Geometry_Key_Count_internal(PK_ENTITY_t tagID, unsigned long geomTypes);


#ifdef _MSC_VER
#pragma warning(disable: 4127 4310 4244 4706 4206 4514 4100 4996 4702)
#endif

#endif
