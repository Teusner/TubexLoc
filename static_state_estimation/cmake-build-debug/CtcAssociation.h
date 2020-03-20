#ifndef __CTCASSOCIATION_H__
#define __CTCASSOCIATION_H__


class CtcAssociation : public ibex::Ctc {
    public:
        CtcAssociation(const std::vector<ibex::IntervalVector>& map): ibex::Ctc(2), m_map(map){}
        void contract(ibex::IntervalVector& a){
            // todo...
        }
    protected:
        const std::vector<ibex::IntervalVector> m_map;
};


#endif //__CTCASSOCIATION_H__
