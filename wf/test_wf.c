#include "utest.h"
#include "wf.h"

UTEST(wf, version) {
    ASSERT_EQ(1, wf_version());
}

UTEST_MAIN()
