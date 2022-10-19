#include "buildTerrainMesh.h"

void buildTerrainMesh(ofMesh& terrainMesh, const ofShortPixels& heightmap, 
	unsigned int xStart, unsigned int yStart, unsigned int xEnd, unsigned int yEnd, glm::vec3 scale)
{

	
	// Will go left to right, top to bottom, by looping the y-coord, and looping x-coord within
	// Grab every quad and make triangles with them
	// (j,i), (j+1,i), (j+1,i+1), (j,i+1)
	for (int i = yStart; i < yEnd; i ++)
	{
		//cout << "loop " << i << endl;

		for (int j = xStart; j < xEnd; j ++)
		{
			//if ( j > 300 && j < 400 && i > 150 && i < 300)
			//cout << static_cast<float>(heightmap.getColor(j, i).r) / static_cast<float>(65535) << endl;
			terrainMesh.addVertex(glm::vec3(static_cast<float>(j), 
				(static_cast<float>(heightmap.getColor(j, i).r) / static_cast<float>(65535)) * 50, 
				static_cast<float>(i)));
			//terrainMesh.addVertex(glm::vec3(j, 0, i));
			//terrainMesh.addVertex(glm::vec3(j, heightmap.getColor(j, i+1).r / 65535, i+1));
			//terrainMesh.addVertex(glm::vec3(j+1, heightmap.getColor(j+1, i).r / 65535, i));
			//terrainMesh.addVertex(glm::vec3(j+1, heightmap.getColor(j+1, i+1).r / 65535, i+1));
		}
	}

	//for (int i = 0; i < xEnd * yEnd; i += 2)
	//{
	//	int indices[4]
	//	{
	//		i,
	//		i + 1,
	//		i + 2,
	//		i + 3
	//	};

	//	terrainMesh.addIndex(indices[0]);
	//	terrainMesh.addIndex(indices[1]);
	//	terrainMesh.addIndex(indices[2]);
	//	terrainMesh.addIndex(indices[2]);
	//	terrainMesh.addIndex(indices[3]);
	//	terrainMesh.addIndex(indices[0]);
	//}

	for (int i = 0; i < yEnd - 1; i++)
	{
		for (int j = 0; j < xEnd - 1; j++)
		{
			terrainMesh.addIndex(j + i * xEnd);					// 0	// 1
			terrainMesh.addIndex(j + (i + 1) * xEnd);			// 3	// 4
			terrainMesh.addIndex((j + 1) + i * xEnd);			// 1	// 2

			terrainMesh.addIndex((j + 1) + (i + 1) * xEnd);		// 4	// 5
			terrainMesh.addIndex((j + 1) + i * xEnd);			// 1	// 2
			terrainMesh.addIndex(j + (i + 1) * xEnd);			// 3	// 4

		}
	}

	terrainMesh.flatNormals();
	for (size_t i{ 0 }; i < terrainMesh.getNumNormals(); i++) {
		terrainMesh.setNormal(i, -terrainMesh.getNormal(i));
	}
	


	//for (int y = 0; y < 100; y++) {
	//	for (int x = 0; x < 100; x++) {
	//		terrainMesh.addVertex(ofPoint(x, 0, y)); // make a new vertex
	//		terrainMesh.addColor(ofFloatColor(0, 0, 0));  // add a color at that vertex
	//	}
	//}

	//// now it's important to make sure that each vertex is correctly connected with the
	//// other vertices around it. This is done using indices, which you can set up like so:
	//for (int y = 0; y < 100 - 1; y++) {
	//	for (int x = 0; x < 100 - 1; x++) {
	//		terrainMesh.addIndex(x + y * 100);               // 0
	//		terrainMesh.addIndex((x + 1) + y * 100);           // 1
	//		terrainMesh.addIndex(x + (y + 1) * 100);           // 10

	//		terrainMesh.addIndex((x + 1) + y * 100);           // 1
	//		terrainMesh.addIndex((x + 1) + (y + 1) * 100);       // 11
	//		terrainMesh.addIndex(x + (y + 1) * 100);           // 10
	//	}
	//}
}
