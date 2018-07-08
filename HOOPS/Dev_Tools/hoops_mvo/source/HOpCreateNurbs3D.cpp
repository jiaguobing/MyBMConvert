// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "HTools.h"

#include "HBaseOperator.h"
#include "HEventInfo.h"
#include "HBaseView.h"
#include "HOpCreateNurbs3D.h"
#include "HStream.h"
#include "HSharedKey.h"
#include "HUtilityGeometryCreation.h"
#include "HSelectionSet.h"
 

#define GWIDTH 0.75
#define GRAN 30
#define MN_NUM 4
#define DEGREE 2
#define SURFACETYPE InternalNurbsType

static int xp0 = -1;

/////////////////////////////////////////////////////////////////////////////
// CreateNurbs3D
//
// Operator for inserting a Nurbs surface into a newly created Scene/Part segment.
// new surface will be created when operator is selected
// existing surface can be modified if selected
// Mouse down - select control point for manipulation
// Mouse motion while down - modify control point position
 
HOpCreateNurbs3D::HOpCreateNurbs3D(HBaseView* view, int DoRepeat, int DoCapture) : HBaseOperator(view, DoRepeat, DoCapture)
{
	HSelectionSet * selection = GetView()->GetSelection();
	m_NurbsKey = 0;
	m_pNurbsSurface = 0;
	int size = selection->GetSize();
  	if (size != 1)
	{
		m_pNurbsSurface = new HNurbsSurface;
		m_NurbsKey = InsertNurbs(view, m_pNurbsSurface,true);

	}
	else 
	{
		char type[MVO_BUFFER_SIZE];
		HC_KEY key = selection->GetAt(0);
		HC_Show_Key_Type(key, type);
		if (strcmp(type,"segment") == 0)
		{
			HC_Open_Segment_By_Key(key);
			HUserOptions text;
			text.Show_One_Net_User_Option("nurbs_surface");
			if (strcmp(text.m_data,"") != 0)
			{
				HC_Set_Selectability("everything = off");
				m_NurbsKey = key;
				m_pNurbsSurface = new HNurbsSurface;
				m_pNurbsSurface->Init(key);
				m_pNurbsSurface->DisplayVertices();
				m_pNurbsSurface->DisableSelectability();
				view->Update();	
			}
			HC_Close_Segment();							
		}
	}
	
	GetView()->Update();
	
	
}

HOpCreateNurbs3D::~HOpCreateNurbs3D()
{
	if (m_pNurbsSurface)
	{
		HC_KEY key;
		char type[MVO_BUFFER_SIZE];
		HC_Open_Segment_By_Key(GetView()->GetModel()->GetModelKey());
 			HC_Begin_Contents_Search("...","mesh, nurbs surface");
		while (HC_Find_Contents(type,&key))
		{
			if (HC_KShow_Owner_By_Key(key) == m_pNurbsSurface->GetKey())
			{
				m_pNurbsSurface->HideVertices();
				m_pNurbsSurface->EnableSelectability();
				GetView()->Update();	
 				EmitFinishNurbsEditing(GetView(), m_NurbsKey, true);	
				break;
			}
		}
		HC_End_Contents_Search();
		delete m_pNurbsSurface;
		m_pNurbsSurface = 0;
 		HC_Close_Segment();
	}
 
}


HBaseOperator * HOpCreateNurbs3D::Clone()
{
	return new HOpCreateNurbs3D(GetView());
}

const char * HOpCreateNurbs3D::GetName() { return "HOpCreateNurbs3D"; }

int HOpCreateNurbs3D::OnLButtonUp(HEventInfo &event)
{
    // Mouse has come up 
	if(!OperatorStarted()) return HBaseOperator::OnLButtonUp(event);

	SetOperatorStarted(false);
 	xp0 = -1;
	return HOP_READY;
}

 
/////////////////////////////////////////////////////////////////////////////
// OpCreateNurbs3D message handlers


