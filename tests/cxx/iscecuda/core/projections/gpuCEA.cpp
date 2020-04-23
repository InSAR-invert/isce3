#include <iostream>
#include <isce/cuda/core/gpuProjections.h>

#include "projtest.h"

isce::cuda::core::CEA testproj;

struct CEATest : public ::testing::Test {
    virtual void SetUp() {
        fails = 0;
    }
    virtual void TearDown() {
        if (fails > 0) {
            std::cerr << "CEA::TearDown sees failures" << std::endl;
        }
    }
    unsigned fails;
};

#define ceaTest(...) PROJ_TEST(CEATest, testproj, __VA_ARGS__)

ceaTest(Point1, {-1.397694375733237e+00,   8.496490909249732e-01,
          6.397636527923552e+03}, {-7.726813212349523e+06,
          5.503591184289403e+06, 6.397636527923552e+03} );

ceaTest(Point2, { -2.405089743372768e-02,  -2.121638452626837e-01,
          7.130263820491701e+03}, { -1.329595334189561e+05,
          -1.539489150010470e+06, 7.130263820491701e+03});

ceaTest(Point3, {  1.513020264912829e+00,  -1.352352581363516e-01,
          7.616334412360351e+03}, {  8.364364324888950e+06,
          -9.855597820966323e+05, 7.616334412360351e+03} );

ceaTest(Point4, { -6.510829420761222e-01,  -5.052018942343930e-01,
          2.690465869094089e+03}, { -3.599353597262635e+06,
          -3.541342346658405e+06, 2.690465869094089e+03} );

ceaTest(Point5, { -8.456729268427137e-01,   1.255355912492460e+00,
          7.246008662493211e+03}, { -4.675096972488437e+06,
          6.976944015207697e+06, 7.246008662493211e+03});

ceaTest(Point6, { -2.843083001951019e-01,  -9.069086712873742e-02,
          2.549446911384445e+03}, { -1.571729248159633e+06,
          -6.620127284244793e+05, 2.549446911384445e+03});

ceaTest(Point7, {  2.114866561120632e+00,   5.746730927033163e-01,
          3.918856209626071e+03}, {  1.169152510773354e+07,
          3.978354850505752e+06, 3.918856209626071e+03});

ceaTest(Point8, {  5.468997020381132e-01,  -5.604976553162178e-01,
          2.445910255167585e+03}, {  3.023401908819473e+06,
          -3.890648969128828e+06, 2.445910255167585e+03});

ceaTest(Point9, { -1.627303568150426e+00,   4.112474410828795e-01,
          1.956182160312234e+03}, { -8.996151754772514e+06,
          2.924045415316114e+06, 1.956182160312234e+03}  );

ceaTest(Point10, {  1.520178238348389e-01,   9.213619867413443e-01,
          1.862068559454819e+03}, {  8.403935439057967e+05,
          5.837947480418514e+06, 1.862068559454819e+03});

ceaTest(Point11, { -1.888165264116561e-01,  -7.374773626803783e-01,
          4.128365859661457e+03}, { -1.043826215743449e+06,
          -4.924960412810313e+06, 4.128365859661457e+03});

ceaTest(Point12, { -1.482868944133783e+00,  -2.915126127839418e-02,
          1.269320808274216e+03},  { -8.197680085608741e+06,
          -2.130491805252357e+05, 1.269320808274216e+03} );

ceaTest(Point13, { -1.232252006434709e+00,   1.385361875289014e+00,
         -2.329770578194399e+01}, { -6.812205335854601e+06,
         7.215253782338585e+06, -2.329770578194399e+01} );

ceaTest(Point14, {  5.371427733359488e-01,  -9.104187460566963e-01,
          3.830848656141965e+03}, {  2.969463102942626e+06,
          -5.788814966911813e+06, 3.830848656141965e+03});

ceaTest(Point15, {  2.032593025432839e+00,   1.474564625955619e+00,
          3.491994915674123e+03}, {  1.123669588782941e+07,   7.307956458783941e+06,
          3.491994915674123e+03});

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}