// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HUtilityBoolean.cpp : implementation of the HUtilityBoolean class
//


#include "HBaseView.h"
#include "HUtilityBoolean.h"
#include "HTools.h"
#include "hc.h"

#include <math.h>
#include <stdlib.h>

SilhouetteCache * HUtilityBoolean::silCache = 0;

#define EPSILON3 0.000f


void HUtilityBoolean::DProjectShell(HC_KEY shellkey, int plen, HDPoint *points, int flen, int *faces)
{
    float matrix[16];
	double dmatrix[16];
    char path[1024];
    HC_KEY owner;
	
    owner = HC_KShow_Owner_By_Key(shellkey);
    HC_Show_Segment( owner, path);
    HC_Open_Segment_By_Key(owner);
    
	HC_Show_Net_Modelling_Matrix(matrix);
    HC_Close_Segment();
	
	for( int i = 0; i < 16; i++)
		dmatrix[i] = (double)matrix[i];
	
    HC_DCompute_Transformed_Points(plen, points, dmatrix, points);
	
}

HC_KEY HUtilityBoolean::ConvertShellToDouble( HC_KEY targetshell, bool deleteOriginal)
{
    int plen, flen;
    HPoint *points;
	HDPoint *dpoints;
    int *faces;
	HC_KEY retkey;
	
	ShowShell(targetshell, plen, points, flen, faces);
	dpoints = new HDPoint[ plen];
	for( int i = 0; i < plen; i++)
		dpoints[i].Set( (double)points[i].x, (double)points[i].y, (double)points[i].z);
	
    HC_Open_Segment_By_Key(HC_KShow_Owner_By_Key(targetshell));
	retkey = HC_DKInsert_Shell( plen, dpoints, flen, faces);
    HC_Close_Segment();	
	
	if( deleteOriginal)
		HC_Delete_By_Key( targetshell);
	
	delete [] points;
	delete [] faces;
	delete [] dpoints;
	
	return retkey;
}


void HUtilityBoolean::DUnProjectShell(HC_KEY shellkey)
{
	float matrix[16];
	double dmatrix[16];
	
	int pcount, fcount;
	HC_Show_Shell_Size(shellkey, &pcount, &fcount);
	HDPoint *points = new HDPoint[pcount];
	HC_DShow_Shell(shellkey, &pcount, points, &fcount, 0);
	
	HC_Open_Segment_By_Key(HC_KShow_Owner_By_Key(shellkey));
	HC_Show_Net_Modelling_Matrix(matrix);
	HC_Compute_Matrix_Inverse(matrix, matrix);
	HC_Close_Segment();
	
	for( int i = 0; i < 16; i++)
		dmatrix[i] = (double)matrix[i];
	
	HC_DCompute_Transformed_Points(pcount, points, dmatrix, points);
	HC_DEdit_Shell_Points(shellkey,0,pcount,pcount, points);
	delete [] points;
	
}


HC_KEY HUtilityBoolean::InsertRectangle (
										 const char	*	seg,
										 float       x0,
										 float       y0,
										 float       x1,
										 float       y1,
										 float       z,
										 bool	       fill, bool inverse) {
	HPoint       pts[5];
	HC_KEY	key;
	
	pts[0].x = x0;   pts[0].y = y0;   pts[0].z = z;
	pts[1].x = x0;   pts[1].y = y1;   pts[1].z = z;
	pts[2].x = x1;   pts[2].y = y1;   pts[2].z = z;
	pts[3].x = x1;   pts[3].y = y0;   pts[3].z = z;
	pts[4].x = x0;   pts[4].y = y0;   pts[4].z = z;
	
	if (fill)
	{
		if (!inverse)
		{       
			int flist[] = {4,0,1,2,3};
			
			key = HC_QKInsert_Shell(seg,4,pts,5, flist);
		}
		else
		{
			int flist[] = {4,3,2,1,0};
			
			key = HC_QKInsert_Shell(seg,4,pts,5, flist);
			
		}
	}
    else
		key = HC_QKInsert_Polyline (seg, 5, pts);
	return key;
}



