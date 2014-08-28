#ifndef BLDG_GENERATOR_PARA_HPP
#define BLDG_GENERATOR_PARA_HPP

#include <rjmcmc/param/parameters_inc.hpp>

template<typename T>
void initialize_parameters(T* p)
{
    p-> caption("Building generation parameters");
    p-> template insert<double>("temp",'t',100,"Initial Temperature");
    p-> template insert<double>("deccoef",'C',0.999999,"Decrease coefficient");
    p-> template insert<int>("nbiter",'I',3000000,"Number of iterations");
//    p->template insert<double>("qtemp",'q',0.5,"Sampler (q) [0;1]");
    p-> template insert<int>("nbdump",'d',3000000,"Number of iterations between each result display");
//    p->template insert<bool>("dosave",'b',false, "Save intermediate results");
    p-> template insert<int>("nbsave_energy",'s',100,"Number of iterations between each save of energy");
    p-> template insert<int>("nbsave_geom",'s',30000,"Number of iterations between each save of geometry");
    p-> template insert<double>("poisson",'p',200, "Poisson processus parameter");
    p-> template insert<double>("pbirth",'B',0.8, "Birth probability");
    p-> template insert<double>("pdeath",'D',0.4, "Death probability");

    p-> template insert<double>("erej",'r',100, "rejection energy");
    p-> template insert<double>("wlcr",'e',20, "lcr weight");
    p-> template insert<double>("wfar",'o',50, "far weight");
    p-> template insert<double>("wdborder",'d',30,"border distance weight");
    p-> template insert<double>("wdpair",'p',20, "binary distance weight");
    p-> template insert<double>("whdiff",'f',20,"binary height difference weight");
}

#endif // BLDG_GENERATOR_PARA_HPP