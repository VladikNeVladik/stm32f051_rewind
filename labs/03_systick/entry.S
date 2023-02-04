.syntax unified

.section .text

.thumb_func
.global __reset_handler
__reset_handler:
	// Copy .data section to SRAM:
	ldr r0, __data_start_lma_val
	ldr r1, __data_start_vma_val
	ldr r2, __data_end_vma_val

__loop_copy_data_section:
	cmp r1, r2
	beq __loop_copy_data_section_end

	ldr r4, [r0, #0]
	str r4, [r1, #0]

	adds r0, r0, #4
	adds r1, r1, #4
	b __loop_copy_data_section
__loop_copy_data_section_end:

	// Execute actual software:
    blx main

__halt:
	b __halt
__exc_handler:
	b __exc_handler

__data_start_lma_val:
.word __data_start_lma
__data_start_vma_val:
.word __data_start_vma
__data_end_vma_val:
.word __data_end_vma

.section .vector_table
.word __stack_start         // Initial SP
.word __reset_handler       // Reset Handler
.word __exc_handler         // NMI Handler
.word __exc_handler         // Hard Fault Handler
.fill 7, 4, 0x00            // Reserved
.word __exc_handler         // SVCall
.fill 2, 4, 0x00            // Reserved
.word __exc_handler         // PendSV
.word systick_handler       // SysTick
