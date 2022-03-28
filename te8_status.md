# Test Event 8 Status

Test Case ID | AmebaZ2 | AmebaD | Comments |
--- | --- | --- | --- |
TC-SC-4.1 | Pass | Pass | - |
TC-SC-4.3 | Pass | Pass | - |
TC-SC-4.7 | - | - | No instructions |
TC-SC-4.8 | - | - | No instructions |
TC-SC-4.9 | Pass | Pass | - |
TC-SC-4.10 | Pass | Pass | - |
TC-IDM-1.2 | Pass | Pass | - |
TC-IDM-2.2 | Fail | Fail | Known issue: https://github.com/project-chip/connectedhomeip/issues/15812 |
TC-IDM-3.2 | Fail | Fail | Known issue: https://github.com/project-chip/connectedhomeip/issues/15999, not in v1.0 specs |
TC-IDM-5.2 | - | - | Marked as not testable |
TC-IDM-7.1 | - | - | No instructions |
TC-MF-1.3 | Pass | Pass | - |
TC-MF-1.4 | Pass | Pass | - |
TC-MF-1.5 | Pass | Pass | - |
TC-MF-1.6 | Pass | Pass | - |
TC-MF-1.9 | Fail | Fail | Known issue: https://github.com/CHIP-Specifications/chip-test-plans/issues/924 |
TC-MF-1.10 | Pass | Pass | - |
TC-MF-1.13 | Pass | Pass | - |
TC-MF-1.15 | Pass | Pass | - |
TC-MF-1.17 | Pass | Pass | - |
TC-MF-1.18 | Pass | Pass | - |
TC-MF-1.21 | Pass | Pass | Need reduce CHIP_CONFIG_MAX_FABRICS to 3 |
TC-MF-1.22 | Pass | Pass | Need reduce CHIP_CONFIG_MAX_FABRICS to 3 |
TC-MF-1.25 | Pass | Pass | - |
TC-MF-1.26 | Pass | Pass | - |
TC-DD-1.1 | Pass | Pass | - |
TC-DD-1.2 | Pass | Pass | - |
TC-DD-1.3 | - | - | NFC not supported |
TC-DD-1.5 | - | - | NFC not supported |
TC-DD-1.6 | Pass | Pass | - |
TC-DD-1.7 | Pass | Pass | - |
TC-DD-2.1 | Pass | Pass | No clear instructions, uploaded NRFConnect screenshots |
TC-DD-3.1 | Pass | Pass | - |
TC-DD-3.2 | Pass | Pass | - |
TC-DD-3.3 | Pass | Pass | User Directed Commissioning not supported |
TC-DD-3.9 | Pass | Pass | Custom Flow Commissioning not supported |
TC-DM-1.1 | Pass | Pass | - |
TC-DM-2.1 | Pass | Pass | No instructions |
TC-DM-2.2 | Pass | Pass | No instructions |
TC-DM-3.1 | Pass | Pass | - |
TC-DM-3.2 | Pass | Pass | - |
TC-BDX-1.2 | Pass | Pass | - |
TC-BDX-2.2 | Pass | Pass | - |
TC-BOOL-1.1 | Pass | Pass | - |
TC-BOOL-2.1 | Pass | Pass | - |
TC-BOOL-2.2 | - | - | Manual operation not supported |
TC-DESC-1.1 | Pass | Pass | - |
TC-GC-1.1 | Pass | Pass | - |
TC-GC-1.2 | Pass | Pass | - |
TC-GC-1.3 | Fail | Fail | Known issue, fix is merged after TE8/RC3: https://github.com/project-chip/connectedhomeip/pull/15309 |
TC-GENDIAG-1.1 | Pass | Pass | - |
TC-GENDIAG-2.1 | Pass | Pass | - |
TC-I-1.1 | Fail | Fail | Known issue: https://github.com/project-chip/connectedhomeip/issues/14825 |
TC-I_2.1 | Pass | Pass | - |
TC-I_2.2 | Pass | Pass | - |
TC-I_2.3 | Pass | Pass | - |
TC-MOD-1.1 | Pass | Pass | - |
TC-MOD-1.2 | Pass | Pass | - |
TC-MOD-2.1 | Pass | Pass | - |
TC-MOD-3.1 | - | - | Marked as not testable |
TC-MOD-3.2 | Pass | Pass | - |
TC-MOD-3.3 | - | - | Marked as not testable |
TC-NC-1.1 | Pass | Pass | - |
TC-NC-1.2 | - | - | Not thread device |
TC-NC-1.3 | - | - | Not ethernet device |
TC-NC-1.16 | Pass | Pass | - |
TC-NC-1.17 | - | - | Not thread device |
TC-NC-1.18 | - | - | Not ethernet device |
TC-OO-1.1 | Pass | Pass | - |
TC-OO-2.1 | Pass | Pass | - |
TC-OO-2.2 | Pass | Pass | Manual operation step not supported |
TC-OO-2.3 | Pass | Pass | - |
TC-OO-2.4 | Fail | Fail | Known issue, merged after TE8: https://github.com/project-chip/connectedhomeip/pull/16377|
TC-SU-2.1 | Pass | Pass | - |
TC-SU-2.2 | - | - | Mark as not testable, DUT did not send another image query after 600 seconds, merged after TE8/RC3: https://github.com/project-chip/connectedhomeip/pull/15433. User consent set but did not trigger image transfer, merged after TE8: https://github.com/project-chip/connectedhomeip/pull/15706 |
TC-SU-2.3 | - | - | Mark as not testable, user consent set but did not trigger image transfer, merged after TE8/RC3: https://github.com/project-chip/connectedhomeip/pull/15706 |
TC-SU-2.4 | Pass | Pass | - |
TC-SU-2.5 | Pass | Pass | - |
TC-SU-2.6 | Pass | Pass | Current method is to add 1 to the softwareversion stored in DCT. This will be updated after TE8 |
TC-SU-2.7 | - | - | Mark as not testable, events not implemented in TE8 |
TC-SU-2.8 | - | - | No instructions |
TC-SWDIAG-1.1 | Pass | Pass | - |
TC-SWDIAG-2.1 | Pass | Pass | - |
TC-SWDIAG-3.1 | Fail | Fail | Known issue: https://github.com/project-chip/connectedhomeip/issues/14590 |
TC-SWITCH-1.1 | Pass | Pass | - |
TC-SWITCH-2.1 | Pass | Pass | - |
TC-SWITCH-2.2 | Pass | Pass | Step with manual operation not supported |
TC-WIFIDIAG-1.1 | Pass | Pass | - |
TC-WIFIDIAG-3.1 | Pass | Pass | - |