int HOpCreateNurbs3D::OnLButtonDown(HEventInfo &event)
{
 	HPoint new_point;
	char tname[MVO_BUFFER_SIZE];
	HC_KEY key;
	int xp1,xp2;
	int temp;

	if (!OperatorStarted()) 
		SetOperatorStarted(true);

 	SetFirstPoint(event.GetMousePixelPos());
	HC_Open_Segment_By_Key(((HBaseView *)GetView())->GetViewKey());
		HPoint first_point = GetFirstPoint();
 	    HC_Compute_Coordinates ("./scene/overwrite", "local pixels", &first_point, "local window", &new_point);

		if (HC_Compute_Selection( ".", "./scene/overwrite","v, selection level = entity, no related selection limit",new_point.x,new_point.y)>0)
		{
			do {
				HC_Show_Selection_Element(&key,&temp,&xp1,&xp2);
				HC_Show_Key_Type(key,tname);
				if (streq(tname,"mesh"))
				{
					if (HC_KShow_Owner_By_Key(HC_KShow_Owner_By_Key(key)) == m_NurbsKey)
						xp0 = temp;
				}
			}while (HC_Find_Related_Selection());
		}

		HC_Compute_Coordinates ("./scene/overwrite", "local pixels", &first_point, "object", &m_last_point);
	HC_Close_Segment();
	GetView()->Update();
    return HOP_OK;
}


int HOpCreateNurbs3D::OnLButtonDownAndMove(HEventInfo &event)
{
			float matrix[16];
	HPoint temp;

 	if (!OperatorStarted()) return HBaseOperator::OnLButtonDownAndMove(event);
	
   	
	SetNewPoint(event.GetMouseViewpointPos());

	if (xp0!=-1)
	{
		HPoint new_point = GetNewPoint();
		HPoint cpoint;
	 		
		HC_Open_Segment_By_Key(((HBaseView *)GetView())->GetSceneKey());{
			HC_Compute_Coordinates (".", "viewpoint", &new_point,"projection", &new_point);

			HC_Open_Segment_By_Key(m_NurbsKey);{
				HC_Show_Net_Modelling_Matrix(matrix);
				m_pNurbsSurface->ShowVertex(xp0,cpoint);
				HC_Compute_Transformed_Points(1,&cpoint,matrix,&temp);
			}HC_Close_Segment();
		 
 			HC_Compute_Coordinates (".", "object", &temp, "projection", &temp);
			new_point.z = temp.z;

			HC_Compute_Coordinates (".", "projection", &new_point, "object", &new_point);
		}HC_Close_Segment();
		
		HC_Open_Segment_By_Key(m_NurbsKey);{
			HC_Show_Net_Modelling_Matrix(matrix);
			HC_Compute_Matrix_Inverse(matrix, matrix);
			HC_Compute_Transformed_Points(1, &new_point, matrix, &new_point);
			m_pNurbsSurface->UpdateVertex(xp0, new_point.x, new_point.y, new_point.z);
			if (GetView()->GetSharedKey())
				GetView()->GetSharedKey()->DisAssociateKey(m_pNurbsSurface->GetSurfaceMeshKey());			

			m_pNurbsSurface->Recalculate();
			if (GetView()->GetSharedKey())
				GetView()->GetSharedKey()->AssociateKey(m_pNurbsSurface->GetSurfaceMeshKey());

			EmitModifyControlPointMessage(GetView(),m_NurbsKey,xp0, new_point.x, new_point.y, new_point.z);
			GetView()->Update();
		}HC_Close_Segment();
		SetNewPoint(new_point);
	}		
		
 	return HOP_OK;
}



