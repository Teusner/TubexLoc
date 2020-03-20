#include <ibex.h>
#include <tubex.h>
#include <tubex-rob.h>
#include <tubex-3rd.h>
#include <iostream>
#include "CtcAssociation.h"

using namespace std;
using namespace ibex;
using namespace tubex;

int main(int argc, char** argv)
{
  /* =========== CREATING DATA =========== */

    // Truth (unknown pose)
    Vector truth(3,0.);
    truth[2] = M_PI/6.; // heading

    // Creating random map of landmarks
    const int nb_landmarks = 1;
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
    x[0] = Interval(NEG_INFINITY, POS_INFINITY);
    x[1] = Interval(NEG_INFINITY, POS_INFINITY);
    x[2] = Interval(truth[2], truth[2]);

    // Contractor Network
    ContractorNetwork cn;
    ibex::CtcFwdBwd ctc_add(*new ibex::Function("x", "y", "z", "x-y-z"));
    ibex::CtcFwdBwd ctc_sub(*new ibex::Function("x", "y", "z", "x-y+z"));

    pyibex::CtcPolar ctc_polar;
    Interval d1, d2, alpha;

    cn.add(ctc_sub, d1, v_b[0][0], x[0]);
    cn.add(ctc_sub, d2, v_b[0][1], x[1]);
    cn.add(ctc_add, alpha, v_obs[0][1], x[2]);
    cn.add(ctc_polar, d1, d2, v_obs[0][0], alpha);
    cn.contract();

    /* =========== Association Tests ===========*/
    IntervalVector a1(2, Interval(2, 3));
    IntervalVector a2(2, Interval(5, 6));
    b[1] = Interval(4, 5);
    IntervalVector a3(2, Interval(9, 10));
    c[1] = Interval(2, 3);

    IntervalVector box (2, Interval());

    std::vector<ibex::IntervalVector> = {a1, a2, a3};
    CtcAssociation(box);

    /* =========== GRAPHICS =========== */

    vibes::beginDrawing();
    VIBesFigMap fig_map("Map");
    fig_map.set_properties(50, 50, 600, 600);
    fig_map.add_observations(v_obs, truth);
    for(const auto& iv : v_b)
        fig_map.add_beacon(Beacon(iv), 0.2);
    fig_map.draw_vehicle(truth, 1.);
    fig_map.axis_limits(map_area);
    fig_map.draw_box(x.subvector(0,1)); // estimated position (2d box)
    fig_map.show();
    
  /* =========== ENDING =========== */

    vibes::endDrawing();
    return EXIT_SUCCESS;
}