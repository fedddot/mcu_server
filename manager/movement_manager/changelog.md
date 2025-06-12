# Changelog

## 2025-06-12

### Features & Implementations
- Implemented `CircularMovement` class in `manager/movement_manager/include/circular_movement.hpp` for generating axis steps along a circular arc defined by a destination and rotation center.
- Added `circular_movement` method to `MovementManager` to support circular movement using the new `CircularMovement` class.
- Created unit tests for `CircularMovement` in `manager/movement_manager/tests/src/ut_circular_movement.cpp`.
- Provided CMake integration instructions for the new test target.

### Refactoring & Structure
- Provided guidance for moving and implementing file-based request readers as template classes.
- Added/updated documentation and error handling in several new and existing classes.

### Testing
- Added unit tests for both full-circle and partial-arc scenarios for `CircularMovement`.

### Build System
- Provided CMake configuration snippets for new test targets and interface libraries.

---
This changelog summarizes the main changes and additions made to the movement_manager module, including new features, refactoring, and testing support.