HC_KEY HOpCreateNurbs3D::InsertNurbs(HBaseView *pView, HNurbsSurface *pNurbsSurface, bool emit_message)
{
	HC_KEY key;

	HPoint target;

	HC_Open_Segment_By_Key(pView->GetSceneKey());
	HC_Show_Net_Camera_Target(&target.x, &target.y, &target.z);
	HC_Close_Segment();

 	HC_Open_Segment_By_Key(pView->GetModel()->GetModelKey());

	HPoint test1, test2;
	test1.Set(-1,0,0);
	test2.Set(1,0,0);
	HC_Open_Segment_By_Key(pView->GetSceneKey());
	HC_Compute_Coordinates(".","local window",&test1,"viewpoint",&test1);
	HC_Compute_Coordinates(".","local window",&test2,"viewpoint",&test2);
	HC_Close_Segment();

	//float size = (float)fabs(test2.x-test1.x)/2.0f/(MN_NUM+1);
	float size = (float)fabs(test2.x-test1.x)/(MN_NUM+1);

	HPoint axis[3];
	axis[0].Set(1.0f, 0.0f,0.0f);
	axis[1].Set(0.0f, 1.0f,0.0f);
	axis[2].Set(0.0f, 0.0f,1.0f);

	if (emit_message)
	{
		HBaseViewEmitMessageFunction emit_message_function = pView->GetEmitMessageFunction();
		if (emit_message_function)
		{
			char message[MVO_BUFFER_SIZE];

			HCLOCALE(sprintf(message, "H_INSERT_NURBS %f %f %f %f %f %f %f %f %f %f %f %f %f ",
				target.x, target.y, target.z,
				axis[0].x, axis[0].y, axis[0].z,
				axis[1].x, axis[1].y, axis[1].z,
				axis[2].x, axis[2].y, axis[2].z,
				size));
		
			emit_message_function(message, 0, pView->GetEmitMessageFunctionUserData());
		}
	}

 	key = pNurbsSurface->Init(&target, axis, MN_NUM, MN_NUM, DEGREE, GRAN, size, SURFACETYPE);

	if (pView->GetSharedKey())
	{
		pView->GetSharedKey()->AssociateKey(key);
		pView->GetSharedKey()->AssociateKey(pNurbsSurface->GetSurfaceMeshKey());
	}

	HC_Close_Segment();
	pNurbsSurface->DisplayVertices();
	pNurbsSurface->DisableSelectability();
	pView->Update();	
	return key;
			
}
 


void HOpCreateNurbs3D::InsertNurbsFromMessage(
	const char *in_data, 
	unsigned int data_length, 
	HBaseView *pView)
{
    char data[MVO_MESSAGE_BUFFER_SIZE];
    memcpy(data, in_data, data_length);
    data[data_length] = '\0';
	
	float size;
	HPoint target;
	HPoint axis[3];

    HCLOCALE (sscanf(data, "%f %f %f %f %f %f %f %f %f %f %f %f %f ",
				&target.x, &target.y, &target.z,
				&axis[0].x, &axis[0].y, &axis[0].z,
				&axis[1].x, &axis[1].y, &axis[1].z,
				&axis[2].x, &axis[2].y, &axis[2].z,
				&size));

	HNurbsSurface * pNurbsSurface = new HNurbsSurface;
		
 	HC_Open_Segment_By_Key(pView->GetModel()->GetModelKey());

	HC_KEY key = pNurbsSurface->Init(&target, axis, MN_NUM, MN_NUM, DEGREE, GRAN, size, SURFACETYPE);

	if (pView->GetSharedKey()) {

		pView->GetSharedKey()->AssociateKey(key);
		pView->GetSharedKey()->AssociateKey(pNurbsSurface->GetSurfaceMeshKey());
	}

	HC_Close_Segment();
	pNurbsSurface->DisplayVertices();
	pNurbsSurface->DisableSelectability();
	pView->Update();	

	delete pNurbsSurface;

}



void HOpCreateNurbs3D::EmitModifyControlPointMessage(HBaseView *pView, HC_KEY key, int control_point, float x, float y, float z)
{
	UNREFERENCED(control_point);

	HBaseViewEmitMessageFunction emit_message_function = pView->GetEmitMessageFunction();
	if (emit_message_function)
	{
		long ident;
		if (pView->GetSharedKey() && (ident = pView->GetSharedKey()->GetIdentFromKey(key)) != 0)
		{

			char message[MVO_BUFFER_SIZE];
			HCLOCALE(sprintf(message, "H_MODIFY_CONTROL_POINT %ld %d %f %f %f",ident, xp0,x,y,z));
		
			emit_message_function((const char *)message, 
				0, pView->GetEmitMessageFunctionUserData());
		}
	}
}


void HOpCreateNurbs3D::ModifyControlPointFromMessage(const char *in_data, unsigned int data_length, HBaseView *pView)
{
    char data[MVO_MESSAGE_BUFFER_SIZE];
	float x,y,z;
    memcpy(data, in_data, data_length);
    data[data_length] = '\0';
	
	long ident;
	HC_KEY key;
	int control_point;
    HCLOCALE (sscanf(data, "%ld %d %f %f %f", &ident, &control_point, &x, &y, &z)); 
	
	if (pView->GetSharedKey() && (key = pView->GetSharedKey()->GetKeyFromIdent(ident)) != 0)
	{
		HNurbsSurface nurbs;
		nurbs.Init(key);
		nurbs.UpdateVertex(control_point,x,y,z);
		pView->GetSharedKey()->DisAssociateKey(nurbs.GetSurfaceMeshKey());			
		nurbs.Recalculate();								
 		pView->GetSharedKey()->AssociateKey(nurbs.GetSurfaceMeshKey());
	}

	pView->Update();	
}




