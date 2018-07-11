// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "HUtility.h"
#include "HIOUtilityStl.h"
#include "HBaseView.h"
#include "HIOManager.h"



HFileInputResult HIOUtilityStl::FileInputByKey(const wchar_t *FileName, HC_KEY key, HInputHandlerOptions * options) 
{
	UNREFERENCED(options);

	if (!FileName) return InputBadFileName;
	if (key == INVALID_KEY) return InputFail;

	SetStartingInput();

	HC_Open_Segment_By_Key(key);
		HFileInputResult result =  HIOUtilityStl::ReadStlFile(FileName);
		HC_Set_Rendering_Options("no lighting interpolation");
	HC_Close_Segment();

	SetFinishedInput();

	return result;
}

HFileOutputResult HIOUtilityStl::ComputeStlTriangles(
	HC_KEY					the_key,
	stl_info				*info,
	HOutputHandlerOptions	*options,
	bool					no_recurse,/* we are descending from a segment reference: don't search subsegments or references */
	float					*matrix_for_segment_reference)
{
	HFileOutputResult status = OutputOK;
	HC_KEY	key = 0;
	char	*type = new char[MVO_BUFFER_SIZE];
	char	*pathname = 0;
	int		* tristrips = 0;
	HPoint	* points = 0;
	int		oldnumvertices=0, oldtristripslen=0;
	char vis_setting[128] = "on";
	HC_KEY vis_path[3] = {the_key, info->copy_segment, info->start_segment};

	HC_Open_Segment_By_Key(the_key);
		if(!no_recurse)
		{
			HC_Begin_Contents_Search(".", "segment");

			/* look for a way down */
			while (HC_Find_Contents(type, &key))
			{
				/* into the deep */
				if (ComputeStlTriangles(key, info, options) == OutputFail)
				{
					status = OutputFail;
					goto cleanup;
				}
			}
			HC_End_Contents_Search();
		}

		//we don't care about the visibility setting in a referenced segment.
		if(!no_recurse)
			HC_PShow_One_Net_Visibility(3, vis_path, "faces", vis_setting);

		if(streq(vis_setting, "on"))
		{
			/* did we find anything? */
			HC_Begin_Contents_Search(".", "shell, sphere, polycylinder, nurbs surface, cylinder, polygon, circle, circular chord, circular wedge, ellipse, mesh, grid, reference");
				int i, numvertices, tristripslen;
				int *iend, *iptr;

				while (HC_Find_Contents(type, &key))
				{
					bool we_have_a_reference_matrix = false;
					bool reference_geometry_is_visible = true;
					float net_matrix[16];
					HC_Compute_Identity_Matrix(net_matrix);
					/* first deal with references, but don't look at references if we got to this segment via another reference */
					if(!no_recurse && streq(type, "reference"))
					{
						float reference_matrix[16];
						char key_type[1024];
						HC_KEY referenced_geometry_key = INVALID_KEY;
						/* in case we have a chain of references, we descend until we get a key to a non-reference, collecting 
						 * matrices and visibilities along the way */
						do
						{
							bool there_is_a_matrix_on_this_reference = false;
							char reference_visibility[1024] = "on";
							HC_Open_Geometry(key);{
								if(HC_Show_Existence("modelling matrix"))
								{
									there_is_a_matrix_on_this_reference = true;
									HC_Show_Modelling_Matrix(reference_matrix);
								}
								if(HC_Show_Existence("visibility"))
								{
									HC_Show_One_Visibility("faces", reference_visibility);
									if(streq(reference_visibility, "off"))
										reference_geometry_is_visible = false;
									else
										reference_geometry_is_visible = true;
								}
							}HC_Close_Geometry();
							if(there_is_a_matrix_on_this_reference)
							{
								we_have_a_reference_matrix = true;
								HC_Compute_Matrix_Product(net_matrix, reference_matrix, net_matrix);
							}

							referenced_geometry_key = HC_KShow_Reference_Geometry(key);
							HC_Show_Key_Type(referenced_geometry_key, key_type);
							key = referenced_geometry_key;
						} while (streq(key_type, "reference"));

						/* if face visibility is turned off for this reference, we've can give up on this key */
						if(!reference_geometry_is_visible)
							continue;

						/* we're at the reference target. if it's a segment, we now 
						 * want to pull in all geometry so we recurse with special options */
						if(streq(key_type, "segment"))
						{
							/* if the reference is to a segment, process the faceted geometry in there, but not in any subsegments (no recursion) */
							ComputeStlTriangles(referenced_geometry_key, info, options, true, (we_have_a_reference_matrix ? net_matrix : 0));
							continue;
						}

						/* Now that we have the underlying geometry key let's proceed as normal -- we will 
						 * apply the transforms from the references (if any) later. */
						strcpy(type, key_type);
					}

					if (streq(type, "shell") || 
						streq(type, "sphere") || 
						streq(type, "polycylinder") || 
						streq(type, "nurbs surface") || 
						streq(type, "cylinder") || 
						streq(type, "polygon") || 
						streq(type, "circle") || 
						streq(type, "circular chord") || 
						streq(type, "circular wedge") || 
						streq(type, "ellipse") || 
						streq(type, "mesh") || 
						streq(type, "grid"))
					{
						if (!streq(type, "shell"))	/* if not a shell, generate a shell from it */
							key = HC_Generate_Shell_From_Geometry(key, "");

						HC_Show_Shell_Size(key, &numvertices, &tristripslen);
						if(tristrips == 0 || tristripslen > oldtristripslen)
						{
							delete [] tristrips;
							oldtristripslen = tristripslen;	
							tristrips = new int[tristripslen];
						}
						if (points == 0 || numvertices > oldnumvertices)
						{
							delete [] points;
							oldnumvertices = numvertices;
							points = new HPoint[numvertices];
						}
						HC_Show_Shell(key, 0, points, 0, tristrips);

	 					HC_Compute_Optimized_Shell(numvertices, points, 0, tristripslen, tristrips, "fix handedness",
	 												&numvertices, points, &tristripslen, tristrips, 0, 0);

						if (!streq(type, "shell"))	/* if we generated a shell, we may delete it now */
							HC_Delete_By_Key(key);

						key = HC_KInsert_Shell(numvertices,points,tristripslen,tristrips);	/* optimized shell */
						HC_Show_Shell_By_Tristrips_Size(key, &numvertices, &tristripslen, 0);

						if (tristrips == 0 || tristripslen > oldtristripslen)
						{
							delete [] tristrips;
							oldtristripslen = tristripslen;	
							tristrips = new int[tristripslen];
						}
						if (points == 0 || numvertices > oldnumvertices)
						{
							delete [] points;
							oldnumvertices = numvertices;
							points = new HPoint[numvertices];
						}

						HC_Show_Shell_By_Tristrips(key, 0, points, 0, tristrips, 0, 0);
						HC_Delete_By_Key(key);	/* we're now done with the (temporary) optimized shell */

						//if we have a geometry reference matrix or a segment reference matrix, apply the transform now
						if(we_have_a_reference_matrix)	
							HC_Compute_Transformed_Points(numvertices, points, net_matrix, points);
						else if(matrix_for_segment_reference != 0)
							HC_Compute_Transformed_Points(numvertices, points, matrix_for_segment_reference, points);

						iend = tristrips + tristripslen;
						iptr = tristrips;
						while (iptr < iend)  /* over entire list of tristrips */
						{
							int striplen = *iptr;  /* store # vertices in tristrip */
							for (i=0; i<striplen-2; i++)  /* make n-2 triangles */
							{
								float normal[3] = {0.f, 0.f, 0.f};  /* null normal (default) */
								float * vertices;
								iptr++;

								/* if we are printing normals*/
								if (options->StlNormals()) {
									HPoint a, b;
									if (i%2 == 0) {
										a = HPoint(points[*(iptr+2)]) - HPoint(points[*(iptr+1)]);
										b = HPoint(points[*(iptr)]) - HPoint(points[*(iptr+1)]);
									}
									else {
										a = HPoint(points[*(iptr)]) - HPoint(points[*(iptr+1)]);
										b = HPoint(points[*(iptr+2)]) - HPoint(points[*(iptr+1)]);
									}
									HC_Compute_Cross_Product(&a, &b, (void *)normal);
									HC_Compute_Normalized_Vector(normal, normal);
								}

								/* output the triangles in this tristrip */
								if (!info->is_ascii) // BINARY output
								{
									/* Normal */
									if (fwrite(normal, 4, 3, info->f) != 3)
									{ 
										status = OutputFail;
										goto cleanup;
									}

									/* flip flop for tristrips */
									if (i%2 == 0)
									{
										/* A */
										vertices = (float *)&points[*iptr]; /* grab the first vertex from tristrip list */
										if (fwrite(vertices, 4, 3, info->f) != 3) /* write x y & z */
										{ 
											status = OutputFail;
											goto cleanup;
										}

										/* B */
										vertices = (float *)&points[*(iptr+1)];
										if (fwrite(vertices, 4, 3, info->f) != 3)
										{ 
											status = OutputFail;
											goto cleanup;
										}
									}
									else {
										/* A */
										vertices = (float *)&points[*(iptr+1)]; /* grab the first vertex from tristrip list */
										if (fwrite(vertices, 4, 3, info->f) != 3) /* write x y & z */
										{ 
											status = OutputFail;
											goto cleanup;
										}

										/* B */
										vertices = (float *)&points[*iptr];
										if (fwrite(vertices, 4, 3, info->f) != 3)
										{ 
											status = OutputFail;
											goto cleanup;
										}
									}

									/* C */
									vertices = (float *)&points[*(iptr+2)];
									if (fwrite(vertices, 4, 3, info->f) != 3)
									{ 
										status = OutputFail;
										goto cleanup;
									}

									/* Attr */
									short zero = 0;
									if (fwrite(&zero, 2, 1, info->f) != 1)
									{ 
										status = OutputFail;
										goto cleanup;
									}

									/* we have successfully written another triangle */
									info->triangle_count++;
								}
								else {		// ASCII output

									/* facet normal (0,0,0) */
									if (fprintf(info->f, "facet normal %f %f %f\n", normal[0], normal[1], normal[2]) < 0)
									{ 
										status = OutputFail;
										goto cleanup;
									}

									/* outer loop */
									if (fprintf(info->f, "  outer loop\n") < 0)
									{ 
										status = OutputFail;
										goto cleanup;
									}

									/* flip flop for tristrips */
									if (i%2 == 0)
									{
										/* vertex v11 v12 v13 */
										vertices = (float *)&points[*iptr]; /* grab the first vertex from tristrip list */
										if (fprintf(info->f, "    vertex %f %f %f\n", *vertices, *(vertices+1), *(vertices+2)) < 0)
										{ 
											status = OutputFail;
											goto cleanup;
										}

										/* vertex v21 v22 v23 */
										vertices = (float *)&points[*(iptr+1)];
										if (fprintf(info->f, "    vertex %f %f %f\n", *vertices, *(vertices+1), *(vertices+2)) < 0)
										{ 
											status = OutputFail;
											goto cleanup;
										}
									} else {
										/* vertex v11 v12 v13 */
										vertices = (float *)&points[*(iptr+1)]; /* grab the first vertex from tristrip list */
										if (fprintf(info->f, "    vertex %f %f %f\n", *vertices, *(vertices+1), *(vertices+2)) < 0)
										{ 
											status = OutputFail;
											goto cleanup;
										}

										/* vertex v21 v22 v23 */
										vertices = (float *)&points[*iptr];
										if (fprintf(info->f, "    vertex %f %f %f\n", *vertices, *(vertices+1), *(vertices+2)) < 0)
										{ 
											status = OutputFail;
											goto cleanup;
										}
									}

									/* vertex v31 v32 v33 */
									vertices = (float *)&points[*(iptr+2)];
									if (fprintf(info->f, "    vertex %f %f %f\n", *vertices, *(vertices+1), *(vertices+2)) < 0)
									{ 
										status = OutputFail;
										goto cleanup;
									}

									/* endloop */
									if (fprintf(info->f, "  endloop\n") < 0)
									{ 
										status = OutputFail;
										goto cleanup;
									}

									/* endfacet */
									if (fprintf(info->f, "endfacet\n") < 0)
									{ 
										status = OutputFail;
										goto cleanup;
									}

									/* we don't need to count triangles in ascii mode */
								}
							}
							iptr += 3;  /* done with tristrip; move to next */
						}
					}
				}
			HC_End_Contents_Search();
		}
	HC_Close_Segment();

cleanup:

	delete [] points;
	delete [] tristrips;
	delete [] pathname;
	delete [] type;

	return status;
}

