﻿/*!****************************************************************************
 * @file		syscall.c
 * @author		d_el
 * @version		V1.0
 * @date		02.05.2017
 * @brief		POSIX System Calls
 * @copyright	Copyright (C) 2017 Storozhenko Roman
 *				All rights reserved
 *				This software may be modified and distributed under the terms
 *				of the BSD license.	 See the LICENSE file for details
 */

#include <errno.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <sys/unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include "stm32f4xx.h"

/* These magic symbols are provided by the linker.  */
extern void (*__preinit_array_start[])(void) __attribute__((weak));
extern void (*__preinit_array_end[])(void) __attribute__((weak));
extern void (*__init_array_start[])(void) __attribute__((weak));
extern void (*__init_array_end[])(void) __attribute__((weak));
extern void (*__fini_array_start[])(void) __attribute__((weak));
extern void (*__fini_array_end[])(void) __attribute__((weak));

extern void _fini(void);
extern uint32_t __get_MSP(void);

/*!****************************************************************************
 * @brief 	Increase program data space. Malloc and related functions depend on this
 * @param 	incr - increase bytes
 * @return	previous heap end value
 */
__attribute__((weak))
caddr_t _sbrk(intptr_t incr){
	extern size_t _ebss; 							/// Defined by the linker
	static caddr_t heapEnd = (caddr_t) &_ebss;
	caddr_t prevHeapEnd = heapEnd;
	caddr_t stack = (caddr_t) __get_MSP();

	if(heapEnd + incr >= stack){
		errno = ENOMEM;
		abort();
	}

	heapEnd += incr;
	return (caddr_t) prevHeapEnd;
}

/*!****************************************************************************
 * @brief	Terminates the calling process "immediately"
 * @param 	code - exit status
 */
__attribute__((weak))
void _exit(int code){
	while(1);
}

/*!****************************************************************************
 * @brief	Initialize serial data structure
 */
__attribute__((weak))
void _init(void){

}

/*!****************************************************************************
 *
 */
__attribute__((weak))
void _fini(void){

}

/*!****************************************************************************
 * @brief	Iterate over all the init routines
 */
__attribute__((weak))
void __libc_init_array(void){
	size_t count;
	size_t i;

	count = __preinit_array_end - __preinit_array_start;
	for(i = 0; i < count; i++){
		__preinit_array_start[i]();
	}

	_init();

	count = __init_array_end - __init_array_start;
	for(i = 0; i < count; i++){
		__init_array_start[i]();
	}
}

/*!****************************************************************************
 * @brief	Run all the cleanup routines
 */
__attribute__((weak))
void __libc_fini_array(void){
	size_t count;
	size_t i;

	count = __fini_array_end - __fini_array_start;
	for(i = count; i > 0; i--){
		__fini_array_start[i - 1]();
	}
	_fini();
}

/*!****************************************************************************
 * @brief	Send a signal. Minimal implementation:
 */
__attribute__((weak))
int _kill(int pid, int sig){
	errno = EINVAL;
	return -1;
}

/*!****************************************************************************
 * @brief	Process-ID; this is sometimes used to generate strings unlikely to
 * 			conflict with other processes.
 * 			Minimal implementation, for a system without processes:
 */
__attribute__((weak))
int _getpid(void){
	return 1;
}

/*!****************************************************************************
 * @brief	Write a character to a file. 'libc' subroutines will use this system
 * 			routine for output to all files, including stdout
 * @retval	Returns -1 on error or number of bytes sent
 */
__attribute__((weak))
int _write(int file, char *ptr, int len){
	return -1;
}

/*!****************************************************************************
 * @brief	Read a character to a file. 'libc' subroutines will use this system
 * 			routine for input from all files, including stdin.
 * 			Returns -1 on error or blocks until the number of characters have
 * 			been read
 */
__attribute__((weak))
int _read(int file, char *ptr, int len){
	return -1;
}

/*!****************************************************************************
 * @brief	Close a file
 * @retval	Returns -1 on error or number of bytes sent
 */
__attribute__((weak))
int _close(int file){
	return -1;
}

/*!****************************************************************************
 * @brief	Status of an open file. For consistency with other minimal
 * 			implementations in these examples,
 * 			all files are regarded as character special devices.
 * 			The sys/stat.h header file required is distributed in the 'include'
 * 			subdirectory for this C library
 */
__attribute__((weak))
int _fstat(int file, struct stat *st){
	st->st_mode = S_IFCHR;
	return 0;
}

/*!****************************************************************************
 * @brief	Query whether output stream is a terminal. For consistency with the
 * 			other
 * 			minimal implementations:
 */
__attribute__((weak))
int _isatty(int file){
	switch(file){
		case STDOUT_FILENO:
		case STDERR_FILENO:
		case STDIN_FILENO:
			return 1;
		default:
			errno = EBADF;
			return 0;
	}
}

/*!****************************************************************************
 * @brief	Set position in a file. Minimal implementation:
 */
__attribute__((weak))
int _lseek(int file, int ptr, int dir){
	return 0;
}

/***************** Copyright (C) Storozhenko Roman ******* END OF FILE *******/
