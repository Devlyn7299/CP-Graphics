#include "buildTerrainMesh.h"

void buildTerrainMesh(ofMesh& terrainMesh, const ofShortPixels& heightmap, 
	unsigned int xStart, unsigned int yStart, unsigned int xEnd, unsigned int yEnd, glm::vec3 scale)
{
	// Setting the width and height of the space that will be built
	int w = xEnd - xStart;
	int d = yEnd - yStart;

	// Looping through each pixel of the heightmap and adding a vertex in its place
	// Vertex height is based on the color of the heightmap in that pixel
	for (int y = yStart; y < yEnd; y++)
	{
		for (int x = xStart; x < xEnd; x++)
		{
			double h = heightmap.getColor(x, y).r;
			terrainMesh.addVertex(ofPoint(x, h / USHRT_MAX, y) * scale);
		}
	}

	// Looping through each vertex and making triangles out of them
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
	
	// Creating normals for the mesh
	terrainMesh.flatNormals();
	for (size_t i{ 0 }; i < terrainMesh.getNumNormals(); i++) 
	{
		terrainMesh.setNormal(i, -terrainMesh.getNormal(i));
	}

}
