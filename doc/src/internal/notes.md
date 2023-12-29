# Notes

## Pokit Bluetooth API

See also: [Pokit API Errata](pokit.html)

### Undocumented Services

Pokit Meter and Pokit Pro both include the following service(s) with no official documentation to match:

* `1d14d6ee-fd63-4fa1-bfa4-8f47b42119f0` - appears to be a Silicon Labs (formerly BlueGiga) OTA Service
  for updating firmware, which would make sense.

Additionally, the Pokit Meter includes the following undocumented service:

* `831d0001-e98e-1fed-e73e-4bb7596f776e` - no idea.

## Qt Versions

QtPokit requires Qt's support Bluetooth Low Energy, in Central mode.

|  Qt | Supported| Notes                                    |
|-----|:--------:|------------------------------------------|
| 5.0 | No       | Qt BLE API was [not added until v5.4].   |
| 5.1 | No       | Qt BLE API was [not added until v5.4].   |
| 5.2 | No       | Qt BLE API was [not added until v5.4].   |
| 5.3 | No       | Qt BLE API was [not added until v5.4].   |
| 5.4 | Probably |                                          |
| 5.5 | Probably |                                          |
| 5.6 | Probably |                                          |
| 5.7 | Probably |                                          |
| 5.8 | Probably |                                          |
| 5.9 | Probably |                                          |
| 5.10| Probably |                                          |
| 5.11| Probably |                                          |
| 5.12| Probably |                                          |
| 5.13| Probably |                                          |
| 5.14| Probably |                                          |
| 5.15| Yes      |                                          |
| 6.0 | No       | Bluetooth module [ported to Qt6 in v6.2].|
| 6.1 | No       | Bluetooth module [ported to Qt6 in v6.2].|
| 6.2 | Yes      |                                          |
| 6.3 | Yes      |                                          |

## CMake Versions

In summary (of the below table), for *building* QtPokit:

* CMake 2.8.9 through to pre-3.0 *should* be possible, but the effort and resulting complexity
  is high enough to not be worth it.
* any CMake 3.x should be fine, except:
* CMake 3.12+ is needed for the full set of unit tests.

| Version| Dependency                                                                           |
|:------:|--------------------------------------------------------------------------------------|
| 2.8.9  | [Qt 5.1+](https://github.com/qt/qtbase/blob/v5.1.0/src/corelib/Qt5Config.cmake.in) |
| 2.8.11 | [`target_include_directories`](https://cmake.org/cmake/help/v2.8.11/cmake.html#command:target_include_directories) |
| 2.8.11 | Has `-fPIC` issues with Qt. |
| 2.8.12 | [`add_compile_options`](https://cmake.org/cmake/help/v2.8.12/cmake.html#command:add_compile_options)
| 2.8.12 | Has compile isssues with Qt. |
|  3.0   | `VERSION` option of the `project()` command.
|  3.1   | [Qt 5.11+](https://github.com/qt/qtbase/blob/v5.11.0/src/corelib/Qt5Config.cmake.in) |
|        | `target_sources` command. |
|  3.12  | [Object Libraries] for sharing binary objects between the console app and unit tests.|
|  3.13  | `-B` CLI option (handy, but not essential).                                          |
|  3.16  | [Qt6.x shared](https://github.com/qt/qtbase/blob/v6.2.0/.cmake.conf)|
|  3.18  | Not actually a requirement, but this is the minimum version that's regularly tested. |
|  3.21  | [Qt6.x static](https://github.com/qt/qtbase/blob/v6.2.0/.cmake.conf)|

[not added until v5.4]: https://doc.qt.io/qt-5/qtbluetooth-le-overview.html
[Object Libraries]: https://cmake.org/cmake/help/latest/command/add_library.html#object-libraries
[ported to Qt6 in v6.2]: https://www.qt.io/blog/qt-6.2-lts-released
