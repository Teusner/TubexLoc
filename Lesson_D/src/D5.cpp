#include <tubex.h>
#include <vibes.h>
#include <math.h>
#include <my_ctc.hpp>

using namespace std;
using namespace tubex;

int main() {

    // The truth state of the robot
    Vector x_truth({2,1,M_PI/6});

    // Landmarks
    vector<IntervalVector> M;
	M.push_back(Vector({1.5,2.5}));
	M.push_back(Vector({3,1}));
	M.push_back(Vector({2,2}));
	M.push_back(Vector({2.5,3}));
	M.push_back(Vector({3.5,2}));
	M.push_back(Vector({4,1}));
	M.push_back(Vector({1.5,0.5}));

    // Unknown state
    IntervalVector x(3);
    x[2] = x_truth[2];

    for(auto& Mi : M)
		Mi.inflate(0.05);

    // Generating static observation
    vector<IntervalVector> v_obs = DataLoader::generate_static_observations(x_truth, M, false);

    // Adding uncertainties on the measurements
    for(auto& obs : v_obs) {
        obs[0].inflate(0.02); // range
        obs[1].inflate(0.02); // bearing
    }

    // Functions
	CtcFunction ctc_add(Function("x1", "x2", "x3", "x1+x2-x3"));
	CtcFunction ctc_sub(Function("x1", "x2", "x3", "x1-x2-x3"));

	// Contractor Network
	ContractorNetwork c;

    for(int i = 0 ; i < v_obs.size() ; i++) {
        IntervalVector& d = c.create_dom(IntervalVector(2));
        Interval& theta = c.create_dom(Interval());

        c.add(ctc_add, {x[2], v_obs[i][1], theta});
        c.add(ctc_sub, {M[i][0], x[0], d[0]});
        c.add(ctc_sub, {M[i][1], x[1], d[1]});
        c.add(ctc::polar, {d, v_obs[i][0], theta});
    }
    c.contract();

	vibes::beginDrawing();

    VIBesFigMap fig_map("Map");
    fig_map.set_properties(100,100,500,500);
    
    for(const auto& Mi : M)
        fig_map.draw_box(Mi, "red[orange]");

    for(const auto& obs : v_obs) {
        fig_map.draw_pie(x_truth[0], x_truth[1], obs[0], obs[1] + x[2], "#7f8c8d");
        fig_map.draw_pie(x_truth[0], x_truth[1], (Interval(0)|obs[0]), obs[1] + x[2], "#95a5a688");
    }

    fig_map.draw_box(x.subvector(0,1), "#2c3e50[#27ae60aa]");
    fig_map.draw_vehicle(x_truth,0.5);
    fig_map.axis_limits(fig_map.view_box(), true, 0.1);

}
