//CATCH_CONFIG_RUNNER tells the catch library that this
//project will now explicitly call for the tests to be run.
#define CATCH_CONFIG_RUNNER

#include <catch2/catch.hpp>
#include <QCoreApplication>

int main( int argc, char* argv[] )
{
   QCoreApplication application(argc, argv);

   const int result = Catch::Session().run(argc, argv);

   return result;
}
