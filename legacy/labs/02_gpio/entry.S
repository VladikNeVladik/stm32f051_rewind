.syntax unified

.section .text

.thumb_func
.global __reset_handler
__reset_handler:
    blx main
__halt:
	b __halt

.section .vector_table
.word __stack_start    // Initial SP
.word __reset_handler  // Reset Handler
