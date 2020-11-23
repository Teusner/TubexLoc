#include <tubex.h>
#include <vibes.h>
#include <math.h>

using namespace std;
using namespace tubex;

int main() {

	double dt = 0.05;
	Interval tdomain(0, 15);

	// Initial pose x0=(0,0,2)
	Vector x0({0,0,2});

	// System input
	Trajectory u(tdomain, TFunction("3*(sin(t)^2)+t/100"), dt);

	// Random noise for u and x3
	RandTrajectory nu(tdomain, dt, Interval(-0.3,0.3));
	RandTrajectory nx3(tdomain, dt, Interval(-0.3,0.3));

	// Actual trajectories (state + derivative)
	TrajectoryVector v_truth(3);
	TrajectoryVector x_truth(3);
	v_truth[2] = u + nu;
	x_truth[2] = v_truth[2].primitive() + x0[2];
	v_truth[0] = 10*cos(x_truth[2]+nx3);
	v_truth[1] = 10*sin(x_truth[2]+nx3);
	x_truth[0] = v_truth[0].primitive() + x0[0];
	x_truth[1] = v_truth[1].primitive() + x0[1];

	// Unknown state
	TubeVector x(tdomain, dt, 3);
	TubeVector v(tdomain, dt, 3);
	Tube u_c = Tube(u+nu, dt);
	Tube xm3 = Tube(x_truth[2]+nx3, dt);

	// Contractor Network
	ContractorNetwork cn;

	// Initial condition constrain
	cn.add(ctc::eval, {Interval(0), x0, x, v});

	// Diferential constrain
	cn.add(ctc::deriv, {x, v});

	// dx = f(x, u) constrain
	CtcFunction ctc_f1(Function("vx", "xm3", "vx-10*cos(xm3)"));
	CtcFunction ctc_f2(Function("vy", "xm3", "vy-10*sin(xm3)"));
	CtcFunction ctc_f3(Function("vt", "u", "vt-u"));

	cn.add(ctc_f1, {v[0], xm3});
	cn.add(ctc_f2, {v[1], xm3});
	cn.add(ctc_f3, {v[2], u_c});

	cn.contract();

	// Graphical part
	vibes::beginDrawing();

	VIBesFigMap fig_map("Map");	
	fig_map.set_properties(100, 100, 600, 300);

	// Showing the trajectory
	fig_map.add_trajectory(&x_truth, "x*", 0, 1);

	// Showing the tube
	fig_map.smooth_tube_drawing(true);
	fig_map.add_tube(&x, "[x](.)", 0, 1);

	fig_map.show(1);
}