HC_KEY HUtilityBoolean::InsertRectangle2 (
										  const char	*	seg,
										  float       z0,
										  float       y0,
										  float       z1,
										  float       y1,
										  float       x,
										  bool	       fill, bool inverse) {
	HPoint       pts[5];
	HC_KEY	key;
	
	pts[0].x = x;   pts[0].y = y0;   pts[0].z = z0;
	pts[1].x = x;   pts[1].y = y1;   pts[1].z = z0;
	pts[2].x = x;   pts[2].y = y1;   pts[2].z = z1;
	pts[3].x = x;   pts[3].y = y0;   pts[3].z = z1;
	pts[4].x = x;   pts[4].y = y0;   pts[4].z = z0;
	
	if (fill)
	{
		if (!inverse)
		{       
			int flist[] = {4,3,2,1,0};
			
			key = HC_QKInsert_Shell(seg,4,pts,5, flist);
		}
		else
		{
			int flist[] = {4,0,1,2,3};
			
			key = HC_QKInsert_Shell(seg,4,pts,5, flist);
			
		}
	}
    else
		key = HC_QKInsert_Polyline (seg, 5, pts);
	return key;
}



HC_KEY HUtilityBoolean::InsertRectangle3 (
										  const char	*	seg,
										  float       x0,
										  float       z0,
										  float       x1,
										  float       z1,
										  float       y,
										  bool	       fill, bool inverse) {
	HPoint       pts[5];
	HC_KEY	key;
	
	pts[0].x = x0;   pts[0].y = y;   pts[0].z = z0;
	pts[1].x = x0;   pts[1].y =y;   pts[1].z = z1;
	pts[2].x = x1;   pts[2].y = y;   pts[2].z = z1;
	pts[3].x = x1;   pts[3].y = y;   pts[3].z = z0;
	pts[4].x = x0;   pts[4].y = y;   pts[4].z = z0;
	
	if (fill)
	{
		if (!inverse)
		{       
			int flist[] = {4,3,2,1,0};
			
			key = HC_QKInsert_Shell(seg,4,pts,5, flist);
		}
		else
		{
			int flist[] = {4,0,1,2,3};
			
			key = HC_QKInsert_Shell(seg,4,pts,5, flist);
			
		}
	}
    else
		key = HC_QKInsert_Polyline (seg, 5, pts);
	return key;
}




void HUtilityBoolean::ShowShell(HC_KEY key, int &plen, HPoint *(&points), int &flen, int *(&faces))
{
    HC_Show_Shell_Size(key, &plen, &flen);
    points = new HPoint[plen];
    faces = new int[flen];
    HC_Show_Shell(key, &plen, points, &flen, faces);
	
}

void HUtilityBoolean::ReorderFace(int *face)
{
    int n = face[0];
	
    for (int i=1;i<=(n/2);i++)
    {
		int temp = face[i];
		face[i] = face[n-i + 1];
		face[n-i + 1] = temp;
		
    }
}

double HUtilityBoolean::area3D_Polygon(HPoint* points, int *face)
{
    int n = face[0];
    HPoint N;
    HPoint V[256];
    for (int i=1;i<=n;i++)
		V[i-1] = points[face[i]];
    V[n] = V[0];
    V[n+1] = V[1];
	
    double A[3][3];
	
    A[0][0] = V[0].x;
    A[0][1] = V[1].x;
    A[0][2] = V[2].x;
	
    A[1][0] = V[0].y;
    A[1][1] = V[1].y;
    A[1][2] = V[2].y;
	
	
    A[2][0] = V[0].z;
    A[2][1] = V[1].z;
    A[2][2] = V[2].z;
	
	return( A[0][0]*(A[1][1]*A[2][2] - A[1][2]*A[2][1]) +
		A[0][1]*(A[1][2]*A[2][0] - A[1][0]*A[2][2]) +
		A[0][2]*(A[1][0]*A[2][1] - A[1][1]*A[2][0]) );
	
	
}



HC_KEY HUtilityBoolean::MakeWindingConsistent(HC_KEY shellkey)
{
	int plen, flen;
    HPoint *points;
    int *faces;
    ShowShell(shellkey, plen, points, flen, faces);
	
    int i=0;
    int fnum = 0;
    bool facing[4096];
    int left = 0, right = 0;
    while (1)
    {
		ReorderFace(&faces[i]);
		
		if (area3D_Polygon(points, &faces[i])>0 || fnum!=7)
		{
			facing[fnum++] = true;
			left++;
		}
		else
		{
			facing[fnum++] = false;
			right++;	
		}
		
		i+=(faces[i] + 1);
		if (i>=flen)
			break;
    }
	
    HC_Open_Segment_By_Key(HC_KShow_Owner_By_Key(shellkey));
    HC_Delete_By_Key(shellkey);
    HC_KEY retkey = HC_KInsert_Shell(plen, points, flen, faces);
    HC_Close_Segment();
	return retkey;
	
}


