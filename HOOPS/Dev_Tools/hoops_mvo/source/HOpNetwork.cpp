
#include "HOpNetwork.h"
#include "HIOManager.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#ifdef _MSC_VER
#define int64_t __int64
#else
#include <inttypes.h>
#endif

#define NEXT_AVAIL_KEY	-100
#define LEN_TEXT_KEY	-101
#define NODE_KEY		-102
#define CONNECT_COUNT	-103

struct KeyPair {
	int64_t line_key;
	int64_t node_key;
};

template <typename T>
void BYTESWAP(T & d)
{
	T const tmp = d;
	char const * inptr = (char const*)&tmp;
	char * outptr = (char*)&d;
	for(unsigned int i = 0; i < sizeof(T); ++i){
		outptr[i] = inptr[sizeof(T) - 1 - i];
	}
}


template <typename T>
void LENDIAN(T & d)
{
	unsigned short const BOM = 0xfeff;
	if(*(unsigned char const*)&BOM == 0xfe){
		/* Big Endian */
		BYTESWAP(d);
	}
}

template <typename T>
void BENDIAN(T & d)
{
	unsigned short const BOM = 0xfeff;
	if(*(unsigned char const*)&BOM == 0xff){
		/* Big Endian */
		BYTESWAP(d);
	}
}

#define ENDIAN BENDIAN

HOpNetwork::HOpNetwork(HBaseView * view):HBaseOperator(view)
{
	SetOperatorStarted(true);
	m_SelectedLineKey = m_SelectedNodeKey = INVALID_KEY;
	HC_Open_Segment_By_Key(GetView()->GetModelKey());{
		m_NetworkKey = HC_KOpen_Segment("NetworkKey");{
			HC_Set_Selectability("off, markers = on");
			HC_Set_Visibility("off, lines = on, text = on");
			HC_Set_Text_Alignment("**");
			HC_Set_Heuristics("quick moves = on");
			m_NodesKey = HC_KOpen_Segment("nodes");{
				HC_Set_Visibility("on");
			}HC_Close_Segment();
		}HC_Close_Segment();
	}HC_Close_Segment();
}

HOpNetwork::~HOpNetwork()
{
	HC_Open_Segment_By_Key(m_NetworkKey);{
		HC_UnSet_Heuristics();
	}HC_Close_Segment();
	SetOperatorStarted(false);
}

HC_KEY HOpNetwork::GetNextKey()
{
	int64_t retval;
	int64_t next_key;

	HC_Open_Segment_By_Key(m_NetworkKey);{
		long rc = HC_Show_User_Data_Indices(0, 0);
		if(rc){
			HC_Show_One_User_Data(NEXT_AVAIL_KEY, &retval, sizeof(retval));
			ENDIAN(retval);
			next_key = retval +1;
		}
		else{
			retval = 2;
			next_key = 3;
		}
		ENDIAN(next_key);
		HC_Set_User_Data(NEXT_AVAIL_KEY, &next_key, sizeof(next_key));
	}HC_Close_Segment();

	return static_cast<HC_KEY>(retval -1);
}


HC_KEY HOpNetwork::My_Renumber_Key(HC_KEY key)
{
	HC_KEY retval = key;

	if(key < INVALID_KEY){
		HC_KEY next_key = GetNextKey();
		HC_Renumber_Key(key, next_key, "local");
		retval = next_key;
	}

	return retval;
}


void HOpNetwork::My_Delete_Line(HC_KEY key)
{
	HC_Open_Segment_By_Key(m_NetworkKey);{
		/* Delete the length text */
		int64_t text_key = INVALID_KEY;
		HC_Open_Geometry(key);{
			if(HC_Show_One_User_Data(LEN_TEXT_KEY, &text_key, sizeof(text_key)) != sizeof(text_key)){
				assert(text_key == INVALID_KEY);
			}
			ENDIAN(text_key);
		}HC_Close_Geometry();
		if(text_key != INVALID_KEY)
			HC_Delete_By_Key(static_cast<HC_KEY>(text_key));

		/* Open the line and set the new text key. */
		HC_Delete_By_Key(key);
	}HC_Close_Segment();
}

