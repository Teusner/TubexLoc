#include <tubex.h>
#include <math.h>

using namespace tubex;

int main() {

	Interval x1(-2., 4.), y1(1., 3.);
	Interval z1 = x1 * y1;
	std::cout << z1 << std::endl;

	Interval x2(8., 10.), y2(-1., 0.);
	Interval z2 = x2 / y2;
	std::cout << z2 << std::endl;

	Interval x3(-2., 4.), y3(6., 7.);
	Interval z3 = x3 | y3;
	std::cout << z3 << std::endl;

	Interval x4(2., 7.), y4(1., 9.);
	Interval z4 = max(x4, y4);
	std::cout << z4 << std::endl;

	Interval x5 = Interval::EMPTY_SET;
	Interval y5(1., 2.);
	Interval z5 = max(x5, y5);
	std::cout << z5 << std::endl;

	Interval x6;
	Interval z6 = cos(x6);
	std::cout << z6 << std::endl;

	Interval x7(-1., 4.);
	Interval z7 = sqr(x7);
	std::cout << z7 << std::endl;

	Interval x8(1., 2.), y8(-1., 3.), w8(1., 3.), v8(6., 7.);
	Interval z8 = (x8*y8) + max((w8 & v8), x8);
	std::cout << z8 << std::endl;
}