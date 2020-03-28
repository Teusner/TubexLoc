#include <ibex.h>
#include <tubex.h>
#include <tubex-rob.h>
#include <tubex-3rd.h>

using namespace std;
using namespace ibex;
using namespace tubex;

int main()
{
  /* =========== CREATING DATA =========== */

    const Interval domain(0.,6.); // temporal domain

    // Truth (analytical expression)

      const TrajectoryVector state_truth(domain,
        // Lissajous function (px,py,theta):
        tubex::Function("(240*cos(t) ; \
                          120*sin(2*t) ; \
                          atan2(240*cos(2*t),-240*sin(t)))"));

    // Sets of trajectories
      const double dt = 0.01; // tube timestep

      TubeVector x(domain, dt, 2); // unbounded 2d tube vector
      x &= IntervalVector(2,Interval(-1000,1000.));

      Tube heading(state_truth[2], dt); // heading computed from truth...
      //heading.inflate(1.2); // ...with some uncertainty

      // Derivatives of positions
      TubeVector v(domain, dt,
        // Lissajous derivative
        tubex::Function("(-240*sin(t) ; \
                          240*cos(2*t))"));
      v.inflate(10.); // ...with some uncertainty

    // Sets of observations

      // Creating random map of landmarks
      const int nb_landmarks = 150;
      const IntervalVector map_area(2, Interval(-400.,400.));
      const vector<Beacon> v_map = DataLoader::generate_landmarks(map_area, nb_landmarks);
      vector<IntervalVector> v_b;
      for(const auto& landmark : v_map)
        v_b.push_back(landmark.pos().subvector(0,1));

      // Generating observations obs=(t,range,bearing) of these landmarks
      const int max_nb_obs = 50;
      const Interval visi_range(0.,75.); // [0m,75m]
      const Interval visi_angle(-M_PI/4.,M_PI/4.); // frontal sonar
      vector<IntervalVector> v_obs =
        DataLoader::generate_observations(state_truth, v_map, max_nb_obs, true, visi_range, visi_angle);

      // Adding uncertainties on the measurements
      for(auto& obs : v_obs) 
      {
        obs[1].inflate(2.); // range
        obs[2].inflate(0.1); // bearing
      }

      // Association set
      vector<IntervalVector> m(v_obs.size(), IntervalVector(2)); // unknown association
    

  /* =========== CREATING CONTRACTORS =========== */

    tubex::CtcDeriv ctc_deriv; // \dot{x}=v constraint
    tubex::CtcEval ctc_eval; // p=x(t_i) constraint

    // Add further contractors here...


  /* =========== CONTRACTOR NETWORK =========== */

    ContractorNetwork cn;

    // Add CtcDeriv here...
    // ...

    for (int i = 0 ; i < v_obs.size() ; i++) {
      // Measurement i (simple shortcuts)
      Interval &t  = v_obs[i][0]; // time
      Interval &y1 = v_obs[i][1]; // range
      Interval &y2 = v_obs[i][2]; // bearing

      // Intermediate variables:
      Interval& psi = cn.create_var(Interval()); // robot heading at t^i
      IntervalVector& p = cn.create_var(IntervalVector(2)); // robot position at t^i
      
      // Add further intermediate variables
      // ...

      // Add further contractors to the CN
      // ...

      cn.add(ctc_eval, t, p, x, v);
      cn.add(ctc_eval, t, psi, heading);
    }

    cn.contract();


  /* =========== GRAPHICS =========== */

    vibes::beginDrawing();

    VIBesFigMap fig_map("Map");
    fig_map.set_properties(1450, 50, 1000, 600);
    fig_map.add_tube(&x, "x", 0, 1);
    fig_map.add_trajectory(&state_truth, "x*", 0, 1, 2, "white");
    fig_map.add_observations(v_obs, &state_truth);
    fig_map.add_beacons(v_map, 2.);
    fig_map.smooth_tube_drawing();
    fig_map.show();
    fig_map.axis_limits(-340., 340., 0., 0., true);
    

  /* =========== ENDING =========== */

    cout << endl << v_map.size() << " landmarks" << endl;
    int identified = 0;
    for(int i = 0 ; i < m.size() ; i++)
      if(m[i].volume() == 0 && !m[i].is_empty()) // degenerate box
      {
        identified ++;
        fig_map.add_beacon(m[i], 2., "#00A53B[#00A53B]");
      }
    cout << identified << "/" << m.size() << " observations identified" << endl << endl;
    fig_map.show();
  
  vibes::endDrawing();
  // Checking if this example still works:
  bool success = x.contains(state_truth.subvector(0,1)) == YES;
  return success ? EXIT_SUCCESS : EXIT_FAILURE;
}