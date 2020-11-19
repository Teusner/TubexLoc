#include <tubex.h>
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
		Trajectory(tdomain, TFunction("5*sin(2*t)+t"), dt)
	});


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
		v[0].inflate(0.1);
		v[1].inflate(0.04);
		v[2].inflate(0.04);
	}


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

	fig_map.show(1);
}