#include "stdafx.h"
#include "Model.h"
#include <fstream>
#include <iostream>
#include <limits.h>
#include <exception>


Model::~Model()
{		
	ib.DeleteBuffer();
	vb.DeleteBuffer();
}

Model::Model(const std::string& path)
{
	ReadModelFile(path);
	vb = { verticies.data(), nrvertces };
	ib = { indicies.data(), nrindicies };	
}

Model::Model(const VertexBuffer& vb, const IndexBuffer& ib)
{
	this->vb = vb;
	this->ib = ib;
}

void Model::ReadModelFile(const std::string& file)
{
	std::ifstream model(file, std::ios::binary);
	try {
		if (model.bad() || model.fail())
		{
			throw std::runtime_error("Model " + file + " could not be loaded");
		}
		{
			std::cout << "Model " << file << " was loaded " << std::endl;

			// Citire numar de vertecsi din model		
			model.ignore(LLONG_MAX, ' ');

			model >> nrvertces;			
			verticies.reserve(nrvertces);
			// Citire parametrii (pos, binorm, etc.. pentru toate randurile din fisier)
			for (unsigned int i = 0; i < nrvertces; i++)
			{
				//Variabile temporare
				Vertex tempVer;

				//Incarcare pos la iteratia i
				model.ignore(LLONG_MAX, '[');
				model >> tempVer.pos.x;
				model.seekg(std::ios::cur, 1);
				model >> tempVer.pos.y;
				model.seekg(std::ios::cur, 1);
				model >> tempVer.pos.z;

				//Incarcare norm la iteratia i
				model.ignore(LLONG_MAX, '[');
				model >> tempVer.norm.x;
				model.seekg(std::ios::cur, 1);
				model >> tempVer.norm.y;
				model.seekg(std::ios::cur, 1);
				model >> tempVer.norm.z;

				//Incarcare binorm la iteratia i
				model.ignore(LLONG_MAX, '[');
				model >> tempVer.binorm.x;
				model.seekg(std::ios::cur, 1);
				model >> tempVer.binorm.y;
				model.seekg(std::ios::cur, 1);
				model >> tempVer.binorm.z;

				//Incarcare tgt la iteratia i
				model.ignore(LLONG_MAX, '[');
				model >> tempVer.tgt.x;
				model.seekg(std::ios::cur, 1);
				model >> tempVer.tgt.y;
				model.seekg(std::ios::cur, 1);
				model >> tempVer.tgt.z;

				//Incarcare uv la iteratia i
				model.ignore(LLONG_MAX, '[');
				model >> tempVer.uv.x;
				model.seekg(std::ios::cur, 1);
				model >> tempVer.uv.y;

				//Construire vectori de vertecsi
				verticies.push_back(tempVer);
			}

			//Citire numaru de indici
			model.ignore(LLONG_MAX, ' ');

			model >> nrindicies;			
			indicies.reserve(nrindicies);
			//Citire indici din fisierul modelului de pe toate randurile.
			for (unsigned int i = 0; i < nrindicies/3; i++)
			{
				GLushort x = 0;		

				model.ignore(LLONG_MAX, '.');
				model >> x;
				indicies.push_back(x);
				model.seekg(std::ios::cur, 1);
				model >> x;
				indicies.push_back(x);
				model.ignore(std::ios::cur, 1);
				model >> x;
				indicies.push_back(x);
			}
			model.close();
		}
	}	
	catch (const std::runtime_error& modelExcept)
	{
		std::cout << modelExcept.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "Unknown model exception for model " <<file << std::endl;
	}
}

const IndexBuffer& Model::GetIb() const { return ib; }
 
const VertexBuffer& Model::GetVd() const {	return vb; }
