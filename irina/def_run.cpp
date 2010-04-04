// INTELIB_CONTRIBUTED
// Written by Igor Bronstein (<igor.bronstein_AT_intelib.org>)

#include "refal/refal.hpp"
#include "refal/rvars.hpp"
#include "refal/rclause.hpp"

#include "def_part.hxx"



RfFormConstructor R;

int main(int argc, char **argv) {
    RefalInit(argc, argv);
    RfReference res = (R|R<Go>R);
    res.Evaluate();
    RefalFree();
}

