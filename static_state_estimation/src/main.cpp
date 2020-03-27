#include <ibex.h>
#include <tubex.h>
#include <tubex-rob.h>
#include <tubex-3rd.h>
#include <iostream>
#include "CtcAssociation.h"

/* Uncomment the following line if you want to show the result of a ctc_asso */
// #define TEST

using namespace std;
using namespace ibex;
using namespace tubex;

int main(int argc, char** argv) {
  /* =========== CREATING DATA =========== */

    // Truth (unknown pose)
    Vector truth(3,0.);
    truth[2] = M_PI/6.; // heading

    // Creating random map of landmarks
    const int nb_landmarks = 4;
    const IntervalVector map_area(2, Interval(-8.,8.));
    vector<IntervalVector> v_b = DataLoader::generate_landmarks_boxes(map_area, nb_landmarks);
    vector<IntervalVector> v_obs = DataLoader::generate_observations(truth, v_b);

    // Adding uncertainties on the measurements
    for(auto& obs : v_obs) {
      obs[0].inflate(0.5); // range
      obs[1].inflate(0.1); // bearing
    }

  /* =========== Localisation Processing =========== */
    IntervalVector x(3);
    x[2] = Interval(truth[2]);

    // Contractor Network
    ContractorNetwork cn;
    ibex::CtcFwdBwd ctc_add(*new ibex::Function("x", "y", "z", "x-y-z"));
    ibex::CtcFwdBwd ctc_sub(*new ibex::Function("x", "y", "z", "x-y+z"));
    pyibex::CtcPolar ctc_polar;
    // Interval d1, d2, alpha;

    for(int i = 0; i < nb_landmarks; i++) {
        // Intermediate variables
        Interval& d1 = cn.create_var(Interval());
        Interval& d2 = cn.create_var(Interval());
        Interval& alpha = cn.create_var(Interval());

        // Contractor Network
        cn.add(ctc_sub, d1, v_b[i][0], x[0]);
        cn.add(ctc_sub, d2, v_b[i][1], x[1]);
        cn.add(ctc_add, alpha, v_obs[i][1], x[2]);
        cn.add(ctc_polar, d1, d2, v_obs[i][0], alpha);
    }
    cn.contract();

    /* =========== Association Tests ===========*/
/* #ifdef TEST
    vector<ibex::IntervalVector> map;
    ibex::IntervalVector b1{{2,3},{2,3}};
    ibex::IntervalVector b2{{5,6},{4,5}};
    ibex::IntervalVector b3{{9,10},{2,3}};
    map.push_back(b1);
    map.push_back(b2);
    map.push_back(b3);

    ibex::IntervalVector a1{{0,4},{0,4}};
    ibex::IntervalVector a2{{3.5,11},{1,6}};

    // =========== Graphics ===========

    vibes::beginDrawing();
    VIBesFigMap fig_asso("Association");
    fig_asso.set_properties(50,50,1000,600);
    ibex::IntervalVector win_area{{-1,11},{-1, 7}};
    fig_asso.axis_limits(win_area);

    fig_asso.draw_box(b1, "black");
    fig_asso.draw_box(b2, "black");
    fig_asso.draw_box(b3, "black");

    fig_asso.draw_box(a1, "blue");
    fig_asso.draw_box(a2, "blue");

    CtcAssociation ctc_asso(map);
    ctc_asso.contract(a1);
    ctc_asso.contract(a2);

    fig_asso.draw_box(a1, "orange");
    fig_asso.draw_box(a2, "orange");
    fig_asso.show();
#endif */

    /* =========== GRAPHICS =========== */

    vibes::beginDrawing();
    VIBesFigMap fig_map("Map");
    fig_map.set_properties(50, 50, 600, 600);
    fig_map.add_observations(v_obs, truth);
    for(const auto& iv : v_b)
        fig_map.add_beacon(Beacon(iv), 0.2);
    fig_map.axis_limits(map_area);
    fig_map.draw_box(x.subvector(0,1), "#2980b9[#ECF0F1]"); // estimated position (2d box)
    fig_map.draw_vehicle(truth, 1.);
    fig_map.show();
    vibes::endDrawing();
    
  /* =========== ENDING =========== */

    return EXIT_SUCCESS;
}