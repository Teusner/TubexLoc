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

	Trajectory actual_y = sqrt(pow(actual_x[0] - b.x(), 2) + pow(actual_x[1] - b.y(), 2));

	vibes::beginDrawing();

	VIBesFigTube fig_dist("Distance to the landmark");
	fig_dist.set_properties(100, 100, 600, 300);
	fig_dist.add_trajectory(&actual_y, "y*");
	fig_dist.show();
}
