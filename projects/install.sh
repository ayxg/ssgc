# Basic pattern for installing a subproject from the monorepo.
#   cmake -B projects/$PACKAGE_NAME --log-level=TRACE
#   cmake --build release/$PACKAGE_NAME -v
#   cmake --install release/$PACKAGE_NAME -v

# Install ssgmono_box from ssgc monorepo.
cmake -S projects/ssgmono_box -B release/ssgmono_box --log-level=TRACE
cmake --install release/ssgmono_box -v

# Install minitest_box from ssgc monorepo.
cmake -S projects/minitest_box -B release/minitest_box --log-level=TRACE
cmake --build release/minitest_box -v
cmake --install release/minitest_box -v