void HUtilityBoolean::CutBlock(HBaseView *view, HC_KEY targetshell, int side, int slicenum)
{
	HC_KEY targetsegment;
    HPoint min, max;
	targetsegment = HC_KShow_Owner_By_Key(targetshell);
	char pathname[4096];
    HC_Show_Segment(targetsegment, pathname);
    HC_Open_Segment_By_Key(targetsegment);
    HC_Compute_Circumcuboid(".", &min, &max);
	
    min.x-=0.1f;
    min.y-=0.1f;
    min.z-=0.1f;
    max.x+=0.1f;
    max.y+=0.1f;
    max.z+=0.1f;
    // need to take care of any modelling matrices on the segment
    HC_KEY keys[MVO_BUFFER_SIZE];
    int kcount = HUtility::CalculateKeyPath(targetsegment, view, keys, MVO_BUFFER_SIZE);
	
    HC_Compute_Coordinates_By_Path(kcount,keys, "object", &min, "world", &min);
    HC_Compute_Coordinates_By_Path(kcount,keys, "object", &max, "world", &max);
	
	
    HC_Close_Segment();
    HPoint delta(max.x - min.x, max.y - min.y, max.z - min.z);
	
    HC_Open_Segment_By_Key(targetsegment);
	for (int i=0;i<slicenum;i++)
    {
		HC_KEY toolkey, toolkey2 = -1, second = -1;
		HC_Open_Segment("");
		if (side == 0)
			toolkey =  InsertRectangle2(".",min.z,min.y,max.z,max.y,min.x + (delta.x/(float)(slicenum + 1)) * i,true, false);
		else if (side == 1)
			toolkey =  InsertRectangle(".",min.x,min.y,max.x,max.y,min.z + (delta.z/(float)(slicenum + 1)) * i,true, false);
		else if (side == 2)
			toolkey =  InsertRectangle3(".",min.x,min.z,max.x,max.z,min.y + (delta.y/(float)(slicenum + 1)) * i,true, false);
		HC_KEY first  = MakeBoolean(targetshell, toolkey, "boolean type = intersect");	
		if (first != -1)
		{	
			if (side == 0)
				toolkey2 =  InsertRectangle2(".",min.z,min.y,max.z,max.y,min.x + (delta.x/(float)(slicenum + 1)) * (i + 1),true, true);
			else if (side == 1)
				toolkey2 =  InsertRectangle(".",min.x,min.y,max.x,max.y,min.z + (delta.z/(float)(slicenum + 1)) * (i + 1),true, true);
			else if (side == 2)
				toolkey2 =  InsertRectangle3(".",min.x,min.z,max.x,max.z,min.y + (delta.y/(float)(slicenum + 1)) * (i + 1),true, true);
			
			second  = MakeBoolean(first, toolkey2, "boolean type = intersect");	
		}
		
		// toolkey =  InsertRectangle(".",min.x,min.y,max.x,max.y,min.z + (delta.z/10.0f) * i,true, false);
		if (first != -1)
			HC_Delete_By_Key(first);
		HC_Delete_By_Key(toolkey);
		if (toolkey2 != -1)
			HC_Delete_By_Key(toolkey2);
		
		
		HC_Close_Segment();
		
    }
	
	
	HC_Close_Segment();
	HC_Delete_By_Key(targetshell);
}


void HUtilityBoolean::ProjectShell(HC_KEY shellkey, int plen, HPoint *points, int flen, int *faces)
{
	float matrix[16];
    HC_Open_Segment_By_Key(HC_KShow_Owner_By_Key(shellkey));
    HC_Show_Net_Modelling_Matrix(matrix);
	HC_Close_Segment();
    HC_Compute_Transformed_Points(plen, points, matrix, points);
	
}

