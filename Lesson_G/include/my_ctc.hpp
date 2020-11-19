#ifndef __MY_CTC_HPP__
#define __MY_CTC_HPP__

#include <ibex.h>
#include <tubex.h>

class My_ctc: public ibex::Ctc {
    public:
        My_ctc(const std::vector<ibex::IntervalVector>& map): ibex::Ctc(2), m_map(map){}
        void contract(ibex::IntervalVector& a);

    protected:
        const std::vector<ibex::IntervalVector> m_map;
};

#endif //__MY_CTC_HPP__