void HOpNetwork::My_Line_Common(HC_KEY key,
					float x1, float y1, float z1,
					float x2, float y2, float z2)
{
	char buffer[MVO_BUFFER_SIZE];

	/* Delete the length text */
	int64_t old_text_key = INVALID_KEY;
	HC_Open_Geometry(key);{
		if(HC_Show_One_User_Data(LEN_TEXT_KEY, &old_text_key, sizeof(old_text_key)) != sizeof(old_text_key)){
			assert(old_text_key == INVALID_KEY);
		}
		ENDIAN(old_text_key);
	}HC_Close_Geometry();
	if(old_text_key != INVALID_KEY)
		HC_Delete_By_Key(static_cast<HC_KEY>(old_text_key));

	/* Compute the new length */
	float x, y, z, length;

	x = x2 - x1;
	y = y2 - y1;
	z = z2 - z1;

	length = sqrt(x*x + y*y + z*z);

	/* Compute the new text position */
	x = static_cast<float>((x1 + x2) / 2.0);
	y = static_cast<float>((y1 + y2) / 2.0);
	z = static_cast<float>((z1 + z2) / 2.0);

	/* Insert the text */
	int64_t length_text_key = INVALID_KEY;

	HC_Show_Owner_By_Key(key, buffer);
	HC_Open_Segment(buffer);{
		sprintf(buffer, "%g' %.1f\"", floor(length), (length - floor(length)) * 12.0);
		length_text_key = HC_KInsert_Text(x, y, z, buffer);
		length_text_key = My_Renumber_Key(static_cast<HC_KEY>(length_text_key));
	}HC_Close_Segment();

	/* Open the line and set the new text key. */
	HC_Open_Geometry(key);{
		ENDIAN(length_text_key);
		HC_Set_User_Data(LEN_TEXT_KEY, &length_text_key, sizeof(length_text_key));
	}HC_Close_Geometry();
}

HC_KEY HOpNetwork::My_KInsert_Line(float x1, float y1, float z1,
								 float x2, float y2, float z2)
{
	HC_KEY key = HC_KInsert_Line(x1, y1, z1, x2, y2, z2);
	key = My_Renumber_Key(key);
	HC_Open_Geometry(key);{
		int64_t len = -1;
		ENDIAN(len);
		HC_Set_User_Data(LEN_TEXT_KEY, &len, sizeof(len));
	}HC_Close_Geometry();
	My_Line_Common(key, x1, y1, z1, x2, y2, z2);
	return key;
}

void HOpNetwork::My_Edit_Line(HC_KEY key,
							   float x1, float y1, float z1,
							   float x2, float y2, float z2)
{
	HC_Open_Segment_By_Key(m_NetworkKey);{
		HC_Edit_Line(key, x1, y1, z1, x2, y2, z2);
		My_Line_Common(key, x1, y1, z1, x2, y2, z2);
	}HC_Close_Segment();
}

HC_KEY HOpNetwork::My_KInsert_Node(float x, float y, float z)
{
	int64_t retval = My_Renumber_Key(HC_KCreate_Segment(""));

	HC_Open_Segment_By_Key(static_cast<HC_KEY>(retval));{
		HC_Open_Segment("");{
			HC_KEY marker_key = HC_KInsert_Marker(0, 0, 0);
			HC_Open_Geometry(marker_key);{
				ENDIAN(retval);
				HC_Set_User_Data(NODE_KEY, &retval, sizeof(retval));
				ENDIAN(retval);
			}HC_Close_Geometry();
		}HC_Close_Segment();
		HC_Translate_Object(x, y, z);
		HC_Set_User_Options("type = network node");
		int64_t count = 0;
		ENDIAN(count);
		HC_Set_User_Data(CONNECT_COUNT, &count, sizeof(count));
	}HC_Close_Segment();

	return static_cast<HC_KEY>(retval);
}

void HOpNetwork::My_Open_Node(HC_KEY key)
{
	key = Get_Node_Key(key);
	HC_Open_Segment_By_Key(key);
}

void HOpNetwork::My_Close_Node()
{
	HC_Close_Segment();
}

void HOpNetwork::My_Edit_Node(HC_KEY key, float x, float y, float z)
{
	HC_Open_Segment_By_Key(key);{
		HC_Flush_Contents(".", "modelling matrix");
		HC_Translate_Object(x, y, z);
	}HC_Close_Segment();
}

void HOpNetwork::Show_Node_Position(HC_KEY key, float &x, float &y, float &z)
{
	HC_Open_Segment_By_Key(m_NodesKey);{
		My_Open_Node(key);{
			float matrix[16];
			HC_Show_Modelling_Matrix(matrix);
			x = matrix[12];
			y = matrix[13];
			z = matrix[14];
		}My_Close_Node();
	}HC_Close_Segment();
}