HC_KEY HUtilityBoolean::DMakeBoolean(HC_KEY box1, HC_KEY box2, char *whattype, bool doTransform) 
{     
	
	
    int j;
    HDPoint *plist1;
    HPoint *NormalList1;
    int *flist1;
    int flen1, plen1;
    HC_Show_Shell_Size(box1, &plen1, &flen1);    
    plist1 = (HDPoint *)malloc(plen1 * sizeof(HDPoint));
    NormalList1 = (HPoint *)malloc(plen1 * sizeof(HPoint));
    flist1 = (int *)malloc(flen1 *sizeof(int));
    HC_DShow_Shell(box1, &plen1, plist1, &flen1, flist1);
    DProjectShell(box1, plen1, plist1, flen1, flist1);
    
    HC_Open_Geometry(box1);
    for (j=0;j<plen1;j++)
    {
		HC_Open_Vertex(j);
		HC_Show_Net_Normal(&NormalList1[j].x,&NormalList1[j].y,&NormalList1[j].z);
		HC_Close_Vertex();
	}
    HC_Close_Geometry();
	
    HPoint *colorlist1;
    HPoint *colorlist2;
    unsigned short *vislist1;
    unsigned short *vislist2;
	
	
    GetFaceColors(box1, &colorlist1);
    GetFaceColors(box2, &colorlist2);
    GetFaceFlags(box1, &vislist1);
    GetFaceFlags(box2, &vislist2);
	
	
    HDPoint *plist2;
    HPoint *NormalList2;
    int *flist2;
    int flen2, plen2;
    HC_Show_Shell_Size(box2, &plen2, &flen2);    
    plist2 = (HDPoint *)malloc(plen2 * sizeof(HDPoint));
    NormalList2 = (HPoint *)malloc(plen2 * sizeof(HPoint));
    flist2 = (int *)malloc(flen2 *sizeof(int));
    HC_DShow_Shell(box2, &plen2, plist2, &flen2, flist2);
    DProjectShell(box2, plen2, plist2, flen2, flist2);
    HC_Open_Geometry(box2);
    for (j=0;j<plen2;j++)
    {
		HC_Open_Vertex(j);
		HC_Show_Net_Normal(&NormalList2[j].x,&NormalList2[j].y,&NormalList2[j].z);
		HC_Close_Vertex();
    }
    HC_Close_Geometry();
	
	HC_KEY  resultkey =     HC_DKCompute_Boolean_Shell(plen1, plist1, 0, flen1, flist1, 
		plen2, plist2, 0, flen2, flist2,whattype, 0, 0,vislist1,vislist2);
	//    resultkey =  FixShell(resultkey);
    
    if (doTransform)
		DUnProjectShell(resultkey);
	
	free(plist1);
	free(NormalList1);
	free(flist1);
	free(plist2);
	free(NormalList2);
	free(flist2);
	
	return resultkey;
}

HC_KEY HUtilityBoolean::MakeBoolean(HC_KEY s1, HC_KEY s2, char *whattype, bool doTransform) 
{     
	
	
    int j;
    HPoint *plist1;
    HPoint *NormalList1;
    int *flist1;
    int flen1, plen1;
    HC_Show_Shell_Size(s1, &plen1, &flen1);    
    plist1 = (HPoint *)malloc(plen1 * sizeof(HPoint));
    NormalList1 = (HPoint *)malloc(plen1 * sizeof(HPoint));
    flist1 = (int *)malloc(flen1 *sizeof(int));
    HC_Show_Shell(s1, &plen1, plist1, &flen1, flist1);
    ProjectShell(s1, plen1, plist1, flen1, flist1);
    
    HC_Open_Geometry(s1);
    for (j=0;j<plen1;j++)
    {
		HC_Open_Vertex(j);
		HC_Show_Net_Normal(&NormalList1[j].x,&NormalList1[j].y,&NormalList1[j].z);
		HC_Close_Vertex();
    }
    HC_Close_Geometry();
	
    HPoint *colorlist1;
    HPoint *colorlist2;
    unsigned short *vislist1;
    unsigned short *vislist2;
	
	
    GetFaceColors(s1, &colorlist1);
    GetFaceColors(s2, &colorlist2);
    GetFaceFlags(s1, &vislist1);
    GetFaceFlags(s2, &vislist2);
	
	
    HPoint *plist2;
    HPoint *NormalList2;
    int *flist2;
    int flen2, plen2;
    HC_Show_Shell_Size(s2, &plen2, &flen2);    
    plist2 = (HPoint *)malloc(plen2 * sizeof(HPoint));
    NormalList2 = (HPoint *)malloc(plen2 * sizeof(HPoint));
    flist2 = (int *)malloc(flen2 *sizeof(int));
    HC_Show_Shell(s2, &plen2, plist2, &flen2, flist2);
    ProjectShell(s2, plen2, plist2, flen2, flist2);
    HC_Open_Geometry(s2);
    for (j=0;j<plen2;j++)
    {
		HC_Open_Vertex(j);
		HC_Show_Net_Normal(&NormalList2[j].x,&NormalList2[j].y,&NormalList2[j].z);
		HC_Close_Vertex();
    }
    HC_Close_Geometry();
	
	HC_KEY  resultkey =     HC_DKCompute_Boolean_Shell(plen1, plist1, 0, flen1, flist1, 
		plen2, plist2, 0, flen2, flist2,whattype, 0, 0,vislist1,vislist2);
	//    resultkey =  FixShell(resultkey);
    
    if (doTransform)
		UnProjectShell(resultkey);
	
	free(plist1);
	free(NormalList1);
	free(flist1);
	free(plist2);
	free(NormalList2);
	free(flist2);
	
	return resultkey;
	
	
}

