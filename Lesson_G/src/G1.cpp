#include <tubex.h>
#include <tubex-rob.h>
#include <vibes.h>
#include <math.h>

using namespace std;
using namespace tubex;

int main() {

	double dt = 0.05;
	Interval tdomain(0, 6);

	// Unknown trajectory
	TrajectoryVector actual_x({
		Trajectory(tdomain, TFunction("10*cos(t)+t"), dt),
		Trajectory(tdomain, TFunction("5*sin(2*t)+t"), dt),
		Trajectory(tdomain, TFunction("atan2((10*cos(2*t)+1),(-10*sin(t)+1))"), dt),
		Trajectory(tdomain, TFunction("sqrt((-10*sin(t)+1)^2+(10*cos(2*t)+1)^2)"), dt)
	});

	// TubeVector x and v unknown
	TubeVector x(tdomain, dt, 4);
	TubeVector v(tdomain, dt, 4);
	TubeVector u(tdomain, dt, 2);

	// Adding measured psi and v in the state
	x[2] = Tube(actual_x[2], dt).inflate(0.01);
	x[3] = Tube(actual_x[3], dt).inflate(0.01);


	// Creating random map of landmarks
	int nb_landmarks = 150;
	IntervalVector map_area(actual_x.codomain().subvector(0,1));
	map_area.inflate(2);
	vector<IntervalVector> v_map = DataLoader::generate_landmarks_boxes(map_area, nb_landmarks);


	// Generating observations obs=(t,range,bearing) of these landmarks
	int max_nb_obs = 20;
	Interval visi_range(0, 4); // [0m,75m]
	Interval visi_angle(-M_PI/4, M_PI/4); // frontal sonar
	vector<IntervalVector> v_obs = DataLoader::generate_observations(actual_x, v_map, max_nb_obs, true, visi_range, visi_angle);
	
	// Inflating the observations
	for (auto& v:v_obs){
		v[1].inflate(0.04);
		v[2].inflate(0.04);
	}

	// Contractor Network
	ContractorNetwork cn;

	CtcConstell ctc_constell(v_map);

	CtcFunction ctc_plus(Function("a", "b", "c", "b+c-a"));
	CtcFunction ctc_eq(Function("a", "b", "a-b"));
	CtcFunction ctc_f1(Function("v", "psi", "dx", "v*cos(psi)-dx"));
	CtcFunction ctc_f2(Function("v", "psi", "dy", "v*sin(psi)-dy"));
	
	cn.add(ctc_f1, {x[3], x[2], v[0]});
	cn.add(ctc_f2, {x[3], x[2], v[1]});
	cn.add(ctc_eq, {u[0], v[2]});
	cn.add(ctc_eq, {u[1], v[3]});

	cn.add(ctc::deriv, {x, v});

	for (const auto& obs:v_obs){
		IntervalVector& mi = cn.create_dom(IntervalVector(2));

		// Data association
		cn.add(ctc_constell, {mi});

		// Evaluation of the measurement
		IntervalVector& pi = cn.create_dom(IntervalVector(4));
		cn.add(ctc::eval, {obs[0], pi, x, v});

		// Distance landmark / robot precessing
		IntervalVector& di = cn.create_dom(IntervalVector(2));
		cn.add(ctc_plus, {mi[0], pi[0], di[0]});
		cn.add(ctc_plus, {mi[1], pi[1], di[1]});

		// Distance processing
		Interval& ai = cn.create_dom(Interval());
		cn.add(ctc_plus, {ai, pi[2], obs[2]});
		cn.add(ctc::polar, {di, obs[1], ai});
	}
	cn.contract(true);

	// Graphical part
	vibes::beginDrawing();

	VIBesFigMap fig_map("Map");	
	fig_map.set_properties(100, 100, 600, 300);
	fig_map.axis_limits(map_area, true);

	// Showing beacons
	for(const auto& b:v_map) {
		Beacon bi(b);
		fig_map.add_beacon(bi, 0.2);
	}

	// Showing the trajectory
	fig_map.add_trajectory(&actual_x, "x*", 0, 1);

	// Showing measurements
	fig_map.add_observations(v_obs, &actual_x);

	// Showing the tube
	fig_map.smooth_tube_drawing(true);
	fig_map.add_tube(&x, "x*", 0, 1);

	fig_map.show(1);
}