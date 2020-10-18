#include <tubex.h>
#include <vibes.h>
#include <math.h>
#include <my_ctc.hpp>

using namespace std;
using namespace tubex;

int main() {

	vector<IntervalVector> M;
	M.push_back(Vector({1.5,2.5}));
	M.push_back(Vector({3,1}));
	M.push_back(Vector({2,2}));
	M.push_back(Vector({2.5,3}));
	M.push_back(Vector({3.5,2}));
	M.push_back(Vector({4,1}));
	M.push_back(Vector({1.5,0.5}));

	for(auto& Mi : M)
		Mi.inflate(0.05);

	IntervalVector a1({{1.25,3},{1.6,2.75}});
	IntervalVector a2({{2,3.5},{0.6,1.2}});
	IntervalVector a3({{1.1,3.25},{0.2,1.4}});

	My_ctc ctc_constell(M);
	ContractorNetwork cn;

	cn.add(ctc_constell, {a1});
	cn.add(ctc_constell, {a2});
	cn.add(ctc_constell, {a3});

	vibes::beginDrawing();

	VIBesFigMap fig_map("Map");
	fig_map.set_properties(100,100,500,500);
	for(const auto& Mi : M)
		fig_map.draw_box(Mi, "red[orange]");
	fig_map.draw_box(a1,"#27ae60");
	fig_map.draw_box(a2,"#2980b9");
	fig_map.draw_box(a3,"#8e44ad");

	cn.contract();

	fig_map.draw_box(a1,"#27ae60[#2ecc7199]");
	fig_map.draw_box(a2,"#2980b9[#3498db99]");
	fig_map.draw_box(a3,"#8e44ad[#9b59b699]");

	fig_map.show();

}