HC_KEY HOpNetwork::Get_Node_Key(HC_KEY key)
{
	int64_t retval = INVALID_KEY;
	char key_type[32];
	char options[MVO_BUFFER_SIZE];

	HC_Show_Key_Type(key, key_type);

	if(streq(key_type, "marker"))	{
		HC_Open_Geometry(key);{
			if(HC_Show_One_User_Data(NODE_KEY, &retval, sizeof(retval)) != sizeof(retval)){
				assert(retval == INVALID_KEY);
			}
			ENDIAN(retval);
		}HC_Close_Geometry();
	}
	else if(streq(key_type, "segment")){
		HC_Open_Segment_By_Key(key);{
			if(HC_Show_Existence("user options = type")){
				HC_Show_One_User_Option("type", options);
				if(streq(options, "network node")){
					retval = key;
				}
			}
		}HC_Close_Segment();
	}

	return static_cast<HC_KEY>(retval);
}


int HOpNetwork::OnLButtonDown(HEventInfo &hevent)
{
	char view_segment[MVO_BUFFER_SIZE];
	HC_Show_Segment(GetView()->GetViewKey(), view_segment);

	HPoint window = hevent.GetMouseWindowPos();

	int count = HC_Compute_Selection(view_segment, 0, "v, selection level = entity", window.x, window.y);

	HC_Open_Segment_By_Key(m_NodesKey);{

		m_SelectedNodeKey = INVALID_KEY;

		/* If something was selected, see if it's one of our nodes... */
		if(count > 0){
			HC_KEY selected_key;
			int not_used;
			HC_Show_Selection_Element(&selected_key, &not_used, &not_used, &not_used);

			m_SelectedNodeKey = Get_Node_Key(selected_key);
		}

		/* If nothing was selected, or if the selected element wasn't one of our nodes... */
		if (m_SelectedNodeKey == INVALID_KEY) {
			HPoint world = hevent.GetMouseWorldPos();
			m_SelectedNodeKey = My_KInsert_Node(world.x, world.y, world.z);
		}

	}HC_Close_Segment();

	GetView()->Update();

	return HOP_OK;
}

int HOpNetwork::OnLButtonDownAndMove(HEventInfo &hevent)
{
	if(m_SelectedNodeKey != INVALID_KEY){
		HPoint world = hevent.GetMouseWorldPos();
		HC_Open_Segment_By_Key(m_NodesKey);{
			float x, y, z;
			Show_Node_Position(m_SelectedNodeKey, x, y, z);
			HPoint marker(x, y, z);
			HUtility::AdjustPositionToPlane(GetView(), world, marker);
			My_Edit_Node(m_SelectedNodeKey, world.x, world.y, world.z);

			int64_t connection_count = 0;
			My_Open_Node(m_SelectedNodeKey);{
				if(HC_Show_One_User_Data(CONNECT_COUNT, &connection_count, sizeof(connection_count)) != sizeof(connection_count)){
					assert(connection_count == 0);
				}
				ENDIAN(connection_count);
			}My_Close_Node();

			for(int i=0; i<connection_count; ++i){
				HC_KEY line_key, node_key;
				My_Open_Node(m_SelectedNodeKey);{
					struct KeyPair keys = {INVALID_KEY, INVALID_KEY};
					if(HC_Show_One_User_Data(i, &keys, sizeof(keys)) != sizeof(keys)){
						assert(keys.line_key == INVALID_KEY && keys.node_key == INVALID_KEY);
					}
					ENDIAN(keys.line_key);
					ENDIAN(keys.node_key);
					line_key = (HC_KEY)keys.line_key;
					node_key = (HC_KEY)keys.node_key;
				}My_Close_Node();
				Show_Node_Position(node_key, x, y, z);
				My_Edit_Line(line_key, x, y, z, world.x, world.y, world.z);
			}
		}HC_Close_Segment();
		GetView()->Update();
	}
	return HOP_OK;
}

int HOpNetwork::OnLButtonUp(HEventInfo &hevent)
{
	UNREFERENCED(hevent);

	m_SelectedNodeKey = INVALID_KEY;
	return HOP_OK;
}



int HOpNetwork::OnRButtonDown(HEventInfo &hevent)
{
	char view_segment[MVO_BUFFER_SIZE];
	HC_Show_Segment(GetView()->GetViewKey(), view_segment);

	HPoint window = hevent.GetMouseWindowPos();

	int count = HC_Compute_Selection(view_segment, 0, "v, selection level = entity", window.x, window.y);

	HC_Open_Segment_By_Key(m_NetworkKey);{

		m_SelectedLineKey = m_SelectedNodeKey = INVALID_KEY;

		/* If something was selected, see if it's one of our nodes... */
		if(count > 0){
			HC_KEY selected_key;
			int not_used;
			HC_Show_Selection_Element(&selected_key, &not_used, &not_used, &not_used);

			m_SelectedNodeKey = Get_Node_Key(selected_key);

			if(m_SelectedNodeKey != INVALID_KEY){
				SetFirstPoint(hevent.GetMouseWorldPos());
				m_SelectedLineKey = My_KInsert_Line(
					GetFirstPoint().x, GetFirstPoint().y, GetFirstPoint().z,
					GetFirstPoint().x, GetFirstPoint().y, GetFirstPoint().z);
				GetView()->Update();
			}
		}

	}HC_Close_Segment();

	return HOP_OK;
}

