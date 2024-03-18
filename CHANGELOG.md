# Change Log

## [0.2.1] - 2024-xx-xx
### Added
- add_random method to python-sdk
### Fixed
- QCirc::add_random function - bug of random gate generation

## [0.2.0] - 2024-03-14
### Added
- python-sdk
### Changed
- project name: sharq -> sharqit
- command name: sharq -> sharqit
- library name: libsharq.so -> libshrqt.so
- insall location of header files: ~/include/ -> ~/include/sharqit/
- Optimizer::execute -> Optimizer::reduce_gates

## [0.1.1] - 2024-03-01
### Added
- tutorial (japanese)
- doxygen documents

## [0.1.0] - 2024-02-17
### Added
- rule based gate reduction and rotation gate merging using phase polynomials

## [0.0.5] - 2023-12-23
### Changed
- improve the T-count, 2Q-count, Gate-count reduction
- refactoring

## [0.0.4] - 2023-12-17
### Changed
- improve the processing time for optimization
- add CZ gate as a basic gate

## [0.0.3] - 2023-11-30
### Added
- sharq command - to execute optimization, random circuit generation, print stats, show circuit diagram etc

## [0.0.2] - 2023-11-24
### Changed
- QCirc::stats - to store every gate count, total gate count, and processing time
### Added
- QCirc::depth - to get depth of the quantum circuit
### Fixed
- QCirc::gate_cancel - bug where the first gate is not canceled

## [0.0.1] - 2023-11-20
- initial version
