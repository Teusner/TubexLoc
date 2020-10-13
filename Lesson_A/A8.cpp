#include <tubex.h>
#include <vibes.h>
#include <math.h>

using namespace tubex;

int main() {

	IntervalVector x ({{0, 0}, {0, 0}});
	IntervalVector b1 ({{1.5, 2.5}, {4, 11}});
	IntervalVector b2 ({{3, 4}, {4, 6.5}});
	IntervalVector b3 ({{5, 7}, {5.5, 8}});

	Interval r(7, 8);

	CtcFunction ctc_dist(Function("x[2]", "b[2]", "r", "sqrt((x[0]-b[0])^2+(x[1]-b[1])^2)-r"));

	vibes::beginDrawing();
	VIBesFigMap fig1("Landmark Detection");
	fig1.set_properties(50, 50, 400, 400);
	fig1.axis_limits(IntervalVector({{-6,6},{-6,6}}));

	fig1.draw_circle(0., 0., r.ub(), "#bdc3c7");
	fig1.draw_circle(0., 0., r.lb(), "#bdc3c7");

	fig1.draw_box(b1, "#7f8c8d");
	fig1.draw_box(b2, "#7f8c8d");
	fig1.draw_box(b3, "#7f8c8d");

	ContractorNetwork c;
    c.add(ctc_dist, {x, b1, r});
	c.add(ctc_dist, {x, b2, r});
	c.add(ctc_dist, {x, b3, r});
    c.contract();

	fig1.draw_circle(0., 0., r.ub(), "#2c3e50");
	fig1.draw_circle(0., 0., r.lb(), "#2c3e50");

	fig1.draw_box(b1, "#2c3e50[#e74c3caa]");
	fig1.draw_box(b2, "#2c3e50[#e74c3caa]");
	fig1.draw_box(b3, "#2c3e50[#e74c3caa]");

	fig1.show();
	vibes::endDrawing();

}