HFileOutputResult HIOUtilityStl::FileOutputByKey(const wchar_t *FileName,
												 HC_KEY key,
												 HOutputHandlerOptions * options)
{
	if (!FileName) return OutputBadFileName;
	if (key == INVALID_KEY) return OutputFail;
	if (!options) return OutputBadOptions;
	const unsigned int triangle_count = 0;
	stl_info *info = new stl_info;
	if (!info) return OutputFail;
	info->is_ascii = options->m_bAscii;
	info->start_segment = key;
	info->triangle_count = 0;

	
	SetStartingOutput();

	if (!options->m_bAscii)	//BINARY
	{
		info->f = wfopen(FileName, L"wb+"); // open the file as BINARY
		if (info->f == NULL)
			return OutputFail;
		char header[80] = {
			3,1,4,1,5,9,2,6,5,3,5,8,9,7,9,3,2,3,8,4,
			6,2,6,4,3,3,8,3,2,7,9,5,0,2,8,8,4,1,9,7,
			1,6,9,3,9,9,3,7,5,1,0,5,8,2,0,9,7,4,9,4,
			4,5,9,2,3,0,7,8,1,6,4,0,6,2,8,6,2,0,8,9
		};
		if (fwrite(header, 1, 80, info->f) != 80)
			return OutputFail;
		if (fwrite(&triangle_count, 4, 1, info->f) != 1)
			return OutputFail;
	}
	else {	//ASCII
		info->f = wfopen(FileName, L"w+"); // open the file as ASCII
		if (info->f == NULL) 
			return OutputFail;
		if (fprintf(info->f, "solid hoopspart\n") < 0)
			return OutputFail;
	}

	char segname[MVO_BUFFER_SIZE];
	HC_Show_Segment(key, segname);
	HC_KEY temp = HC_KCopy_Segment(segname, "/temporary stl include directory with an indisputably unique name");
	HC_Optimize_Segment_Tree_By_Key(temp, "collapse matrices=on,expand includes=on,merge shells=on,reorganize=spatial");
	info->copy_segment = temp;

	if (ComputeStlTriangles(temp, info, options) == OutputFail)
		return OutputFail;

	HC_Delete_By_Key(temp);

	if (!options->m_bAscii)	//BINARY
	{
		if (fseek(info->f, 80, SEEK_SET) != 0)
			return OutputFail;
		int temp[1] = {info->triangle_count};
		if (fwrite(temp, 4, 1, info->f) != 1)
			return OutputFail;
	}
	else {	//ASCII
		if (fprintf(info->f, "endsolid hoopspart\n") < 0)
			return OutputFail;
	}
	fclose(info->f);

	delete info;

	SetFinishedOutput();

	return OutputOK;
}

