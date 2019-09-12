#include "SknReader.h"
#include <iostream>
bool SknReader::read(istream& file, char* filename )
{



	int minlen = 8;
	file.seekg (0, ios::end);
	int length = file.tellg();
	file.seekg (0, ios::beg);

	
	if (length < minlen)
		printf("\nSknReader: the file is empty!");


	int magic;
	file.read(reinterpret_cast<char*>(&magic), 4);
	if (magic != 0x00112233)
		printf("\nSknReader: magic is wrong!");


	unsigned short version;
	file.read(reinterpret_cast<char*>(&version), 2);
	if (version > 2)
		printf("\nSknReader: skn type not supported, \n please report that to ThiSpawn");
	data_.version = version;


	unsigned short num_objects;
	file.read(reinterpret_cast<char*>(&num_objects), 2);
	if (num_objects != 1)
		printf("\nSknReader: more than 1 or no objects in the file.");

	if (version == 1 || version == 2)
	{
		minlen += 4;
		if (length < minlen)
			printf("\nSknReader: unexpected end of file");

		int num_materials;
		file.read(reinterpret_cast<char*>(&num_materials), 4);

		minlen += SknMaterial::kSizeInFile * num_materials;
		if (length < minlen)
			printf("\nSknReader: unexpected end of file");

		for (int i = 0; i < num_materials; i++)
		{
			SknMaterial material;
			file.read(reinterpret_cast<char*>(&material), SknMaterial::kSizeInFile);
			data_.materials.push_back(material);
		}
	}

	minlen += 8;
	if (length < minlen)
		printf("\nSknReader: unexpected end of file");


	int num_indices;
	file.read(reinterpret_cast<char*>(&num_indices), 4);
	data_.num_indices = num_indices;
	int num_vertices;
	file.read(reinterpret_cast<char*>(&num_vertices), 4);
	data_.num_vtxs = num_vertices;
	data_.num_final_vtxs = num_vertices;

	if (num_indices % 3 != 0)
		printf("\nSknReader: num_indices % 3 != 0 ...");


	minlen += 2 * num_indices + SknVtx::kSizeInFile * num_vertices;
	if (length < minlen)
		printf("\nSknReader: unexpected end of file");



	int num_triangles = num_indices / 3;
	for (int i = 0; i < num_triangles; i++)
	{
		unsigned short indices[3];
		file.read(reinterpret_cast<char*>(&indices), 6);

		if (indices[0] == indices[1] ||
			indices[0] == indices[2] ||
			indices[1] == indices[2] ||
			indices[0] < 0 ||
			indices[0] >= data_.num_vtxs ||
			indices[1] < 0 ||
			indices[1] >= data_.num_vtxs ||
			indices[2] < 0 ||
			indices[2] >= data_.num_vtxs)
		{
			printf("\nSknReader: input mesh has a badly built triangle, removing it...");
			data_.num_indices -= 3; 
		}
		else
		{
			data_.indices.push_back(indices[0]);
			data_.indices.push_back(indices[1]);
			data_.indices.push_back(indices[2]);
		}
	}


	for (int i = 0; i < num_vertices; i++)
	{
		SknVtx vtx;
		file.read(reinterpret_cast<char*>(&vtx.x), 4);
		file.read(reinterpret_cast<char*>(&vtx.y), 4);
		file.read(reinterpret_cast<char*>(&vtx.z), 4);
		file.read(reinterpret_cast<char*>(&vtx.skn_indices), 4);
		file.read(reinterpret_cast<char*>(&vtx.weights), 16);
		file.read(reinterpret_cast<char*>(&vtx.normal), 12);
		file.read(reinterpret_cast<char*>(&vtx.U), 4);
		file.read(reinterpret_cast<char*>(&vtx.V), 4);
		data_.vertices.push_back(vtx);
	}


	if (version == 2)
		file.read(reinterpret_cast<char*>(&(data_.endTab)), 12);

	data_.switchHand();
	texture = new CImage();
	texture->Load(filename);

	return true;
}
void SknReader::render()
{
	glEnable(GL_TEXTURE_2D);
	if(texture)
		texture->Bind();

	int num_triangle = data_.num_indices/3;
	glBegin(GL_TRIANGLES);
	
	for(int i=0; i<num_triangle; i++)
	{
		

		glTexCoord2f(data_.vertices[data_.indices[i*3]].U ,1.0f - data_.vertices[data_.indices[i*3]].V);
		glNormal3fv(data_.vertices[data_.indices[i*3]].normal);
		glVertex3f(data_.vertices[data_.indices[i*3]].x,data_.vertices[data_.indices[i*3]].y, data_.vertices[data_.indices[i*3]].z );



		glTexCoord2f(data_.vertices[data_.indices[i*3+1]].U ,1.0f - data_.vertices[data_.indices[i*3+1]].V);
		glNormal3fv(data_.vertices[data_.indices[i*3+1]].normal);
		glVertex3f(data_.vertices[data_.indices[i*3+1]].x,data_.vertices[data_.indices[i*3+1]].y, data_.vertices[data_.indices[i*3+1]].z );


		glTexCoord2f(data_.vertices[data_.indices[i*3+2]].U ,1.0f - data_.vertices[data_.indices[i*3+2]].V);
		glNormal3fv(data_.vertices[data_.indices[i*3+2]].normal);
		glVertex3f(data_.vertices[data_.indices[i*3+2]].x,data_.vertices[data_.indices[i*3+2]].y, data_.vertices[data_.indices[i*3+2]].z );

	}
	glEnd();
	
}