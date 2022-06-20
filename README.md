---
Introduction
---
This project emulates the LIN-CP communications between an EV and SE for the purposes of development.  It uses POSIX shared memory objects to mimic the function of the LIN stack.  The bus master executable directly copies data between the memory maps of the EV and SE executables to mimic the function of the LIN stack.  The SE executable sets the LIN schedule via another shared memory object with the bus master.  The bus timing is not accurately modeled, the entire schedule is copied at once at approximately the schedule repetition rate.  Only the LIN flags used by the application code (the frame flags) are implemented.  Unplug/replug is also not properly implemented.  

It is based on the [reference implementation](https://github.com/udv2g/saej3068-ref) and, therefore, implements both the EV and SE in the same code base and the application code supports two connectors/inlets on each side, but the emulator only supports one (channel A).  The SAE J3068/1 implementation also only currently supports channel A.  

**The data definitions are far from final and may not match those in the current draft of SAE J3068/1.  The intention of this code is to develop the transfer mechanism, not the data definitions.**

---
Usage
---
The project has been tested on Linux, WSL, and OSX.  To build, simply run `make`.  Once, built, it can be run by starting each of the three executables in a separate terminal with `./ev`, `./se`, and `./bus`.  bus must be run last, or it will fail to open the shared memory objects created by the other processes. 

Serial debug console output (`PrintConsoleXX()`) is shown in <span style="color:#ffff5f;background-color:black">yellow</span> and serial command console output (`PrintCmdXX()`) is shown in <span style="color:#5fff00;background-color:black">green</span>.  Debug and command can be sent to the same or different serial ports in the main project.  Uncolored text is output from development statements (`printf()`).

---
Files
---
General files

| Filename(s)                  | Description |
| :--------------------------- | :---------- |
| Draft_J3068_1.ldf            | LDF file updated with the frames required for SAE J3068/1. |
| Info Codes.ods               | OpenDocument spreadsheet used to generate `info_codes.h` and `InfoCodeStrings.h`. |
| `makefile`                   | GNU Make configuration file. |
| `globals.h` <br/>  `ids_handler.h` <br/>  `info_code_handler.h` <br/> `msleep.h` <br/> `protocol_version_hanlder.h` | headers necessary to link the similarly named .c files. |

Emulation files

| Filename                     | Description |
| :--------------------------  | :---------- |
| `bus.c`                      | Defines the `bus` executable. Also generates a log file of the emulated LIN bus traffic.|
| `lincp.c`                    | Defines the `ev` and `se` executables. Includes a function (`void sig_user_1()`) for asynchronous testing using POSIX signal SIGUSR1. |
| `globals.c`                  | Defines global variables. |
| `msleep.c`                   | Defines a millisecond sleep function. |
| `lin-cp.h`                   | Defines a number of macros required by `lin-cp.c` including those required to create a standard LIN stack interface and **configuration settings**. |
| <nobr>`lincp-emu.c`</nobr>   | Defines the schedules, the print functions for the log file, and some other miscellaneous functions required by the application code. |
| <nobr>`lincp-emu.h`</nobr>   | Defines data structures in accordance with the LDF file, some other miscellaneous functions required by the application code, and the (colored) print macros. |


Application code files -- These files can be inserted into the [reference implementation](https://github.com/udv2g/saej3068-ref) with little to no modification (in theory at least).

| Filename                     | Description |
| :--------------------------  | :---------- |
| `lin-cp.c`                   | Defines the main LIN-CP communication logic. |
| `ids_handler.c`              | Implements SAE J3068/1 |
| `info_code_handler.c`        | Sends and receives paged `InfoEntryX` values and formats them for human and machine legibility. |
| `protocol_version_handler.c` | Implements Protocol Version negotiation when many Protocol Versions are supported. |
| `info_codes.h`               | Header file providing macros for `InfoEntryX` values. |
| `InfoCodeStrings.h`          | "Improper" header file defining strings for each `InfoEntryX` value. |