HFileInputResult HIOUtilityStl::ReadStlFile(const wchar_t *FileName) 
{
	if(!FileName) return InputBadFileName;
	
	FILE *f;
	char teststring[128];
	HFileInputResult return_val;
	bool is_ascii = false;
	int num_read, count=0;

	/* load a bit of test data */
	f = wfopen(FileName, L"rb"); // open the file as "read binary"
	if (f == NULL) 
		return InputFail;
	//we may not get the full 100 bytes we ask for, but that's ok.
	num_read = (int)fread( teststring, 1, 100, f );  
	teststring[num_read] = '\0';
    fclose( f );
 	/* here we assume that the file is ascii if and only if 
	 * a) it starts with "solid", and
	 * b) it contains "facet" somewhere within the first 100 bytes */ 
	char * strptr = &teststring[0];
	while ((*strptr == ' ' || *strptr == '\t' || *strptr == '\n' || *strptr == '\r'  || *strptr == '\f') && count++ < num_read)
		strptr++;
	/* now that we're past the whitespace */
	is_ascii = (strstr( strptr, "solid" ) == strptr ) || (strstr( strptr, "SOLID" ) == strptr );
	if (is_ascii)
		is_ascii = (strstr( teststring, "facet" ) != NULL ) || (strstr( teststring, "FACET" ) != NULL );
    if (is_ascii)
	    return_val = ReadStlFileAscii( FileName );
	else
		return_val = ReadStlFileBinary( FileName );

	return return_val;
}