void HUtilityBoolean::UnProjectShell(HC_KEY shellkey)
{
	float matrix[16];
	int pcount, fcount;
	HC_Show_Shell_Size(shellkey, &pcount, &fcount);
	HPoint *points = new HPoint[pcount];
	HC_Show_Shell(shellkey, &pcount, points, &fcount, 0);
    HC_Open_Segment_By_Key(HC_KShow_Owner_By_Key(shellkey));
    HC_Show_Net_Modelling_Matrix(matrix);
    HC_Compute_Matrix_Inverse(matrix, matrix);
	HC_Close_Segment();
    HC_Compute_Transformed_Points(pcount, points, matrix, points);
    HC_Edit_Shell_Points(shellkey,0,pcount,pcount, points);
	delete [] points;
	
}



void HUtilityBoolean::SplitTarget(HBaseView *view, HC_KEY targetshell)
{
	
	HPoint min, max;
    if (targetshell != -1)
    {
		
		
		targetshell = MakeWindingConsistent(targetshell);
		
		
		CutBlock(view, targetshell, 0,10);
		
    }
    HC_KEY shelllist[10000];
    int count = 0;
    HC_Open_Segment_By_Key(view->GetModelKey());
    HC_Open_Segment("target");
    HC_Begin_Contents_Search("...", "shells");
    while (HUtility::FindContents(targetshell))
    {
		shelllist[count++] = targetshell;
    }
	HC_End_Contents_Search();
	
	HC_Close_Segment();
	HC_Close_Segment();
	for (int i=0;i<count;i++)
		CutBlock(view,shelllist[i], 2,10);
	count = 0;
    HC_Open_Segment_By_Key(view->GetModelKey());
    HC_Open_Segment("target");
    HC_Begin_Contents_Search("...", "shells");
    while (HUtility::FindContents(targetshell))
    {
		shelllist[count++] = targetshell;
    }
	HC_End_Contents_Search();
	
	HC_Close_Segment();
	HC_Close_Segment();
	for (int i=0;i<count;i++)
		CutBlock(view,shelllist[i], 1,10);
	
}

void HUtilityBoolean::GetFaceColors(HC_KEY key, HPoint **box1)
{
    HC_Show_Geometry_Pointer(key, "face colors", box1);
    
}

void HUtilityBoolean::GetFaceFlags(HC_KEY key, unsigned short **box1)
{
    HC_Show_Geometry_Pointer(key, "face flags", box1);
    
}



