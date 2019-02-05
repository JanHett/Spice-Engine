#include <gtest/gtest.h>

#include "../../src/graph/node.h"

TEST(Loader, ConstructWithPathPMB) {
    Loader l("MyLoader", "./test.pbm");
}

TEST(Loader, OpenPMB) {
    Loader l("MyLoader");
    l.open("./test.pbm");
}