HFileOutputResult HIOUtilityStl::WriteStlFile(const wchar_t *FileName,
											  const float *vertices,
											  const float *normals,
											  unsigned int triangle_count,
											  bool is_ascii,
											  char *partname)
{
	if (!FileName) return OutputBadFileName;

	HFileOutputResult return_val;

	if (is_ascii)
		return_val = WriteStlFileAscii( FileName, vertices, normals, triangle_count, partname );
	else
		return_val = WriteStlFileBinary( FileName, vertices, normals, triangle_count );

	return return_val;
}


#define SWAPL(a) ((((a)&0x00ffp) << 24) | (((a)&0x00ff00) << 8) | \
					(((a)&0x00ff0000) >> 8) | (((a) >> 24) & 0x00ffp))
#define FIXL(a) (*(int *)(&(a)) = SWAPL(*(int *)(&(a))))

/*
Helper function for writing a binary Stereolithography file.
*/
HFileOutputResult HIOUtilityStl::WriteStlFileBinary(const wchar_t *FileName,
												   const float *vertices,
												   const float *normals,
												   unsigned int triangle_count)
{
	int header[80] = {
		3,1,4,1,5,9,2,6,5,3,5,8,9,7,9,3,2,3,8,4,
		6,2,6,4,3,3,8,3,2,7,9,5,0,2,8,8,4,1,9,7,
		1,6,9,3,9,9,3,7,5,1,0,5,8,2,0,9,7,4,9,4,
		4,5,9,2,3,0,7,8,1,6,4,0,6,2,8,6,2,0,8,9
	};
	FILE *f;

	f = wfopen(FileName, L"wb+"); // open the file as BINARY
	if (f == NULL) 
		return OutputFail;

	if( fwrite(header, 4, 80, f) != 1)
		return OutputFail;

	if (triangle_count == 0)
		return OutputFail;
	if (fwrite(&triangle_count, 4, 1, f) != 1)
		return OutputFail;

	while (triangle_count > 0)
	{
		if (fwrite(normals++, 4, 1, f) != 1)
			return OutputFail;
		if (fwrite(vertices++, 4, 3, f) != 1)
			return OutputFail;
		triangle_count--;
	}

	fclose(f);
	return OutputOK;
}