HC_KEY HUtilityBoolean::CalculateSilhouetteInternal(int plen, HPoint *points, int flen, int *flist, HPoint start, HPoint end)
{
	
    HPoint delta, v;
    delta.Set(end.x - start.x, end.y - start.y, end.z - start.z);
    HC_Compute_Normalized_Vector(&delta, &v);
	
    // Lets see if we actually moved anywhere.  If not, make a shell from the existing tool points and call it
    // a day.   
    if( start.x == end.x &&
		start.y == end.y &&
		start.z == end.z )
    {
		HC_KEY retkey = HC_KInsert_Shell(plen, points, flen, flist);
		return retkey;
    }
	
	
    //_ASSERT( !CheckCoincident( plen, points, flen, flist));
    int i=0;
	
    // setup memory cache.  Some of these values are hard-coded for now
    // and should be changed at some point to either be variable or checks
    // put in later in the code to check for overruns.
    silCache->CheckSizesAndClear( 350, 1000, 1000, plen*4, flen*4, 1000);
    
    for (i=0;i<plen;i++)
    {
		silCache->newpoints[i] = points[i];
		silCache->newpoints[i+plen].Set(points[i].x + delta.x, points[i].y + delta.y, points[i].z + delta.z);
    }
	
    i = 0;
    int numvalidedges = 0;
    int numfaces = 0;
    int facenum2 = 0;    
	
    while (1)
    {
		int facenum = flist[i];
		i++;	
		HPoint p[3];
		HPoint f1;
		p[0] = points[flist[i]];
		p[1] = points[flist[i+1]];
		p[2] = points[flist[i+2]];
        HC_Compute_Polygon_Normal(3,p,"right",&f1);
		silCache->facelist[numfaces].angle = ComputeAngleBetweenVector(&f1,&v);
		silCache->facelist[numfaces].position = i - 1;
        silCache->newfaces[facenum2++] = flist[i-1];
		for (int j=0;j<facenum;j++)
		{
			
			if (silCache->facelist[numfaces].angle > 90.0f)
			{
				silCache->newfaces[facenum2++] = flist[i+j];
			}
			else
			{
				silCache->newfaces[facenum2++] = flist[i+j]+plen;
				
			}
			
			
			
			int a,b;
			a = flist[i+j];
			if (j==facenum-1)	    	    
				b = flist[i];
			else
				b = flist[i+j+1];
			
			int sava = a;
			int savb = b;
			if (a>b)
			{
				int temp = a;
				a = b;
				b = temp;
			}
			
			
			if (silCache->facelist[numfaces].angle > 90.0f + EPSILON3)
				silCache->edgematrix[a][b].SetPoints(savb,sava);
			
			if (silCache->edgematrix[a][b].AddFace(numfaces))
			{
				silCache->validedges[numvalidedges++] = &silCache->edgematrix[a][b];
				//		edgematrix[a][b].SetPoints(a,b);
			}
			
		}
		
		if (numfaces > 499)
			numfaces = numfaces;
		
		i+=facenum;
		numfaces++;
		if (i>=flen)
			break;
    }
	
	
    for (i=0;i<numvalidedges;i++)
    {
		HPoint f1, f2;
		EdgeObject *edge = silCache->validedges[i];
		HPoint p[3];	        
		float angle_f1 = silCache->facelist[edge->faces[0]].angle;
		float angle_f2 = silCache->facelist[edge->faces[1]].angle;
		if ((angle_f2>(90.0f + EPSILON3) && angle_f1<=(90.0f - EPSILON3)) || (angle_f2<=(90.0f-EPSILON3) && angle_f1>(90.0f + EPSILON3)))
		{
			//	    if (!pointdone[edge->p[0]])
			//{
			//		HPoint temp1 = points[edge->p[0]];
			//		AdjustPointPosition(temp1,v);
			//		HPoint temp2 = points[edge->p[1]];
			//		AdjustPointPosition(temp2,v);
			//		pointdone[edge->p[0]] = true;
			
			//	    }
			
			
			silCache->newfaces[facenum2++] = 4;
			silCache->newfaces[facenum2++] = edge->p[0];
			silCache->newfaces[facenum2++] = edge->p[1];
			silCache->newfaces[facenum2++] = edge->p[1] + plen;
			silCache->newfaces[facenum2++] = edge->p[0] + plen;
			numfaces++;
			//	    HC_Insert_Line(points[edge->p[0]].x,points[edge->p[0]].y,points[edge->p[0]].z,
			//		points[edge->p[1]].x,points[edge->p[1]].y,points[edge->p[1]].z);
			
			//	    HC_Insert_Line(temp1.x, temp1.y, temp1.z, temp2.x, temp2.y, temp2.z);	  
			
		}
		
		
    }
	
    for (i=0;i<numfaces;i++)
		silCache->colors[i].Set(0.8f, 0.8f, 0.8f);
	
	
	// Deletion is performed in reversed order.
    // Pay special attention to the delete[] operator
    // which must be used to delete arrays (instead
    // of the "simple" delete)
    //
	
	//    _ASSERT( !CheckCoincident( plen*2, silCache->newpoints, facenum2, silCache->newfaces));
	
	
	
    HC_KEY retkey = HC_KInsert_Shell(plen*2, silCache->newpoints, facenum2, silCache->newfaces);
    HC_MSet_Face_Colors_By_Value(retkey, "faces", 0, "rgb", numfaces, silCache->colors);
	
	
    return retkey;
}



HC_KEY HUtilityBoolean::CalculateSilhouette(HC_KEY targetshell, HPoint start, HPoint end)
{
	
	
	HPoint min, max;
	
    int plen, flen;
    HPoint *points;
    int *faces;
	ShowShell(targetshell, plen, points, flen, faces);
    HPoint v1,v2;
    v1.Set(0,0,0);
    v2.Set(3,3,3);
    HC_Open_Segment_By_Key(HC_KShow_Owner_By_Key(targetshell));
    HC_Open_Segment("sil");
    HC_Flush_Contents(".", "everything");
    HC_KEY retkey = CalculateSilhouetteInternal(plen, points, flen, faces, start, end);
    HC_Close_Segment();
    HC_Close_Segment();
	
    delete [] points;
    delete [] faces;
    return retkey;
	
}



