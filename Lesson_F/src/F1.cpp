#include <tubex.h>
#include <vibes.h>
#include <math.h>

using namespace std;
using namespace tubex;

int main() {

	double dt = 0.01;
	Interval tdomain(0,5);

	// Trajectory
	TrajectoryVector actual_x(tdomain, TFunction("(2*cos(t) ; sin(2*t))"), dt);

	// TubeVector x unknown
	TubeVector x(tdomain, dt, 2);

	// TubeVector v measured with a noise in [-0.03; 0.03]
	TubeVector v(tdomain, dt, TFunction("(-2*sin(t) ; 2*cos(2*t))"));
	v.inflate(0.03);

	// Measurement
	Interval ti (2.);
	IntervalVector yi({{-0.84, -0.83}, {-0.76, -0.75}});

	vibes::beginDrawing();

	VIBesFigMap fig_map("Map");	
	fig_map.set_properties(100, 100, 600, 300);
	fig_map.add_trajectory(&actual_x, "x*", 0, 1);
	fig_map.add_tube(&x, "x", 0, 1);
	fig_map.add_beacon(yi);
	fig_map.axis_limits(-2.5,2.5,-0.1,0.1, true);

	// Contractor network
	ContractorNetwork cn;
	cn.add(ctc::eval, {ti, yi, x, v});
	cn.contract();

	fig_map.show(0.5);
}
