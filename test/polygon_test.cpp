//========= Copyright 2015, Murdoch University, All rights reserved (kinda). ============
//
// Purpose: main program for testing classes
//
//=============================================================================

#include "../types/polygon.h"
#include <iostream>
#include <time.h>
#include <stdlib.h>

void constructor_test()
{
	CPolygon poly = CPolygon();
	cout << "Normal constructor test" << "\n";
	cout << poly.GetNumTexCoords() << "\n";
	cout << poly.GetNumVertices() << "\n";
	getchar();

	cout << poly.IsValid() << "\n";
	getchar();

	cout << poly.GetMidpoint() << "\n";
	cout << poly.GetNormal() << "\n";
	getchar();

	//Comment these out, they should cause assertion failures.
	//cout << poly.GetTexCoord(0).u << "\n";
	//cout << poly.GetTexCoord(0).v << "\n";
	//cout << poly.GetVertex(0) << "\n";

	vector<CVector> verts;
	vector<tex_coord_t> tex_coords;

	verts.push_back(CVector(1,1,1));
	verts.push_back(CVector(1,3,5));
	verts.push_back(CVector(1,9,6));

	tex_coord_t coord;
	coord.u = 1;
	coord.v = 5;

	tex_coords.push_back(coord);

	coord.u = 5;
	coord.v = 10;

	tex_coords.push_back(coord);

	coord.u = 50;
	coord.v = 10;

	tex_coords.push_back(coord);

	CPolygon poly2 = CPolygon(verts, tex_coords);
	cout << "Initialised constructor test" << "\n";
	cout << poly2.GetNumTexCoords() << "\n";
	cout << poly2.GetNumVertices() << "\n";
	getchar();

	cout << poly2.IsValid() << "\n";
	getchar();

	cout << poly2.GetMidpoint() << "\n";
	cout << poly2.GetNormal() << "\n";
	getchar();

	cout << poly2.GetTexCoord(0).u << "\n";
	cout << poly2.GetTexCoord(0).v << "\n";
	cout << poly2.GetVertex(0) << "\n";
}

void iterator_get_set_test()
{
	srand(time(NULL));
	CPolygon poly;
	CVector vertex;
	tex_coord_t texcoord;

	vertex.Random(0, 10);
	poly.AddVertex(vertex);
	vertex.Random(0, 10);
	poly.AddVertex(vertex);
	vertex.Random(0, 10);
	poly.AddVertex(vertex);
	vertex.Random(0, 10);
	poly.AddVertex(vertex);

	texcoord.u = 10;
	texcoord.v = 10;

	poly.AddTexCoord(texcoord);

	texcoord.u = 5;
	texcoord.v = 60;

	poly.AddTexCoord(texcoord);

	texcoord.u = 9;
	texcoord.v = 20;

	poly.AddTexCoord(texcoord);

	texcoord.u = 12;
	texcoord.v = 105;

	poly.AddTexCoord(texcoord);


	vector<CVector>::iterator itr;
	vector<tex_coord_t>::iterator itr2;

	itr = poly.GetVerticesBegin();
	itr2 = poly.GetTexCoordsBegin();

	int i = 0;

	while(itr != poly.GetVerticesEnd())
	{
		cout << *itr << " : " << (*itr2).u << ", " << (*itr2).v << "\n";
		cout << poly.GetVertex(i) << "\n";
		cout << poly.GetTexCoord(i).u << " : " << poly.GetTexCoord(i).v << "\n";
		itr++;
		itr2++;
		i++;
	}

	cout << poly.GetNormal() << "\n";
	cout << poly.GetCachedNormal() << "\n";

	cout << poly.GetMidpoint() << "\n";
	cout << poly.GetCachedMidpoint() << "\n";
}