/*
Helper function for writing a ascii Stereolithography file.
*/
HFileOutputResult HIOUtilityStl::WriteStlFileAscii(const wchar_t *FileName,
													const float *vertices,
													const float *normals,
													unsigned int triangle_count,
													char *partname)
{
	FILE *f;

	f = wfopen(FileName, L"w+"); // open the file as ASCII
	if (f == NULL) 
		return OutputFail;

	if (triangle_count == 0)
		return OutputFail;

	/* solid name */
	if (fprintf(f, "solid %s\n", partname) < 0)
		return OutputFail;

	while (triangle_count > 0)
	{
		float n1=*normals++, n2=*normals++, n3=*normals++;
		float v11=*vertices++, v12=*vertices++, v13=*vertices++;
		float v21=*vertices++, v22=*vertices++, v23=*vertices++;
		float v31=*vertices++, v32=*vertices++, v33=*vertices++;

		/* facet normal n1 n2 n3 */
		if (fprintf(f, "facet normal %e.6 %e.6 %e.6\n", n1, n2, n3) < 0)
			return OutputFail;

		/* outer loop */
		if (fprintf(f, "  outer loop\n") < 0)
			return OutputFail;

		/* vertex v11 v12 v13 */
		if (fprintf(f, "    vertex %e.6 %e.6 %e.6\n", v11, v12, v13) < 0)
			return OutputFail;

		/* vertex v21 v22 v23 */
		if (fprintf(f, "    vertex %e.6 %e.6 %e.6\n", v21, v22, v23) < 0)
			return OutputFail;

		/* vertex v31 v32 v33 */
		if (fprintf(f, "    vertex %e.6 %e.6 %e.6\n", v31, v32, v33) < 0)
			return OutputFail;

		/* endloop */
		if (fprintf(f, "  endloop\n") < 0)
			return OutputFail;

		/* endfacet */
		if (fprintf(f, "endfacet\n") < 0)
			return OutputFail;

		triangle_count--;
	}

	fclose(f);
	return OutputOK;
}

