#include <tubex.h>
#include <vibes.h>
#include <math.h>

using namespace std;
using namespace tubex;

int main() {

	double dt = 0.01;
	Interval tdomain(0,5); // temporal domain [t0,tf]
	TrajectoryVector actual_x(tdomain, TFunction("(2*cos(t) ; sin(2*t))"), dt);

	Beacon b(0.5, 1);

	vibes::beginDrawing();

	VIBesFigMap fig_map("Map");
	fig_map.set_properties(100, 100, 600, 300);
	fig_map.add_trajectory(&actual_x, "x*", 0, 1);
	fig_map.add_beacon(b, 0.1);                   // 0.1: landmark width
	fig_map.axis_limits(-2.5,2.5,-0.1,0.1, true);
	fig_map.show(0.5);                            // argument is robot size
}
