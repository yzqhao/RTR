#include "Matrix.h"
#include "vector2.h"
#include "vector3.h"
#include "Color.h"
#include "Light.h"
#include "Frustum.h"

using namespace RTR;
using namespace std;

#define TEST
#ifdef TEST
void testMatrix()
{
	Matrix m1;
	Matrix m2(1, 2, 3 ,4 ,5, 6, 7, 8, 11, 12, 13, 14, 15, 16, 17,18);
	Matrix m3 = m2 * m2;
	m3.print();
}

void testVector2()
{
	vector2di v1(1, 2);
	vector2di v2(2, 1);
	vector2di v3 = v1 * v2;
	int i = 3;
	v3 *= i;
	v3 = v3 * i;
	v1.print();
	v2.print();
	v3.print();
	bool t1 = v1 == v3;
	bool t2 = v2 == v3;
	cout << t1 << " ssss " << t2 << endl;
}

void testVector3()
{
	vector3di v1(1, 2, 1);
	vector3di v2(2, 1, 2);
	vector3di v3 = v1 - v2;
	v3 = v1 * v2;
	v1.print();
	v2.print();
	v3.print();
	bool t1 = v1 == v3;
	bool t2 = v2 == v3;
	cout << t1 << " ssss " << t2 << endl;
}

void testColor()
{
	Color c(255, 255, 255);
	c.print();
}
#endif

int main()
{
	testColor();

	return 0;
}

