#include "my_ctc.hpp"

void My_ctc::contract(ibex::IntervalVector& a) {
    IntervalVector out = IntervalVector::empty(2);
    for(auto const& box: m_map)
        out |= (a & box);
    a = out;
}