/*
  Helper function for reading a binary Stereolithography file.
*/
HFileInputResult HIOUtilityStl::ReadStlFileBinary(const wchar_t *FileName) 
{
	int const NUM_TRIS_PER_SHELL = 40000;
		
	FILE *	fp = wfopen(FileName, L"rb"); // open the file as BINARY
	if (fp == NULL) 
		return InputFail;

	/* rewind the file to the beginning, and read the header info: 
	 * 80 bytes of title, plus a 4 byte int to indicate # of faces */
    fseek( fp, 80, SEEK_SET );

	int facets_left;

	if( fread( &facets_left, 4, 1, fp ) != 1 )
		return InputFail;

	/* 40000 triangles or less */
	int workspace_count = MIN(NUM_TRIS_PER_SHELL, facets_left);

	HPoint *vertexes = new HPoint[workspace_count * 3];
	HPoint *pout = new HPoint [workspace_count * 3];
	int *flist = new int [workspace_count * 4];
	int *fout = new int [workspace_count * 4];

	if (vertexes == 0 || pout == 0 || flist == 0 || fout == 0) 
		return InputFail;

	//get the facets
	while (facets_left > 0){
		
		char buffer[sizeof(HPoint) + 3*sizeof(HPoint) +sizeof(short)];  //normal, v1, v2, v3, attribute byte count (always zero)

		int point_index = 0;
		int triangle_count;

		for( triangle_count = 0; triangle_count < NUM_TRIS_PER_SHELL; ++triangle_count, --facets_left)
		{
			/* Read the triangle from the file and copy the points to our buffer. */
			if(fread(buffer, sizeof(buffer), 1, fp) != 1){
				break;
			} else {
				memcpy(&vertexes[point_index], buffer + sizeof(HPoint), 3*sizeof(HPoint));
				point_index += 3;
			}
		}

		if (triangle_count) {
			int npout;
			int foutlen;
			int flistlen = triangle_count * 4;   //assumes triangles only

			//build face list
			point_index = 0;
			for(int j = 0 ; j < triangle_count ; ++j ){	
				/* j<<2 == j*4 */
				flist[(j<<2)+0] = 3;
				flist[(j<<2)+1] = point_index++; 
				flist[(j<<2)+2] = point_index++; 
				flist[(j<<2)+3] = point_index++; 
			}

			HC_Compute_Optimized_Shell(
					point_index, vertexes, 0, flistlen, flist,
					"orphan elimination,"
					"tolerance = 0.01 % fru",
					&npout, pout, &foutlen, fout,
					0,0
			);
			HC_Insert_Shell( npout, pout, foutlen, fout);

		}
		else 
			break;
	}

	delete [] fout; 
	delete [] pout;
	delete [] flist;
	delete [] vertexes;
	fclose(fp);

	return InputOK;
}


