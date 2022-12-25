#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TextOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestRunner.h>
#include <cppunit/TestResultCollector.h>


int add(int a, int b)
{
    return a + b;
}

class AddTest : public CppUnit::TestFixture {
private:
    CPPUNIT_TEST_SUITE(AddTest);
    CPPUNIT_TEST(runAddTest);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp() {}

    void tearDown() {}

    void runAddTest() {
        CPPUNIT_ASSERT(5 == add(2, 3));
        CPPUNIT_ASSERT(20 == add(10, 10));
    }

};

CPPUNIT_TEST_SUITE_REGISTRATION(AddTest);

// Don't need to touch main, it's ok
int main(int argc, char** argv)
{
    // Create the event manager and test controller
    CppUnit::TestResult controller;
    // Add a listener that colllects test result
    CppUnit::TestResultCollector result;
    controller.addListener(&result);
    // Add a listener that print dots as test run.
    CppUnit::BriefTestProgressListener progress;
    controller.addListener(&progress);

    CppUnit::TextUi::TestRunner runner;
    CppUnit::TestFactoryRegistry& registry = CppUnit::TestFactoryRegistry::getRegistry();
    runner.addTest(registry.makeTest());
    std::cout << std::endl << " === Running Tests " << std::endl;
    runner.run(controller);
    // Print test in a compiler compatible format.
    std::cout << std::endl << " === Test Results" << std::endl;
    CppUnit::CompilerOutputter outputter(&result, CppUnit::stdCOut());
    //CppUnit::TextOutputter outputter( &result, CppUnit::stdCOut() );
    outputter.write();

    return result.wasSuccessful() ? 0 : 1;
}