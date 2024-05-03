/**
 * @file   blinky.h
 *
 * @brief  	Brief description of the content of blinky.h
 * @author 	mootseeker
 * @date   	May 3, 2024 - first implementation
 * @version 1.0.0
 * 
 * MIT License
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Copyright (c) 2024 mootseeker
 */
 

#ifndef INC_BLINKY_H_
#define INC_BLINKY_H_

#include "dbc_assert.h" /* Design By Contract (DBC) assertions */

enum Signals
{
	TIMEOUT1_SIG, TIMEOUT2_SIG,
	/* ... */
	MAX_SIG /* the last signal */
};

void Blinky_instantiate( void );
extern SST_Task *const AO_Blinky; /* opaque task pointer */

#endif /* END: blinky.h */