/*
  Helper function for reading an ASCII Stereolithography file
*/
HFileInputResult HIOUtilityStl::ReadStlFileAscii(const wchar_t *FileName) 
{
	HPoint	p[4], n; 
	HPoint	*pbuf;									// vertices
	char	partName[160];
	int		pi=0, inSolid = 0, inFacet = 0; 
	int		pbufCnt = 0;							// counter of all vertices
	int		pbufMax = 4096;							// allocated size
	FILE *f;
	unsigned int i;

	inSolid=inSolid;
	inFacet=inFacet;

	f = wfopen(FileName, L"r"); // open the file for reading as ASCII
	if (f == NULL) 
		return InputFail;

	pbuf = (HPoint *) malloc (pbufMax * sizeof(HPoint));


	while (!feof(f)) 
	{
		char tok[MVO_BUFFER_SIZE];
 
 		if	(fscanf(f,"%s",tok) != 1) 	continue; 

		// make it lower case
		for(i = 0; i < strlen(tok); i++)
			tok[i] = (char) tolower(tok[i]);

		switch (tok[0]) 
		{
		case 's' :
			if (streq(tok,"solid")) 
			{
				inSolid = 1;
				 // partname
   				if	(fscanf(f,"%s",partName) != 1) 	continue; 

				// make it lower case
				for(i = 0; i < strlen(tok); i++)
					tok[i] = (char) tolower(tok[i]);

				break;
			} 
	
		case 'e' :
			if (streq(tok,"end solid")) 
			{
				inSolid = 0;
				break;
			} 
			else if (streq(tok,"end")) 
			{
   				if	(fscanf(f,"%s",tok) != 1) 	continue; 
				
				// make it lower case
				for(i = 0; i < strlen(tok); i++)
					tok[i] = (char) tolower(tok[i]);

				if (streq(tok,"solid")) inSolid = 0;

				break;
			} 
			else if (streq(tok,"endfacet")) 
			{
				// store the 3 coordinates
				if ((pbufCnt + 3) >= pbufMax) 
				{
					pbufMax +=1024;
					pbuf = (HPoint *) realloc(pbuf,pbufMax * sizeof(HPoint));
				}

				pbuf[pbufCnt] = p[0]; pbufCnt ++;
				pbuf[pbufCnt] = p[1]; pbufCnt ++;
				pbuf[pbufCnt] = p[2]; pbufCnt ++;

				inFacet = 0;
				break;
			} 
			else if (streq(tok,"endloop")) 
			{
				break;
			} 

		case 'f' :
			if (streq(tok,"facet")) 
			{
				inFacet = 1;
				pi=0;
				break;
			} 

		case 'l' :
			if (streq(tok,"loop")) 
			{
				break;
			} 

		case 'v' :
			if (streq(tok,"vertex")) 
			{
				if (pi >= 3) pi=0; // to many data, should not occur

				HCLOCALE(fscanf(f,"%f %f %f",&p[pi].x,&p[pi].y,&p[pi].z));
				pi++;
				break;
			} 

		case 'n' :
			if (streq(tok,"normal")) 
			{
				 HCLOCALE(fscanf(f,"%f %f %f",&n.x,&n.y,&n.z));
				 break;
			} 
	
		default :
			{
				//TRACE("Unknown Token : %s \n",tok);
			}
			break;
		} // switch 

	}

	//TRACE("Triangles read : %d \n",pbufCnt / 3 );

	// build facelist 
	{
		int *flist; //Facelist
		int fcnt = (pbufCnt/3) * 4;

		flist = (int *) malloc(fcnt*sizeof(int));
		int *fp=flist;
		int i=0;

		while (i<pbufCnt) 
		{
			*fp++=3;
			*fp++ = i++;
			*fp++ = i++;
			*fp++ = i++;
		}

		if (1)
		{  // minimize 
			int npout;
			HPoint *pout = (HPoint *) malloc(pbufCnt * sizeof(HPoint));
			int nfout;
			int *fout = (int *) malloc(fcnt * sizeof(int));

			HC_Compute_Minimized_Shell(pbufCnt,pbuf,fcnt,flist,
					"orphan elimination,"
					"normal tolerance = 0 degrees,"
					"tolerance = 0.01 % fru",
      				&npout,pout,&nfout,fout,NULL,NULL);
			HC_Insert_Shell(npout,pout,nfout,fout);
			free(fout); 
			free(pout);

		} else 
		{
			HC_Insert_Shell(pbufCnt,pbuf,fcnt,flist);
		}
	
		free(flist);  
	}

	free(pbuf);
	fclose(f);
  
	return InputOK;
}
