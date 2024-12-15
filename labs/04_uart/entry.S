.syntax unified

.section .text

.thumb_func
.global __reset_handler
__reset_handler:
    blx main
__halt:
	b __halt
__exc_handler:
	b __exc_handler

.section .vector_table
.word __stack_start    // Initial SP
.word __reset_handler  // Reset Handler
.word __exc_handler    // Hard Fault Handler
.word __exc_handler    // Memory Management Fault Handler
.word __exc_handler    // Bus Fault Handler
.word __exc_handler    // Usage Fault Handler