void HOpCreateNurbs3D::EmitFinishNurbsEditing(HBaseView *pView, HC_KEY key, bool emit_message)
{
	HBaseViewEmitMessageFunction emit_message_function = pView->GetEmitMessageFunction();
	if (emit_message)
	{
		if (emit_message_function)
		{
			long ident;
			if (pView->GetSharedKey() && (ident = pView->GetSharedKey()->GetIdentFromKey(key)) != 0)
			{
				char message[MVO_BUFFER_SIZE];
				sprintf(message, "H_FINISH_NURBS_EDITING %ld",ident);			
				emit_message_function((const char *)message, 
					0, pView->GetEmitMessageFunctionUserData());
			}
			
		}
	}
}


void HOpCreateNurbs3D::FinishNurbsEditingFromMessage(const char *in_data, unsigned int data_length, HBaseView *pView)
{
    char data[MVO_MESSAGE_BUFFER_SIZE];
    memcpy(data, in_data, data_length);
    data[data_length] = '\0';
	
	long ident;
	HC_KEY key;
    sscanf(data, "%ld", &ident); 	
	if (pView->GetSharedKey() && (key = pView->GetSharedKey()->GetKeyFromIdent(ident)) != 0)
	{	
		HNurbsSurface nurbs;
		nurbs.Init(key);
		nurbs.HideVertices();
		nurbs.EnableSelectability();
		pView->Update();	
	}
}





HC_KEY HNurbsSurface::GetKey()
{
	return m_nurbsKey;
}




void HNurbsSurface::DisplayVertices()
{
	HC_Open_Segment_By_Key(m_nurbsKey);
	{
		HC_Open_Segment("settings");
		{
			HC_Set_Visibility("geometry=off, edges=(off,mesh quads), markers=on");
		}
		HC_Close_Segment();
	}
	HC_Close_Segment();
}



void HNurbsSurface::HideVertices()
{
	HC_Open_Segment_By_Key(m_nurbsKey);
	{
		HC_Open_Segment("settings");
		{
			HC_Set_Visibility("everything = off");
		}
		HC_Close_Segment();
	}
	HC_Close_Segment();
}



void HNurbsSurface::EnableSelectability()
{
	HC_Open_Segment_By_Key(m_nurbsKey);
	{
			HC_UnSet_Selectability();
	}
	HC_Close_Segment();
}



void HNurbsSurface::DisableSelectability()
{
	HC_Open_Segment_By_Key(m_nurbsKey);
	{
		HC_Set_Selectability("everything = off");
	}
	HC_Close_Segment();
}


void HNurbsSurface::CreateDefaultVertices(HPoint * center, HPoint * axis)
{
	float x,z;
	int i=0;
	int j=0;

	z=-(m_vertexDistance*m_numUVertices/2.0f);

 	for (i=0;i<=m_numUVertices;i++) {

		x=-(m_vertexDistance*m_numUVertices/2.0f);

 		for (j=0;j<=m_numUVertices;j++) {

 			m_pVertices[j+i*(m_numUVertices+1)].x=x;

			m_pVertices[j+i*(m_numUVertices+1)].y=0.0;

			m_pVertices[j+i*(m_numUVertices+1)].z=z;

			x+=m_vertexDistance;
		}

		z+=m_vertexDistance;
	}

	m_pVertices[m_numUVertices/2+m_numUVertices/2*(m_numUVertices+1)].y = m_vertexDistance;
	
	float matrix[4][4];

	matrix[0][0] = axis[0].x;
	matrix[0][1] = axis[0].y;
	matrix[0][2] = axis[0].z;
	matrix[0][3] = 0.0f;

	matrix[1][0] = axis[1].x;
	matrix[1][1] = axis[1].y;
	matrix[1][2] = axis[1].z;
	matrix[1][3] = 0.0f;

	matrix[2][0] = axis[2].x;
	matrix[2][1] = axis[2].y;
	matrix[2][2] = axis[2].z;
	matrix[2][3] = 0.0f;

	matrix[3][0] = center->x;
	matrix[3][1] = center->y;
	matrix[3][2] = center->z;
	matrix[3][3] = 1.0f;

	HC_Compute_Transformed_Points(j*i, m_pVertices, (float*)matrix, m_pVertices);

}		




