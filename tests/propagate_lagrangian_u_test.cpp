#include <iostream>
#include <iomanip>
#include <boost/lexical_cast.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/random.hpp>

#include "../src/keplerian_toolbox.h"

using namespace std;
using namespace kep_toolbox;
int main() {
    // Preamble
    array3D r0,v0,r0_cp,v0_cp;
    double tof;
    boost::mt19937 rng;
    boost::uniform_int<> dist(0, 1);
    boost::variate_generator<boost::mt19937&, boost::uniform_int<> > rand_bit(rng, dist);
    boost::uniform_real<> dist1(-2,2);
    boost::variate_generator<boost::mt19937&, boost::uniform_real<> > drng(rng, dist1);
    double acc=0,err_max=0,err=0;
    int count=0;

    // Experiment Settings
    unsigned int Ntrials = 50000;

    // Start Experiment
    for (unsigned int i = 0; i<Ntrials; ++i){
        //1 - generate a random propagation set-up
        r0[0] = drng() * 2; r0[1] = drng() * 2; r0[2] = drng() * 2;
        v0[0] = drng() * 2; v0[1] = drng() * 2; v0[2] = drng() * 2;
        tof = drng() * 20;
        r0_cp = r0;
        v0_cp = v0;
        //2 - propagate back and forth
	propagate_lagrangian_u(r0,v0,tof,1.0);
	propagate_lagrangian_u(r0,v0,-tof,1.0);
        diff(r0_cp,r0,r0_cp);
        err = norm(r0_cp);
        err_max = std::max(err_max,err);
        acc += err;
        count ++;
    }
    std::cout << "Max error: " << err_max << std::endl;
    std::cout << "Average Error: " << acc / count << std::endl;
    std::cout << "Number of Propagations Made: " << count << std::endl;
	if (err_max < 1e-7) {
		return 0;
	} else {
		return 1;
	}

}