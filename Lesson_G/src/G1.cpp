#include <tubex.h>
#include <vibes.h>
#include <math.h>

using namespace std;
using namespace tubex;

int main() {

	double dt = 0.005;
	Interval tdomain(0, 3);

	// Unknown trajectory
	TrajectoryVector actual_x({
		Trajectory(tdomain, TFunction("10*cos(t)+t"), dt),
		Trajectory(tdomain, TFunction("5*sin(2*t)+t"), dt)
	});

	// TubeVector x unknown
	TubeVector x(tdomain, dt, 2);

	// TubeVector v measured with a noise in [-0.03; 0.03]
	TubeVector v(tdomain, dt, TFunction("(-10*sin(t)+1 ; 10*cos(2*t)+1)"));
	v.inflate(0.03);

	// Time
	IntervalVector t({Interval(0.3), Interval(1.5), Interval(2)});

	// Distance
	IntervalVector y({Interval(1.9), Interval(3.6), Interval(2.8)});
	y.inflate(0.1);

	// Landmarks
	vector<Vector> b(3, Vector(2));
	b[0] = Vector({8, 3});
	b[1] = Vector({0, 5});
	b[2] = Vector({-2, 1});


	vector<IntervalVector> d(3, IntervalVector(2, Interval(-10, 10)));
	
	ContractorNetwork cn;
	for(int i=0; i<t.size(); i++) {
		cn.add(ctc::deriv, {x, v});
		cn.add(ctc::dist, {d[i], b[i], y[i]});
        cn.add(ctc::eval, {t[i], d[i], x, v});
    }
	cn.contract();

	for(int i=0; i<t.size(); i++) {
		std::cout << d[i] << std::endl;
    }

	vibes::beginDrawing();

	VIBesFigMap fig_map("Map");	
	fig_map.set_properties(100, 100, 600, 300);
	TrajectoryVector position = actual_x.subvector(0, 1);
	fig_map.add_trajectory(&position, "x*", 0, 1);
	fig_map.axis_limits(-2.5,2.5,-0.1,0.1, true);

	for(int i = 0 ; i < b.size() ; i++) {
		Beacon bi(b[i]);
		fig_map.draw_circle(bi.x(), bi.y(), y[i].ub(), "grey"); 
		fig_map.draw_circle(bi.x(), bi.y(), y[i].lb(), "grey");
		fig_map.add_beacon(bi, 0.2);
    }
	fig_map.add_tube(&x, "x", 0, 1);
	fig_map.show(1);
}