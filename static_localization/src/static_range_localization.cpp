#include <ibex.h>
#include <tubex.h>
#include <tubex-rob.h>
#include <tubex-3rd.h>
#include <iostream>

using namespace std;
using namespace ibex;
using namespace tubex;

int main() {
    /* =========== INITIALISATION =========== */
    IntervalVector x(2);

    // Truth (unknown pose)
    Vector truth(3.0);
    truth[2] = M_PI/6.;

    // Creating random map of landmarks
    const int nb_landmarks = 3;
    const IntervalVector map_area(2, Interval(-8.,8.));
    vector<IntervalVector> v_b = DataLoader::generate_landmarks_boxes(map_area, nb_landmarks);

    // Generating range-only observations of these landmarks
    vector<Interval> v_range(v_b.size());
    for(int i = 0 ; i < v_b.size() ; i++) {
        v_range[i] = sqrt(pow(truth[0]-v_b[i][0],2) + pow(truth[0]-v_b[i][1],2));
        v_range[i].inflate(0.1); // adding uncertainties
    }

    /* =========== PROCESSING =========== */
    ibex::CtcFwdBwd ctc_dist(*new ibex::Function("x1", "x2", "b1", "b2", "r", "sqrt((x1-b1)^2+(x2-b2)^2)-r"));
    ContractorNetwork cn;
    for(int i = 0 ; i < v_b.size() ; i++) {
        cn.add(ctc_dist, x[0], x[1], v_b[i][0], v_b[i][1], v_range[i]);
    }
    cn.contract();

    /* =========== GRAPHICS ===========*/
    vibes::beginDrawing();
    VIBesFigMap fig_map("Map");
    fig_map.set_properties(50, 50, 600, 600);
    for(const auto& iv : v_b)
        fig_map.add_beacon(Beacon(iv), 0.2);
    for(int i = 0 ; i < v_range.size() ; i++) {
        fig_map.draw_circle(v_b[i][0].mid(), v_b[i][1].mid(), v_range[i].lb(), "gray");
        fig_map.draw_circle(v_b[i][0].mid(), v_b[i][1].mid(), v_range[i].ub(), "gray");
    }
    fig_map.draw_box(x.subvector(0,1)); // estimated position
    fig_map.draw_vehicle(truth, 1.);
    fig_map.show();
    vibes::endDrawing();
}