void HNurbsSurface::UpdateVertex(int vertexnum, float x, float y, float z)
{
 	m_pVertices[vertexnum].Set(x,y,z);
	int row, column;
	row = vertexnum /(m_numUVertices + 1);
	column = vertexnum - row *(m_numVVertices+1);
  	HC_Edit_Mesh(m_verticesKey,row, column,1,1,&m_pVertices[vertexnum]);
}




void HNurbsSurface::ShowVertex(int vertexnum, HPoint &v)
{
 	v = m_pVertices[vertexnum];
}


 void HNurbsSurface::UpdateVertices()
{
	if (m_verticesKey)
		HC_Delete_By_Key(m_verticesKey);
	HC_Open_Segment_By_Key(m_nurbsKey);
	{
		HC_Open_Segment("settings");
		{
			m_verticesKey = HC_KInsert_Mesh(m_numUVertices+1, m_numVVertices+1,m_pVertices);
		}
		HC_Close_Segment();
	}
	HC_Close_Segment();
}


void HNurbsSurface::UpdateParameters()
{
	HC_Open_Segment_By_Key(m_nurbsKey);
	{
		HC_Open_Segment("settings");
		{
			char text[MVO_BUFFER_SIZE];
			HCLOCALE(sprintf(text,"nurbs_settings = %d %f %d %d %d",m_numUVertices, m_vertexDistance, m_tesselationLevel, m_degree, m_surfaceType));
			HC_Set_User_Options(text);
		}
		HC_Close_Segment();
	}
  	HC_Close_Segment();
}

float HNurbsSurface::nip(int i,int p, float u, float *uv)
{
	float erg1,erg2;
	if (p==0)
	{
		if (uv[i]<=u  && u<uv[i+1])
			return 1;
		else
			return 0;
	}
 	erg1=(u-uv[i])/(uv[i+p]-uv[i]+0.00001f);
	if (erg1!=0.0)
	erg1=erg1*nip(i,p-1,u,uv);
	erg2=(uv[i+p+1]-u)/(uv[i+p+1]-uv[i+1]+0.00001f);
	if (erg2!=0.0)
		erg2=erg2*nip(i+1,p-1,u,uv);
 	return erg1+erg2;
}

void HNurbsSurface::InsertNurbsMesh(HPoint *pNurbs)
{

	if (m_surfaceType == NurbsType)
	{
 	float uv[500];
	float uvad;
	float cpad;
	int i,j,k,l;
	float xp,yp,zp,dp;
	float temp;
	float u=0,v=0;
	int u_num = m_tesselationLevel;
  	uvad=1.0f/m_numUVertices;
	cpad=1.0f/u_num - 0.000001f;

 	memset(uv,0,sizeof(float)*m_degree);
  	for (i=0;i<m_numUVertices;i++)
 		uv[i+m_degree]=uvad*i;
	for (j=0;j<=m_degree;j++)
		uv[i+m_degree+j]=1.0;
 
	for (j=0;j<=u_num;j++)
	{
		u=0.0;
		for (i=0;i<=u_num;i++)
		{
			xp=0, yp=0, zp=0, dp=0;
			for (k=0;k<=m_numUVertices;k++)
			{
				for (l=0;l<=m_numUVertices;l++)
				{
					temp=nip(k,m_degree,u,uv)*nip(l,m_degree,v,uv);
					xp+=temp*m_pVertices[l+k*(m_numUVertices+1)].x;
					yp+=temp*m_pVertices[l+k*(m_numUVertices+1)].y;
					zp+=temp*m_pVertices[l+k*(m_numUVertices+1)].z;
   					dp+=temp;
				}
			}
			xp/=dp;
			yp/=dp;
			zp/=dp;

			pNurbs[j+i*(u_num+1)].x=xp;
			pNurbs[j+i*(u_num+1)].y=yp;
			pNurbs[j+i*(u_num+1)].z=zp;
			u+=cpad;
		}
		v+=cpad;
 
	}

	HC_Open_Segment_By_Key(m_nurbsKey);	
		HC_Flush_Geometry(".");
	m_surfaceMeshKey = HC_KInsert_Mesh(m_tesselationLevel+1,m_tesselationLevel+1,pNurbs);
	HC_Close_Segment();

	}else
	{
	HC_Open_Segment_By_Key(m_nurbsKey);	
		HC_Flush_Geometry(".");
//m_surfaceMeshKey = HC_KInsert_Mesh(m_tesselationLevel+1,m_tesselationLevel+1,pNurbs);
//m_surfaceMeshKey = HC_KInsert_Mesh(m_tesselationLevel+1,m_tesselationLevel+1,pNurbs);


	
	float uniform_knots[] = {
		0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f};
	HC_Insert_NURBS_Surface (m_degree, m_degree,  m_numUVertices+1, m_numUVertices+1,  m_pVertices,  0,  uniform_knots, uniform_knots);

	HC_Close_Segment();

	}
}

