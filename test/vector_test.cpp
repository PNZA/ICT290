//========= Copyright 2015, Murdoch University, All rights reserved (kinda). ============
//
// Purpose: main program for testing classes
//
//=============================================================================

#include "../types/vector.h"
#include <iostream>
#include <time.h>
#include <stdlib.h>

using namespace std;

void test_init()
{
	cout << CVector(5, 10, 15) << "\n";

	cout << CVector() << "\n";

	float array[5] = {19, 18287.3, 1093.5, 1273, 1230};

	cout << CVector(array) << "\n";
}

void test_random()
{
	bool exit = false;

	while(!exit)
	{
		CVector vec = CVector(0,0,0);

		vec.Random(-5.0, 5.0);
		cout << vec;

		if(getchar() == 'y') exit = true;
	}
}

void test_get_length()
{
	CVector test_vec;
	test_vec.Random(0, 50);
	cout << "Cached Length before value: " << test_vec.GetCachedLength() << "\n";
	cout << test_vec << "\n";
	cout << "Length: " << test_vec.GetLength() << "\n";
	cout << "SQLength: " << test_vec.GetSQLength() << "\n";
	test_vec.x = 100000;
	cout << test_vec << "\n";
	cout << "Cached Length: " << test_vec.GetCachedLength();
}

void test_dot_product()
{
	CVector vec1 = CVector(0, 1, 0);
	CVector vec2 = CVector(0, 1, 0);

	cout << vec1.DotProduct(vec2) << "\n";

	vec1 = CVector(0, 0, 1);
	vec2 = CVector(0, 1, 0);

	cout << vec1.DotProduct(vec2) << "\n";

	vec1 = CVector(1, 0, 0);
	vec2 = CVector(0, 1, 0);

	cout << vec1.DotProduct(vec2) << "\n";

	vec1 = CVector(0, -1, 0);
	vec2 = CVector(0, 1, 0);

	cout << vec1.DotProduct(vec2) << "\n";

	vec1 = CVector(1, 10, 5);
	vec2 = CVector(-5, 5, 6);

	cout << vec1.DotProduct(vec2) << "\n";
}

void test_cross_product()
{
	CVector vec1 = CVector(0, 1, 0);
	CVector vec2 = CVector(0, -1, 1);
	CVector vec3;

	vec1.CrossProduct(vec2, vec3);
	cout << vec3 << "\n";
}

void test_array_get_set()
{
	CVector vec1 = CVector(100,500,100);
	CVector vec2;

	vec2.SetArray(vec1.GetArrayPtr());

	cout << vec2 << "\n";
}

void test_normalise()
{
	CVector vec1 = CVector(500, 500, 0);
	vec1.Normalise();

	cout << vec1 << "\n";
}

void test_negate()
{
	CVector vec1 = CVector(500, 500, 0);

	vec1.Negate();

	cout << vec1 << "\n";
}

void operator_test()
{
	CVector vec1 = CVector(100,100,100);
	CVector vec2 = CVector(600,188,300);

	cout << "Vec1: " << vec1 << "\n";
	cout << "Vec2: " << vec2 << "\n";

	cout << "+ " << vec1 + vec2 << "\n";
	cout << "+= " << (vec1 += vec2) << "\n";
	cout << "- " << vec1 - vec2 << "\n";
	cout << "-= " << (vec1 -= vec2) << "\n";
	cout << "* " << vec1 * vec2 << "\n";
	cout << "*= " << (vec1 *= vec2) << "\n";
	cout << "/ " << vec1 / vec2 << "\n";
	cout << "/= " << (vec1 /= vec2) << "\n";
	cout << "== " << (vec1 == vec2) << "\n";
	cout << "!= " << (vec1 != vec2) << "\n";
	cout << "= " << (vec1 = vec2) << "\n";

	vec1 = CVector(100,100,100);
	vec2 = CVector(600,188,300);

	cout << "* float " << (vec1 * 5) << "\n";
	cout << "*= float " << (vec1 *= 5) << "\n";

	vec1 = CVector(100,100,100);
	vec2 = CVector(600,188,300);

	cout << "/ float " << (vec1 / 5) << "\n";
	cout << "/= float " << (vec1 /= 5) << "\n";

	vec1 = CVector(100,100,100);
	vec2 = CVector(600,188,300);

	cout << "- " << -vec1  << "\n";

	cout << "/ float " << (vec1 / 0) << "\n"; // Should fail (does :D)
}

void multiply_add_test()
{
	CVector start = CVector(0,0,0);
	CVector direction = CVector(0,5,0);
	CVector result;
	float scale = 0.5;

	VectorMultiplyAdd(start, scale, direction, result);
	
	cout << result << "\n";

	scale = 1.5;

	VectorMultiplyAdd(start, scale, direction, result);

	cout << result << "\n";
}

void vector_interp_test()
{
	CVector start = CVector(0,0,0);
	CVector end = CVector(100,100,100);
	CVector interp;
	float percent = 0.7597;

	VectorInterp(start, end, percent, interp);

	cout << interp << "\n";
}

void vector_between_test()
{
	CVector start = CVector(0,0,0);
	CVector end = CVector(100,100,100);

	cout << VectorDistanceBetween(start, end) << "\n";
	cout << VectorDistanceBetweenSQ(start, end) << "\n";
}