HC_KEY HUtilityBoolean::DCalculateSilhouette( HC_KEY targetshell, HPoint start, HPoint end)
{
	HC_KEY silKey = CalculateSilhouette( targetshell, start, end);
	silKey = ConvertShellToDouble( silKey, true);
	
	return silKey;
}


float HUtilityBoolean::ComputeAngleBetweenVector(HPoint *p1, HPoint *p2)
{
	HPoint p3;
    HC_Compute_Normalized_Vector(p1,p1);
    HC_Compute_Normalized_Vector(p2,p2);
    HC_Compute_Cross_Product(p1,p2,&p3);
	
    float dprod = (float)HC_Compute_Dot_Product(p1,p2);
    float l=(float)HC_Compute_Vector_Length(&p3);
	float   ang = (float)atan2(l,dprod);
    ang*=(180.0f/(float)3.1415927f);
	return ang;
	
}

void HUtilityBoolean::enumerate_points (HC_KEY key1, HBaseView *view,   const HPoint * p1,
										const HPoint * p2, HPoint &min, HPoint &max) {
	
    float x,y,z;
    HPoint out[8];
	out[0].x = p1->x;	 out[0].y = p1->y;	  out[0].z = p1->z;
	out[1].x = p1->x;	 out[1].y = p1->y;	  out[1].z = p2->z;
	out[2].x = p1->x;	 out[2].y = p2->y;	  out[2].z = p1->z;
	out[3].x = p1->x;	 out[3].y = p2->y;	  out[3].z = p2->z;
	out[4].x = p2->x;	 out[4].y = p1->y;	  out[4].z = p1->z;
	out[5].x = p2->x;	 out[5].y = p1->y;	  out[5].z = p2->z;
	out[6].x = p2->x;	 out[6].y = p2->y;	  out[6].z = p1->z;
	out[7].x = p2->x;	 out[7].y = p2->y;	  out[7].z = p2->z;
	
	HC_KEY keys[MVO_BUFFER_SIZE];
	int kcount;
	float matrix[16];
	kcount = HUtility::CalculateKeyPath(key1, view, keys, MVO_BUFFER_SIZE);
	HC_Compute_Transform_By_Path(kcount, keys, "object", "world", matrix);
	HC_Compute_Transformed_Points(8, out, matrix, out);
	
	
	x = out[0].x;
	y =  out[0].y;
	z =  out[0].z;
	min.x = max.x =  x;
	min.y = max.y =  y;
	min.z = max.z =  z;
	for( int i = 1 ; i < 8 ; i++ ) {
		x = out[i].x;
		y = out[i].y;
		z = out[i].z;
		
		if( x < min.x )
			min.x = x;
		if( y < min.y )
			min.y = y;
		if( z < min.z )
			min.z = z;
		if( x > max.x )
			max.x = x;
		if( y > max.y )
			max.y = y;
		if( z > max.z )
			max.z = z;
	}
	
}




bool HUtilityBoolean::TestCollision(HBaseView *view, HC_KEY key1, HC_KEY key2, bool first, bool doTransform1, bool doTransform2)
{
    static HPoint min1, max1;
    key1 = HC_KShow_Owner_By_Key(key1);
    key2 = HC_KShow_Owner_By_Key(key2);
    HPoint min2, max2;
	
    if (first)
    {    
		HC_Open_Segment_By_Key(key1);   
		HPoint mint, maxt;
		
		HC_Compute_Circumcuboid(".", &mint, &maxt);
		enumerate_points(key1, view, &mint, &maxt, min1, max1);
		//    kcount = HUtility::CalculateKeyPath(key1, view, keys, MVO_BUFFER_SIZE);
		//  HC_Compute_Coordinates_By_Path(kcount,keys, "object", &min1, "world", &min1);
		//HC_Compute_Coordinates_By_Path(kcount,keys, "object", &max1, "world", &max1);
		mint = min1;
		maxt = max1;
		HC_Close_Segment();
    }
	
	HC_Open_Segment_By_Key(key2);
	HPoint mint, maxt;
	if (doTransform2)
	{
		HC_Compute_Circumcuboid(".", &mint, &maxt);
		enumerate_points(key2, view, &mint, &maxt, min2, max2);
	}
	else
		HC_Compute_Circumcuboid(".", &min2, &max2);
	
	//    kcount = HUtility::CalculateKeyPath(key2, view, keys, MVO_BUFFER_SIZE);
	//    HC_Compute_Coordinates_By_Path(kcount,keys, "object", &min2, "world", &min2);
	//  HC_Compute_Coordinates_By_Path(kcount,keys, "object", &max2, "world", &max2);
	HC_Close_Segment();
	
	if (!(max2.x < min1.x || min2.x > max1.x) && !(max2.y < min1.y || min2.y > max1.y) && 
		!(max2.z < min1.z || min2.z > max1.z))
		return true;
	else
		return false;
}