/*! If there's a line anchored at one end to the circle, move the other end. */
int HOpNetwork::OnRButtonDownAndMove(HEventInfo &hevent)
{
	if(m_SelectedLineKey != INVALID_KEY){
		HPoint world = hevent.GetMouseWorldPos();
		My_Edit_Line(m_SelectedLineKey,
				GetFirstPoint().x, GetFirstPoint().y, GetFirstPoint().z,
				world.x, world.y, world.z);
		GetView()->Update();
	}
	return HOP_OK;
}

/*! If there's a different circle ( from RButtonDown) under the mouse, anchor the line to
 *  this circle.  Otherwise, remove the line. */
int HOpNetwork::OnRButtonUp(HEventInfo &hevent)
{
	if(m_SelectedLineKey != INVALID_KEY){
		char view_segment[MVO_BUFFER_SIZE];
		HC_Show_Segment(GetView()->GetViewKey(), view_segment);

		HPoint window = hevent.GetMouseWindowPos();

		int count = HC_Compute_Selection(view_segment, 0, "v, selection level = entity", window.x, window.y);

		HC_Open_Segment_By_Key(m_NodesKey);{

			/* If something was selected, see if it's one of our icons... */
			if(count > 0){
				HC_KEY selected_key = INVALID_KEY;
				HC_KEY other_node_key = INVALID_KEY;
				int not_used;
				HC_Show_Selection_Element(&selected_key, &not_used, &not_used, &not_used);

				other_node_key = Get_Node_Key(selected_key);

				if(other_node_key != INVALID_KEY){
					int64_t connection_count = 0;
					float x, y, z;

					Show_Node_Position(other_node_key, x, y, z);
					SetNewPoint(HPoint(x, y, z));

					My_Open_Node(other_node_key);{
						connection_count = 0;
						if(HC_Show_One_User_Data(CONNECT_COUNT, &connection_count, sizeof(connection_count)) != sizeof(connection_count)){
							assert(connection_count == 0);
						}
						ENDIAN(connection_count);
						struct KeyPair keys = {m_SelectedLineKey, m_SelectedNodeKey};
						ENDIAN(keys.line_key);
						ENDIAN(keys.node_key);
						HC_Set_User_Data(static_cast<HC_POINTER_SIZED_INT>(connection_count), &keys, sizeof(keys));
						++connection_count;
						ENDIAN(connection_count);
						HC_Set_User_Data(CONNECT_COUNT, &connection_count, sizeof(connection_count));
					}My_Close_Node();

					Show_Node_Position(m_SelectedNodeKey, x, y, z);
					SetFirstPoint(HPoint(x, y, z));

					My_Open_Node(m_SelectedNodeKey);{
						connection_count = 0;
						if(HC_Show_One_User_Data(CONNECT_COUNT, &connection_count, sizeof(connection_count)) != sizeof(connection_count)){
							assert(connection_count == 0);
						}
						ENDIAN(connection_count);
						struct KeyPair keys = {m_SelectedLineKey, other_node_key};
						ENDIAN(keys.line_key);
						ENDIAN(keys.node_key);
						HC_Set_User_Data(static_cast<HC_POINTER_SIZED_INT>(connection_count), &keys, sizeof(keys));
						++connection_count;
						ENDIAN(connection_count);
						HC_Set_User_Data(CONNECT_COUNT, &connection_count, sizeof(connection_count));
					}My_Close_Node();

					My_Edit_Line(m_SelectedLineKey,
						GetFirstPoint().x, GetFirstPoint().y, GetFirstPoint().z,
						GetNewPoint().x, GetNewPoint().y, GetNewPoint().z);

				} else {
					My_Delete_Line(m_SelectedLineKey);
				}

			} else My_Delete_Line(m_SelectedLineKey);

		}HC_Close_Segment();

		m_SelectedLineKey = m_SelectedNodeKey = INVALID_KEY;
		GetView()->Update();
	}

	return HOP_OK;
}

