#include <tubex.h>
#include <math.h>
#include <vibes.h>
#include <tubex-rob.h>
#include <ibex.h>

using namespace std;
using namespace ibex;
using namespace tubex;

int main() {
    // Localisation
    Interval al(0, 0);
    Interval bl(3, 4);
    ibex::Function loc("x1", "x2", "b1", "b2", "r", "sqrt((x1-b1)^2+(x2-b2)^2)-r");
    IntervalVector c(4);
    c[0] = Interval(0, 0);
    c[1] = Interval(0, 0);
    c[2] = Interval(3, 4);
    c[3] = Interval(3, 4);
    Interval position = loc.eval(c);

    IntervalVector box{{3, 4}, {3, 4}};

    // Graphic
    vibes::beginDrawing();
    VIBesFigMap fig("Map Loc");
    fig.set_properties(50, 50, 400, 400);
    fig.draw_box(box);
    fig.draw_circle(0, 0, position.lb());
    fig.draw_circle(0, 0, position.ub());

    // Contractors
    ibex::CtcFwdBwd ctc_add(*new ibex::Function("x", "y", "a", "x+y-a"));
    ibex::CtcFwdBwd ctc_dist(*new ibex::Function("x1", "x2", "b1", "b2", "r", "sqrt((x1-b1)^2+(x2-b2)^2)-r"));

    IntervalVector x(2, Interval(0, 0));
    IntervalVector b1(2, Interval());
    IntervalVector b2(2, Interval(6, 9));
    IntervalVector b3(2, Interval(7.2, 7.8));
    IntervalVector b4(2, Interval(7.5, 12));
    IntervalVector b5(2, Interval(-1, 1));
    b5[1] = Interval(7, 10);
    Interval r(7, 8);



    VIBesFigMap fig1("Ctc_dist");
    fig1.set_properties(50, 50, 400, 400);
    //fig1.draw_box(b1, "blue");
    fig1.draw_box(b2, "blue");
    //.draw_box(b3, "blue");
    //fig1.draw_box(b4, "blue");
    //fig1.draw_box(b5, "blue");

//    ContractorNetwork c1;
//    c1.add(ctc_dist, x[0], x[1], b1[0], b1[1], r);
//    c1.contract();
//    cout << b1 << endl;
    ContractorNetwork c2;
    c2.add(ctc_dist, x[0], x[1], b2[0], b2[1], r);
    c2.contract();
//    ContractorNetwork c3;
//    c3.add(ctc_dist, x[0], x[1], b3[0], b3[1], r);
//    c3.contract();
//    ContractorNetwork c4;
//    c4.add(ctc_dist, x[0], x[1], b4[0], b4[1], r);
//    c4.contract();
//    ContractorNetwork c5;
//    c5.add(ctc_dist, x[0], x[1], b5[0], b5[1], r);
//    c5.contract();

    cout << b2 << endl;

    //fig1.draw_box(b1, "black");
    fig1.draw_box(b2, "black");
    //fig1.draw_box(b3, "black");
    //fig1.draw_box(b4, "black");
    //fig1.draw_box(b5, "black");

    fig1.show();
    vibes::endDrawing();

}