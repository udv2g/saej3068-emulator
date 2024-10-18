Introduction
---
This project emulates the LIN-CP communications between an EV and SE (EVSE) for the purposes of development.  It uses POSIX shared memory objects to mimic the function of the LIN stack.  The bus master executable directly copies data between the memory maps of the EV and SE executables to mimic the function of the LIN stack.  The SE executable sets the LIN schedule via another shared memory object with the bus master.  The bus timing is not accurately modeled, the entire schedule is copied at once at approximately the schedule repetition rate.  Only the LIN flags used by the application code (the frame flags) are implemented.  Unplug/replug can be simulated by stopping and restarting the bus executable.

It is based on the [reference implementation](https://github.com/udv2g/saej3068-ref) and, therefore, implements both the EV and SE in the same code base and the application code supports two connectors/inlets on each side, but the emulator only supports one (channel A).

It has been updated with the data definitions from SAE J3068/1 and SAE J3068/2 including an updated LDF file with finalized definitions.

---
Usage
---
The project has been tested on Linux, WSL, and OSX.  To build, simply run `make`.  Once, built, it can be run by starting each of the three executables in a separate terminal with `./ev`, `./se`, and `./bus`.  bus must be run last, or it will fail to open the shared memory objects created by the other processes.

Serial debug console output (`PrintConsoleXX()`) is shown in <span style="color:#ffff5f;background-color:black">yellow</span> and serial command console output (`PrintCmdXX()`) is shown in <span style="color:#5fff00;background-color:black">green</span>.  Debug and command can be sent to the same or different serial ports in the main project.  Uncolored text is output from development statements (`printf()`).

Bus logs in both a human-readable and Vector ASCII log format are generated by default.  They can be suppressed by removing the `GENERATE_TEXT_LOG` and `GENERATE_ASC_LOG` macros from `lincp-emu.h` respectively.  The Vector ASCII log file generated is fictitious, and reports frame timing that is plausible, but unrelated to the timing in the emulator.

---
Octave-based code generator
---
Included is a code generator written in [Octave](https://octave.org) script (open-source Matlab, the script uses constructs not supported by Matlab) to convert a template file and an annotated description of the SAE J3068/1 data fields (`field table pub.ods`) to generate the multiplexing code (`ids_handler.c`).  See below for usage.

---
Vector CAPL parsing file
---
Using the `capl` option, the code generation script can instead be used to generate a CAPL script that can be used, along with the included LDF file, in Vector CANoe or CANalyzer to interpret a conversation between an EV and SE.  The script parses `InfoEntryX` values as defined in SAE J3068 as well as the multiplexed data defined in SAE J3068/1.

For ease of interpretation, EV messages are shown in shades of <span style="color:#002aff">blue</span>, and SE messages are shown in shades of <span style="color:#ff5d00">orange</span>, depending on the message type.

---
Files
---
General files

| Filename(s)                  | Description |
| :--------------------------- | :---------- |
| J3068_1.ldf                  | LDF file updated for SAE J3068/1. |
| J3068_2.ldf                  | LDF file updated for SAE J3068/2. |
| Info Codes.ods               | OpenDocument spreadsheet used to generate `info_codes.h` and `InfoCodeStrings.h` (and `InfoCodeStrings.cin`). |
| `makefile`                   | GNU Make configuration file. |
| `globals.h` <br/>  `ids_handler.h` <br/>  `info_code_handler.h` <br/> `msleep.h` <br/> `protocol_version_hanlder.h` <br/> `scheduler.h` | headers necessary to link the similarly named .c files. |

Emulation files

| Filename                     | Description |
| :--------------------------  | :---------- |
| `bus.c`                      | Defines the `bus` executable. Also generates a log file of the emulated LIN bus traffic.|
| `lincp.c`                    | Defines the `ev` and `se` executables. Includes a function (`void sig_user_1()`) for asynchronous testing using POSIX signal SIGUSR1. |
| `globals.c`                  | Defines global variables. |
| `msleep.c`                   | Defines a millisecond sleep function. |
| `lin-cp.h`                   | Defines a number of macros required by `lin-cp.c` including those required to create a standard LIN stack interface and **configuration settings**. |
| `lincp-emu.c`                | Defines the schedules, the print functions for the log file, and some other miscellaneous functions required by the application code. |
| `lincp-emu.h`                | Defines data structures in accordance with the LDF file, some other miscellaneous functions required by the application code, and the (colored) print macros. |
| `scheduler.c`                | Modified version of the scheduler from the reference implementation to run in the emulated environment. |


Application code files -- These files can be inserted into the [reference implementation](https://github.com/udv2g/saej3068-ref) with little to no modification (in theory at least).

| Filename                     | Description |
| :--------------------------  | :---------- |
| `config.h`                   | Defines main parameters of EV and SE.  Analogous to same file in the reference implementation. |
| `lin-cp.c`                   | Defines the main LIN-CP communication logic. |
| `ids_handler.c`              | Implements SAE J3068/1 |
| `info_code_handler.c`        | Sends and receives paged `InfoEntryX` values and formats them for human and machine legibility. |
| `protocol_version_handler.c` | Implements Protocol Version negotiation when many Protocol Versions are supported. |
| `info_codes.h`               | Header file providing macros for `InfoEntryX` values. |
| `InfoCodeStrings.h`          | "Improper" header file defining strings for each `InfoEntryX` value. |
| `j3072_handler.c`            | Partial implementation of SAE J3068/2 |


Code generator files

| Filename                     | Description |
| :--------------------------  | :---------- |
| `gen_field_code.m`           | Top-level generator script |
| `field table pub.ods`        | Description of SAE J3068/1 data in a format used by the code generator. |
| `field table pub CAPL.ods`   | As above for CAPL code generation. |
| `gen_page_array.m` <br/> `gen_parse_switch.m` <br/> `gen_variables.m` <br/> `gen_xmit_switch.m` <br/> `parse_row.m` | Internal functions for the generation script. |

Vector CAPL files
| Filename                     | Description |
| :--------------------------  | :---------- |
| `ids.can`                    | Parsing script that can be added inline with any unused node in "Measurement Setup" to interpret an SAE J3068 or SAE J3068/1 connection session.  Output is sent to the "Write" window in the "CAPL / .NET" tab. Bus activity is not passed to the connected node. |
| `ids-template.can`           | Template file used by code generator to create `ids.can`. |
| `InfoCodeStrings.cin`        | Include file required by `ids.can` to identify `InfoEntryX`s.  Equivalent to `InfoCodeStrings.h` in the C project. |
| `CANoe.png`                  | Screenshot of `ids.can` used in CANoe. Use in CANalyzer similar. |
| `j3072.can`                  | Parsing script that can be added inline with any unused node in "Measurement Setup" to, in conjunction with ids.can, interpret an SAE J3068/2 connection session.  Output is sent to the "Write" window in the "J3072" tab. Bus activity is not passed to the connected node. |
| `j3072-template.can`         | Template file used by code generator to create `j3072.can`. |

---
Code Generator Usage
---
The script requires the `io` package be installed (if not already present) and loaded (`pkg install -forge io` and `pkg load io`).

The function declaration of the top-level script is:

    function gen_field_code(ods_in, c_in, c_out, print_command='commented', rpage=false, capl=false, interactive=false)

| Argument                   | Description |
| :------------------------- | :---------- |
| `ods_in`                   | Input spreadsheet file.  Normally `'field table pub.ods'` |
| `c_in`                     | Template file. |
| `c_out`                    | Output file.  Cannot be the same file as the input file. |
| `print_command`            | Controls generation of debug printf() statements.  Options are `'yes'`, `'no'`, and `'commented'`.  Defaults to `'commented'`.  If `'yes'`, will also generate and print random data to test the multiplexed data transfer. |
| `capl`                     | Boolean to generate Vector CAPL parsing file instead of C for the emulator.  Defaults to `false`. |
| `rpage` (*new)             | Boolean to generate code that uses paged RAM. |
| `interactive`              | Causes the script to enter `keyboard` mode where the contents of the data structures can be examined.  Execution continues with `dbcont`. |

**_NOTE:_**  Vector CANoe/CANalyzer may crash if a file is loaded that was generated by Octave while Octave is still open

