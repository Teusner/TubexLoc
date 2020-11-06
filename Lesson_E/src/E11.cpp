#include <tubex.h>
#include <vibes.h>
#include <math.h>

using namespace std;
using namespace tubex;

int main() {

	double dt = 0.01;
	Interval tdomain(0,5); // temporal domain [t0,tf]
	TrajectoryVector actual_x(tdomain, TFunction("(2*cos(t) ; sin(2*t))"), dt);
	TrajectoryVector actual_v(tdomain, TFunction("(-2*sin(t) ; 2*cos(2*t))"), dt);

	Beacon b(0.5, 1);

	TubeVector x(actual_x, dt);
	x.inflate(0.2);

	vibes::beginDrawing();

	VIBesFigMap fig_map("Map");
	fig_map.set_properties(100, 100, 600, 300);
	fig_map.add_trajectory(&actual_x, "x*", 0, 1);
	fig_map.add_beacon(b, 0.1);                   // 0.1: landmark width
	fig_map.add_tube(&x, "x", 0, 1);
	fig_map.axis_limits(-2.5,2.5,-0.1,0.1, true);
	fig_map.show(0.5);
	
	Trajectory actual_y = sqrt(pow(actual_x[0] - b.x(), 2) + pow(actual_x[1] - b.y(), 2));
	
	Tube y(actual_y, dt);

	IntervalVector b1({{Interval(0.5)}, {Interval(1)}});

	TubeVector v(actual_v, dt);
	v.inflate(0.01);
	
	ContractorNetwork cn;
	cn.add(ctc::dist, {x, b1, y});
	cn.add(ctc::deriv, {x, v});
	cn.contract();
	
	fig_map.show(0.5);
}
