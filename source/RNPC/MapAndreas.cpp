//----------------------------------------------------------
//
//   SA-MP Multiplayer Modification For GTA:SA
//   Copyright 2004-2010 SA-MP Team
//
//   Author: Kye 10 Jan 2010
//	 Edits: Mauzen 2011/2012
//
//----------------------------------------------------------
#pragma warning( disable : 4996 )

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "MapAndreas.h"

//----------------------------------------------------------

CMapAndreas::CMapAndreas()
{
	m_iOperatingMode = MAP_ANDREAS_MODE_NONE;
	m_pPointData = NULL;
}

//----------------------------------------------------------

CMapAndreas::~CMapAndreas()
{
	m_iOperatingMode = MAP_ANDREAS_MODE_NONE;
    if(m_pPointData) free(m_pPointData);
	m_pPointData = NULL;
}

//----------------------------------------------------------

int CMapAndreas::Init(int iMode)
{
	// check if already inited
	if(m_iOperatingMode != MAP_ANDREAS_MODE_NONE) 
		return MAP_ANDREAS_ERROR_SUCCESS;

	if(iMode == MAP_ANDREAS_MODE_FULL)
	{
        // allocate the memory we need
		m_pPointData = (unsigned short *)calloc(MAP_ANDREAS_POINTS_FULL,sizeof(unsigned short));
		if(NULL == m_pPointData) return MAP_ANDREAS_ERROR_MEMORY;

		// load the file contents in to our point data buffer
		FILE *fileInput = fopen(MAP_ANDREAS_HMAP_FILE_FULL,"rb");
		if(NULL == fileInput) return MAP_ANDREAS_ERROR_DATA_FILES;
		
		for(int x=0;x<MAP_ANDREAS_POINTS_FULL;x++) {
            fread(&m_pPointData[x],1,sizeof(unsigned short),fileInput);
		}

		fclose(fileInput);

        m_iOperatingMode = MAP_ANDREAS_MODE_FULL;
		m_gridSize = MAP_ANDREAS_GRID_FULL;
		return MAP_ANDREAS_ERROR_SUCCESS;
	} else

	if(iMode == MAP_ANDREAS_MODE_MEDIUM)
	{
        // allocate the memory we need
		m_pPointData = (unsigned short *)calloc(MAP_ANDREAS_POINTS_MEDIUM,sizeof(unsigned short));
		if(NULL == m_pPointData) return MAP_ANDREAS_ERROR_MEMORY;

	
		// load the file contents in to our point data buffer
		FILE *fileInput = fopen(MAP_ANDREAS_HMAP_FILE_FULL,"rb");
		if(NULL == fileInput) return MAP_ANDREAS_ERROR_DATA_FILES;
		
		unsigned int waste = 0;

		for(int x=0;x<MAP_ANDREAS_POINTS_MEDIUM;x++) {
			if(x % 6000 == 0 && x != 0) {		// Skip a line
				for (int i = 0; i < 6000; i++)
				{
					fread(&waste,1,sizeof(unsigned short),fileInput);
				}
			}
			fread(&m_pPointData[x],1,sizeof(unsigned short),fileInput);
            fread(&waste,1,sizeof(unsigned short),fileInput);
		}

		fclose(fileInput);

        m_iOperatingMode = MAP_ANDREAS_MODE_MEDIUM;
		m_gridSize = MAP_ANDREAS_GRID_MEDIUM;
		return MAP_ANDREAS_ERROR_SUCCESS;
	} else

	if(iMode == MAP_ANDREAS_MODE_MINIMAL)
	{
        // allocate the memory we need
		m_pPointData = (unsigned short *)calloc(MAP_ANDREAS_POINTS_MINIMAL,sizeof(unsigned short));
		if(NULL == m_pPointData) return MAP_ANDREAS_ERROR_MEMORY;

	
		// load the file contents in to our point data buffer
		/*
		// Just for creating the hmap file
		FILE *fileInput = fopen(MAP_ANDREAS_HMAP_FILE_FULL,"rb");
		if(NULL == fileInput) return MAP_ANDREAS_ERROR_DATA_FILES;
		
		unsigned int pos = 0;

		fseek(fileInput, 12002, SEEK_SET);

		for (int l = 1; l < 6000; l += 3)
		{
			for (int c = 1; c < 6000; c += 3)
			{				
				fread(&m_pPointData[pos++],1,sizeof(unsigned short),fileInput);
				fseek(fileInput, 4, SEEK_CUR);
			}
			fseek(fileInput, 24000, SEEK_CUR);
		}

		fclose(fileInput);

		fileInput = fopen(MAP_ANDREAS_HMAP_FILE_MINIMAL,"wb");
		//for(int i = 0; i < 4000000; i ++)
		//{
			fwrite(m_pPointData,2,4000000,fileInput);
		//}
		
		fclose(fileInput);
		*/

		// load the file contents in to our point data buffer
		FILE *fileInput = fopen(MAP_ANDREAS_HMAP_FILE_MINIMAL, "rb");
		if(NULL == fileInput) return MAP_ANDREAS_ERROR_DATA_FILES;
		
		for(int x=0;x<MAP_ANDREAS_POINTS_MINIMAL;x++) {
            fread(&m_pPointData[x],1,sizeof(unsigned short),fileInput);
		}

		fclose(fileInput);

        m_iOperatingMode = MAP_ANDREAS_MODE_MINIMAL;
		m_gridSize = MAP_ANDREAS_GRID_MINIMAL;
		return MAP_ANDREAS_ERROR_SUCCESS;
	} else

	if(iMode == MAP_ANDREAS_MODE_NOBUFFER)
	{
        m_iOperatingMode = MAP_ANDREAS_MODE_NOBUFFER;
		m_gridSize = MAP_ANDREAS_GRID_FULL;
		m_pPointData = (unsigned short *)calloc(1,sizeof(unsigned short));
		mapFile = fopen(MAP_ANDREAS_HMAP_FILE_FULL,"rb");
		if(NULL == mapFile) return MAP_ANDREAS_ERROR_DATA_FILES;
		return MAP_ANDREAS_ERROR_SUCCESS;
	}
	
	return MAP_ANDREAS_ERROR_FAILURE;
}