int HUtilityBoolean::ObjectMovedEvent(HBaseView *view, HC_KEY key, HPoint *p, HPoint posbeforemove) 
{
    static bool first = true;
    bool NoExtrusion = false;
    bool doTransform1 = false, doTransform2 = false;
	static bool doingDouble = true;
	
	
	
    // do a quick check... we may only want to do booleans if we have made a "reasonable" move, otherwise
    // we create billions of sliver faces.
    if (HUtility::IsModelKey(view->GetModelKey(), key))
	{
		
		first = false;
		
		float dx,dy,dz,dsqr;
		dx = p->x - posbeforemove.x;
		dy = p->y - posbeforemove.y;
		dz = p->z - posbeforemove.z;
		dsqr = dx*dx + dy*dy + dz*dz;
		
		
		
		//     return HLISTENER_PASS_EVENT;   
		// if (m_pHView->GetModel()->GetBhvBehaviorManager()->IsPlaying())
		{    
			
			HC_Open_Segment_By_Key(view->GetModelKey());
			HC_KEY testkey = HC_KOpen_Segment("target");
			HC_Close_Segment();
			if (testkey == key)
			{
				key = HC_KOpen_Segment("tool");
				HC_Close_Segment();
				NoExtrusion = true;
				doTransform2 = true;
			}
			HC_Close_Segment();
			
			HC_KEY toolkey;
			HC_Open_Segment_By_Key(key);
			HC_Begin_Contents_Search(".", "shells");
			HUtility::FindContents(toolkey);
			HC_End_Contents_Search();
			HC_Close_Segment();
			
#ifdef DOUBLE_PREC
			if (m_bUseExtrusion && !NoExtrusion)
			{
				if( !doingDouble)
					toolkey = CalculateSilhouette(toolkey, p, posbeforemove);
				else
					toolkey = DCalculateSilhouette( toolkey, p, posbeforemove);
			}
#endif
			
			HC_Open_Segment_By_Key(view->GetModelKey());
			HC_Open_Segment("target");
			
			if (HC_Show_Existence("modelling matrix"))
				doTransform2 = true;
			
			HC_KEY targetkey;
			bool first = true;
			HC_Begin_Contents_Search("...", "shells");
			while (HUtility::FindContents(targetkey))
			{    
				if (TestCollision(view, toolkey, targetkey, first, doTransform1, doTransform2))
				{
					
					HC_Open_Segment_By_Key(HC_KShow_Owner_By_Key(targetkey));
					
					
					HC_KEY result = -1;
					
					if( !doingDouble)
					{
						result  = MakeBoolean(toolkey, targetkey, "boolean type = subtract", doTransform2);	
					}
					else
					{
						int isDouble;
						isDouble = HC_DShow_Existence_By_Key( toolkey);
						if( !isDouble)
							toolkey = ConvertShellToDouble( toolkey, true);
						
						isDouble = HC_DShow_Existence_By_Key( targetkey);
						if( !isDouble)
							targetkey = ConvertShellToDouble( targetkey, true);
						
						result = DMakeBoolean( toolkey, targetkey, "boolean type = subtract", doTransform2);
						
					}
					
					
					//HC_KEY result = -1;
					
					HC_Close_Segment();
					
 					//	    if (result != -1)
					HC_Delete_By_Key(targetkey);
					
					//	    CheckManifoldShell( result);
				}
				
				first = false;
			}
			HC_End_Contents_Search();
			HC_Close_Segment();
			HC_Close_Segment();
			if (!NoExtrusion)
				HC_Delete_By_Key(toolkey);
			
		}
		
		view->ForceUpdate();
	}
	
    return HLISTENER_PASS_EVENT;
}



