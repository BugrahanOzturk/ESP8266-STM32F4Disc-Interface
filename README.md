#### Compile

[makefile](makefile) contains necessary build scripts and compiler flags.

Browse into the directory and run `make` to compile. You should see a similar output as shown below.

```
Cleaning...
Building main.c
   text    data     bss     dec     hex filename
    432	     16	     16	    464	    1d0 main.elf
Successfully finished...
```

If you see any errors about command not found, make sure the toolchain binaries are in your `PATH`. On Windows check the *Environment Variables* for your account. On Linux/macOS run `echo $PATH` to verify your installation.

#### Program

Run `make burn` to program the chip.

```
...
Flash written and verified! jolly good!
```

Install the [ST LINK](https://www.st.com/en/development-tools/st-link-v2.html) drivers if you cannot see your board when `make burn` is run.

#### Disassemble

Run `make disass` / `make disass-all` to disassamble.

#### Debug

In order to debug your code, connect your board to the PC, run `st-util` (comes with stlink utility) from one terminal, and from another terminal within the project directory run `make debug`. You can then use general **gdb** commands to browse through the code.
