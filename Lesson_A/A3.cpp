#include <tubex.h>
#include <math.h>

using namespace tubex;

int main() {

	IntervalVector y{{0., M_PI},{-M_PI/6, M_PI/6}};
	std::cout << abs(y) << std::endl;

}