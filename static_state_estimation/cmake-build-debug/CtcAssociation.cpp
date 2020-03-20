#include "CtcAssociation.h"

void CtcAssociation::contract(ibex::IntervalVector& a) {
    double m_top, m_left, m_bottom, m_right;
    for(auto const& box: m_map) {
        if (box.is_subset(a)) {
            m_left = std::min(box[0].lb(), m_left);
            m_right = std::max(box[0].ub(), m_right);
            m_bottom = std::min(box[1].lb(), m_bottom);
            m_top = std::max(box[1].ub(), m_top);
        }
    }
    a[0].lb() = m_left;
    a[0].ub() = m_right;
    a[1].lb() = m_bottom;
    a[1].ub() = m_top;
}