void HNurbsSurface::CalculateCornerPoint(HPoint &p1, HPoint &p2, HPoint &res, float factor)
{
	HPoint tmp;
	tmp.Set(p2.x-p1.x,p2.y-p1.y, p2.z-p1.z);
	res.Set(p1.x + factor*tmp.x,p1.y + factor*tmp.y,p1.z + factor*tmp.z);
}


void HNurbsSurface::SubdivideSurface(HPoint *old_surface, HPoint *ssurf, int xlen, int ylen, int &new_xlen, int &new_ylen, bool xrefine)
{
	int x,y;
	int xn = 0;
	int yn = 0;
	if (xrefine)
	{
		 new_xlen = (xlen) * 2 -2;
		 new_ylen = ylen;
	}
	else
	{
		 new_xlen = xlen;
		 new_ylen = ylen * 2 - 2;
	}

	yn = 0;
	xn = 0;
	 
	if (xrefine)
	{
	for (y=0;y<ylen;y++)
	{
		xn = 0;
		for (x=0;x<xlen;x++)
		{
			if (x==0 || x == (xlen - 1))
			{
				ssurf[xn + (yn*new_xlen)].x = old_surface[x + y*xlen].x;
				ssurf[xn + (yn*new_xlen)].y = old_surface[x + y*xlen].y;
				ssurf[xn + (yn*new_xlen)].z = old_surface[x + y*xlen].z;
				xn++;
			}				
			else
			{
				HPoint p;
				CalculateCornerPoint(old_surface[x - 1 + y*xlen], old_surface[x + y*xlen], p, 0.75);			
				ssurf[xn + (yn*new_xlen)] = p;
				xn++;
				CalculateCornerPoint(old_surface[x + y*xlen], old_surface[x + 1 +y*xlen], p, 0.25);			
				ssurf[xn + (yn*new_xlen)] = p;
				xn++;
			}
		}
		yn++;
	}
	}
	else
	{
		for (x=0;x<xlen;x++)
	{
		yn = 0;
		for (y=0;y<ylen;y++)
		{
			if (y==0 || y == (ylen - 1))
			{
				ssurf[xn + (yn*new_xlen)].x = old_surface[x + y*xlen].x;
				ssurf[xn + (yn*new_xlen)].y = old_surface[x + y*xlen].y;
				ssurf[xn + (yn*new_xlen)].z = old_surface[x + y*xlen].z;
				yn++;
			}				
			else
			{
				HPoint p;
				CalculateCornerPoint(old_surface[x  + (y-1)*xlen], old_surface[x + y*xlen], p, 0.75);			
				ssurf[xn + (yn*new_xlen)] = p;
				yn++;
				CalculateCornerPoint(old_surface[x + y*xlen], old_surface[x  +(y+1)*xlen], p, 0.25);			
				ssurf[xn + (yn*new_xlen)] = p;
				yn++;
			}
		}
		xn++;
	}

	}

}

	

void HNurbsSurface::InsertChakinSubdivionSurface(HPoint *pNurbs)
{
	UNREFERENCED(pNurbs);

	int xlen = 0, ylen = 0;

	HPoint ssurf[12500];
	HPoint ssurf2[12500];
 
	bool b1 = true;
	bool b2 = false;
	int i;
	for (i=0;i<4;i++)
	{
		if (i==0)
			SubdivideSurface(m_pVertices,ssurf, m_numUVertices+1,m_numUVertices+1, xlen, ylen,b1);
		else
			SubdivideSurface(ssurf2,ssurf, xlen,ylen, xlen, ylen,b1);
		
		SubdivideSurface(ssurf,ssurf2, xlen,ylen,xlen, ylen,b2);
		if (b1)
		{
			b1=false;
			b2=true;
		}
		else
		{
			b1=true;
			b2=false;
		}


	}
  
	HC_Open_Segment_By_Key(m_nurbsKey);	
		HC_Flush_Geometry(".");
		m_surfaceMeshKey = HC_KInsert_Mesh(ylen,xlen,ssurf2);
	HC_Close_Segment();

}


