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
    ibex::Function loc("x1", "x2", "b1", "b2", "sqrt((x1-b1)^2 + (x2 - b2)^2)");
    IntervalVector c(4);
    c[0] = Interval(0, 0);
    c[1] = Interval(0, 0);
    c[2] = Interval(3, 4);
    c[3] = Interval(3, 4);
    Interval position = loc.eval(c);

    IntervalVector box{{3, 4}, {3, 4}};

    vibes::beginDrawing();
    VIBesFigMap fig("Map");
    fig.set_properties(50, 50, 400, 400);
    fig.draw_box(box);
    fig.draw_circle(0, 0, position.lb());
    fig.draw_circle(0, 0, position.ub());
    fig.show();
    vibes::endDrawing();
}