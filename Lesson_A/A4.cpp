#include <tubex.h>
#include <vibes.h>
#include <math.h>

using namespace tubex;

int main() {

	IntervalVector x ({{0, 0}, {0, 0}});
	IntervalVector b ({{3, 4}, {2, 3}});
	IntervalVector c = cart_prod(x, b);

	Function g("x[4]", "sqrt((x[0] - x[2])^2 + (x[1] - x[3])^2)");
	Interval y = g.eval(c);

	std::cout << y << std::endl;

	vibes::beginDrawing();
	VIBesFigMap fig1("Landmark Detection");
	fig1.set_properties(50, 50, 400, 400);
	fig1.axis_limits(IntervalVector({{-6,6},{-6,6}}));

	fig1.draw_circle(0., 0., y.ub(), "#2c3e50[#3498db]");
	fig1.draw_circle(0., 0., y.lb(), "#2c3e50[white]");
	fig1.draw_box(b, "#2c3e50[#e74c3c]");
	fig1.draw_box(x, "#2c3e50[#34495e]");

	fig1.show();

	x.inflate(0.1, 0.1);

	VIBesFigMap fig2("Landmark Detection 2");
	fig2.set_properties(50, 50, 400, 400);
	fig2.axis_limits(IntervalVector({{-6,6},{-6,6}}));

	fig2.draw_circle(0., 0., y.ub(), "#2c3e50[#3498db]");
	fig2.draw_circle(0., 0., y.lb(), "#2c3e50[white]");
	fig2.draw_box(b, "#2c3e50[#e74c3c]");
	fig2.draw_box(x, "#2c3e50[#34495e]");

	fig2.show();
	vibes::endDrawing();

}