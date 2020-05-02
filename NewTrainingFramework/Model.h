#pragma once
#include <string>
#include "../Utilities/Math.h"
#include <vector>
#include "Vertex.h"
#include "Resources.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"

class Model {
public:
	Model() = default;
	~Model();
	Model(const std::string& path);
	Model(const VertexBuffer& vb, const IndexBuffer& ib); //constructor for generated models

	void ReadModelFile(const std::string& file);

	const IndexBuffer& GetIb() const;
	const VertexBuffer& GetVd() const;	

	const std::vector<GLushort>& getIndcies() const;
	const std::vector<Vertex>& getVerts() const;	
	
	  
private:	
	GLuint wiredIboId; // id buffer afisare wired
	VertexBuffer vb; //VertexBuffer generat pentru model
	IndexBuffer ib; //IndexBuffer generat pentru model	

	//Variable citire model din fisier		 
	
	std::vector<GLushort> indicies;//vectori vertecs cititi din nfg
	std::vector<Vertex> verticies;//vectori indici cititi din nfg

	unsigned int nrvertces = 0; //nr vertecsi
	unsigned int nrindicies = 0; // nr indici
	
};
