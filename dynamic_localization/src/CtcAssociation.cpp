#include "CtcAssociation.h"

void CtcAssociation::contract(ibex::IntervalVector& a) {
    IntervalVector out;
    bool flag = false;
    for(auto const& box: m_map) {
        if (box.is_subset(a)) {
            if (not flag) {
                out = box;
                flag = true;
            }
            else
                out |= box;
        }
    }
    a = out;
}