void HNurbsSurface::GetVerticesFromHOOPS()
{
	int x,y;

	H_SAFE_DELETE_ARRAY(m_pVertices);

	HC_Show_Mesh_Size(m_verticesKey, &x, &y);
	m_pVertices = new HPoint[(x + 1) * (y + 1)];
	
	HC_Show_Mesh(m_verticesKey, &x, &y, m_pVertices);
}

void HNurbsSurface::GetParametersFromHOOPS()
{
  	HC_Open_Segment_By_Key(m_nurbsKey);
	{
		HC_Open_Segment("settings");
		{
			HUserOptions text;
 			text.Show_One_Net_User_Option("nurbs_settings");
 			HCLOCALE (sscanf(text.m_data,"%d %f %d %d %d",&m_numUVertices, &m_vertexDistance, &m_tesselationLevel, &m_degree, (int*)&m_surfaceType));
			m_numVVertices = m_numUVertices;
		}
		HC_Close_Segment();
	}
	HC_Close_Segment();
}

void HNurbsSurface::Recalculate()
{
	if (m_surfaceType == NurbsType || m_surfaceType == InternalNurbsType)
		InsertNurbsMesh(m_pNurbs);
	else
		InsertChakinSubdivionSurface(m_pNurbs);

}


void HNurbsSurface::Init(HC_KEY key)
{
	m_nurbsKey = key;
	char type[MVO_BUFFER_SIZE];
	HC_Open_Segment_By_Key(m_nurbsKey);
	{
		HC_Begin_Contents_Search(".","mesh");
		HC_Find_Contents(type, &m_surfaceMeshKey);
		HC_End_Contents_Search();		
		HC_Open_Segment("settings");
		{
			HC_Begin_Contents_Search(".","mesh");
			HC_Find_Contents(type, &m_verticesKey);
			HC_End_Contents_Search();
		}
		HC_Close_Segment();
	}
	HC_Close_Segment();
	
	
	GetParametersFromHOOPS();
	
	m_pVertices			= new HPoint[(m_numUVertices + 1) * (m_numVVertices + 1)];
	m_pNurbs			= new HPoint[(m_tesselationLevel + 2) * (m_tesselationLevel + 2)];
	
	GetVerticesFromHOOPS();
}



HC_KEY HNurbsSurface::Init(HPoint * target, HPoint * axis, int num_u_vertices, int num_v_vertices, int degree, int tesselation_level, float distance, HSurfaceType stype)
{ 
	m_numUVertices		= num_u_vertices;
	m_numVVertices		= num_v_vertices;
 	m_degree			= degree;
	m_tesselationLevel	= tesselation_level;
	m_vertexDistance	= distance;
	m_pVertices			= new HPoint[(m_numUVertices + 1) * (m_numVVertices + 1)];
	m_pNurbs			= new HPoint[(m_tesselationLevel + 2) * (m_tesselationLevel + 2)];
	m_surfaceType		= stype;


	CreateDefaultVertices(target, axis);

	m_nurbsKey = HC_KOpen_Segment("");
	{
		HC_Set_User_Options("nurbs_surface = true");
		HC_Set_Selectability("everything = off");
		HC_Open_Segment("settings");
		{
			HC_Set_Visibility("everything = off");

 			HC_Set_Marker_Size(0.3);				
			HC_Set_Visibility("everything = off");
	 		HC_Set_Color("geometry=white");
			HC_Set_Selectability("markers=on");

		}
		HC_Close_Segment();
	}
	HC_Close_Segment();

	UpdateParameters();
	UpdateVertices();

	Recalculate();
  
// 	if (pView->GetSharedKey())
//		pView->GetSharedKey()->AssociateKey(NurbsKey);			
//	if (pView->GetSharedKey())
//	pView->GetSharedKey()->AssociateKeysInSegment(".");
 	return m_nurbsKey;
}






