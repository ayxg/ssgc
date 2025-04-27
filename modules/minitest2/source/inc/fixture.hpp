#pragma once

namespace minitest {
/// Base class for all fixtures.
///
/// Inherit from this class to create a fixture.
/// All members of child class must be public or protected
/// If you wish to access them in your test cases.
struct Fixture {
  void virtual SetUp() { /* Override pre-test actions. */ }
  void virtual TearDown() { /* Override post-test actions. */ }
  void virtual Run() { /* Override test actions. */ }
  virtual ~Fixture() = default;
};
}  // namespace minitest
