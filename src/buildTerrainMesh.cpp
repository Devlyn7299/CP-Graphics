#include "buildTerrainMesh.h"

void buildTerrainMesh(ofMesh& terrainMesh, const ofShortPixels& heightmap, 
	unsigned int xStart, unsigned int yStart, unsigned int xEnd, unsigned int yEnd, glm::vec3 scale)
{
	//cout << "Inner Test 1" << endl;

	int w = xEnd - xStart;
	int d = yEnd - yStart;
	// Will go left to right, top to bottom, by looping the y-coord, and looping x-coord within
	// Grab every vertex
	for (int y = yStart; y < yEnd; y++)
	{
		//cout << "Inner Test 2" << endl;

		for (int x = xStart; x < xEnd; x++)
		{
			//cout << "Inner Test 3" << endl;
			double h = heightmap.getColor(x, y).r;
			terrainMesh.addVertex(ofPoint(x, h / USHRT_MAX, y) * scale);
			//terrainMesh.addVertex(glm::vec3(static_cast<float>(j), 
			//	(static_cast<float>(heightmap.getColor(j, i).r) / static_cast<float>(USHRT_MAX)), 
			//	static_cast<float>(i)) * scale);
		}
	}
	//cout << "Inner Test 4" << endl;

	//// Adding indices 
	//for (int i = 0; i < yEnd - 1; i++)
	//{
	//	//cout << "Inner Test 5" << endl;

	//	for (int j = 0; j < xEnd - 1; j++)
	//	{
	//		//cout << "Inner Test 6" << endl;

	//		terrainMesh.addIndex(j + i * xEnd);					
	//		terrainMesh.addIndex(j + (i + 1) * xEnd);			
	//		terrainMesh.addIndex((j + 1) + i * xEnd);			

	//		terrainMesh.addIndex((j + 1) + (i + 1) * xEnd);		
	//		terrainMesh.addIndex((j + 1) + i * xEnd);			
	//		terrainMesh.addIndex(j + (i + 1) * xEnd);			

	//	}
	//}

	for (int y = 0; y < yEnd - yStart - 1; y++)
	{
		for (int x = 0; x < xEnd - xStart - 1; x++)
		{
			terrainMesh.addIndex(x + y * (xEnd - xStart));
			terrainMesh.addIndex(x + (y + 1) * (xEnd - xStart));
			terrainMesh.addIndex((x + 1) + y * (xEnd - xStart));

			terrainMesh.addIndex((x + 1) + y * (xEnd - xStart));
			terrainMesh.addIndex(x + (y + 1) * (xEnd - xStart));
			terrainMesh.addIndex((x + 1) + (y + 1) * (xEnd - xStart));
		}
	}

	//cout << "Inner Test 7" << endl;

	terrainMesh.flatNormals();
	//cout << "Inner Test 8" << endl;

	for (size_t i{ 0 }; i < terrainMesh.getNumNormals(); i++) {
		//cout << "Inner Test 9" << endl;

		terrainMesh.setNormal(i, -terrainMesh.getNormal(i));
	}


	/*
	//Make vertices
	int w = xEnd - xStart;
	int d = yEnd - yStart;
	for (int y = yStart; y < yEnd; y++)
	{
		for (int x = xStart; x < xEnd; x++)
		{
			double h = heightmap.getColor(x, y).r;
			terrainMesh.addVertex(ofPoint(x, h / USHRT_MAX, y) * scale);
		}
	}

	//Connect vertices with triangles
	for (int y = 0; y < yEnd - yStart - 1; y++)
	{
		for (int x = 0; x < xEnd - xStart - 1; x++)
		{
			terrainMesh.addIndex(x + y * (xEnd - xStart));
			terrainMesh.addIndex(x + (y + 1) * (xEnd - xStart));
			terrainMesh.addIndex((x + 1) + y * (xEnd - xStart));

			terrainMesh.addIndex((x + 1) + y * (xEnd - xStart));
			terrainMesh.addIndex(x + (y + 1) * (xEnd - xStart));
			terrainMesh.addIndex((x + 1) + (y + 1) * (xEnd - xStart));
		}
	}
	terrainMesh.flatNormals();
	*/

}