//----------------------------------------------------------

float CMapAndreas::FindZ_For2DCoord(float X, float Y)
{
	// check for a co-ord outside the map
	if(X < -3000.0f || X > 3000.0f || Y > 3000.0f || Y < -3000.0f) return 0.0f;

	// get row/col on 6000x6000 grid
	int iGridX = ((int)X) + 3000;
	int iGridY = (((int)Y) - 3000) * -1;
	int iDataPos;
   
	if(m_iOperatingMode == MAP_ANDREAS_MODE_FULL)
	{
		iDataPos = (iGridY * 6000) + iGridX; // for every Y, increment by the number of cols, add the col index.
		return (float)m_pPointData[iDataPos] / 100.0f; // the data is a float stored as ushort * 100
	} else

	if(m_iOperatingMode == MAP_ANDREAS_MODE_MEDIUM)
	{		
		iDataPos = (iGridY * 3000) + iGridX / 2; // for every Y, increment by the number of cols, add the col index.
		if(iGridY % 2 != 0) iDataPos -= 3000;
		return (float)m_pPointData[iDataPos] / 100.0f; // the data is a float stored as ushort * 100
	} else

	if(m_iOperatingMode == MAP_ANDREAS_MODE_MINIMAL)
	{		
		iDataPos = ((iGridY / 3) * 2000) + iGridX / 3; // for every Y, increment by the number of cols, add the col index.
		return (float)m_pPointData[iDataPos] / 100.0f; // the data is a float stored as ushort * 100
	} else

	if(m_iOperatingMode == MAP_ANDREAS_MODE_NOBUFFER)
	{		
		// load the file contents in to our point data buffer
		iDataPos = (iGridY * 6000) + iGridX;			
		
		fseek(mapFile,iDataPos * sizeof(unsigned short), SEEK_SET);
        fread(&m_pPointData[0],1,sizeof(unsigned short), mapFile);

		//fclose(mapFile);

		return (float)m_pPointData[0] / 100.0f;
	}

	return 0.0f;
}

Node * CMapAndreas::GetCornersInArea(float xmin, float ymin, float xmax, float ymax, float zdiff)
{
	std::vector<Node> nodes;

	float zv;
	int count = 0;
	for(float cy = ymin; cy <= ymax; cy++) {
		for(float cx = xmin; cx <= xmax; cx++) {
			zv = FindZ_For2DCoord(cx, cy);
			/*if (fabs(zv - FindZ_For2DCoord(cx + 1.0, cy)) <= zdiff) count++;
			if (fabs(zv - FindZ_For2DCoord(cx - 1.0, cy)) <= zdiff) count++;
			if (fabs(zv - FindZ_For2DCoord(cx, cy + 1.0)) <= zdiff) count++;
			if (fabs(zv - FindZ_For2DCoord(cx, cy - 1.0)) <= zdiff) count++;
			if (fabs(zv - FindZ_For2DCoord(cx + 1.0, cy + 1.0)) <= zdiff) count++;
			if (fabs(zv - FindZ_For2DCoord(cx - 1.0, cy - 1.0)) <= zdiff) count++;
			if (fabs(zv - FindZ_For2DCoord(cx - 1.0, cy + 1.0)) <= zdiff) count++;
			if (fabs(zv - FindZ_For2DCoord(cx + 1.0, cy - 1.0)) <= zdiff) count++;
			if (*/
		}
	}
	return NULL;
}

float CMapAndreas::GetAverageZ(float x, float y)
{
	float p2;
	float p3;
	float xx;
	float yy;

	float p1 = FindZ_For2DCoord(x,y);
	if (x < 0.0f) p2 = FindZ_For2DCoord(x + m_gridSize, y);
	    else p2 = FindZ_For2DCoord(x - m_gridSize, y);
	if (y < 0.0f) p3 = FindZ_For2DCoord(x, y + m_gridSize);
	    else p3 = FindZ_For2DCoord(x, y - m_gridSize);
	
	#pragma warning (disable : 4244)
	double temp;
	xx = modf(x, &temp);
	yy = modf(y, &temp);
	if (xx < 0) x = -xx;
	if (yy < 0) y = -yy;

	temp = p1 + xx * (p1 - p2) + yy * (p1 - p3);
	
	return (float)temp;
}

bool CMapAndreas::Unload()
{
	if (mapFile != NULL)
	{
		fclose(mapFile);
	}
	free(m_pPointData);
	m_iOperatingMode = MAP_ANDREAS_MODE_NONE;
	return true;
}

//----------------------------------------------------------