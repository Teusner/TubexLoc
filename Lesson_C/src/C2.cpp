#include <tubex.h>
#include <tubex-pyibex.h>
#include <vibes.h>
#include <math.h>

using namespace std;
using namespace tubex;

int main() {
	// Truth (unknown pose)
	Vector x_truth({2.,1.,M_PI/6.}); // (x,y,heading)
	Vector y_truth({6,M_PI/6});
	Vector m_truth({5,6.2});

	// Bounded state
	IntervalVector x(3);
	IntervalVector y(y_truth);
	IntervalVector m = m_truth;

    x[2] = Interval(x_truth[2]);
	y[0].inflate(0.3);
	y[1].inflate(0.1);
	m.inflate(0.2);

	// Intermediate variables
	Interval theta;
	IntervalVector d(2);

	// Functions
	CtcFunction ctc_add(Function("x1", "x2", "x3", "x1+x2-x3"));
	CtcFunction ctc_sub(Function("x1", "x2", "x3", "x1-x2-x3"));

	// Contractor Network
	ContractorNetwork c;
	c.add(ctc_add, {y[1], x[2], theta});
	c.add(ctc_sub, {m[0], x[0], d[0]});
	c.add(ctc_sub, {m[1], x[1], d[1]});
	c.add(ctc::polar, {d, y[0], theta});

	c.contract();

	vibes::beginDrawing();

	VIBesFigMap fig_map("Map");
	fig_map.set_properties(100,100,500,500);
	fig_map.axis_limits(0,7,0,7);
	fig_map.draw_vehicle(x_truth, 1);
	fig_map.draw_box(m, "#d35400[#e67e22cc]");
	fig_map.draw_box(x.subvector(0,1));
	fig_map.draw_pie(x_truth[0],x_truth[1],(Interval(0.1)|y[0]),y[1]+x_truth[2],"lightGray");
  	fig_map.draw_pie(x_truth[0],x_truth[1],y[0],y[1]+x_truth[2]);
	fig_map.show();

	vibes::endDrawing();
}
