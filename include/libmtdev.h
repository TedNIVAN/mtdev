/*****************************************************************************
 *
 * mtdev - Multitouch Protocol Translation Library (MIT license)
 *
 * Copyright (C) 2010 Henrik Rydberg <rydberg@euromail.se>
 * Copyright (C) 2010 Canonical Ltd.
 * Copyright (C) 2013 Red Hat, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 ****************************************************************************/

#ifndef _LIBMTDEV_H
#define _LIBMTDEV_H

#ifdef __cplusplus
extern "C" {
#endif

#include <linux/input.h>

/* This is the library interface for mtdev, for users that need conversion
 * from protocol A to protocol B, but do not require or want mtdev to handle
 * the file descriptor.
 *
 * Pseudo-code on how to use it:
 *
 *  struct mtdev *dev = mtdev_new();
 *  mtdev_set_abs(dev, ABS_MT_POSITION_X, absx);
 *  mtdev_set_abs(dev, ABS_MT_POSITION_Y, absy);
 *
 *  mtdev_put_event(dev, ... ABS_MT_POSITION_X event ... );
 *  mtdev_put_event(dev, ... ABS_MT_POSITION_Y event ... );
 *  mtdev_put_event(dev, ... SYN_MT_REPORT event ... );
 *  mtdev_put_event(dev, ... ABS_MT_POSITION_X event ... )
 *  mtdev_put_event(dev, ... ABS_MT_POSITION_Y event ... );
 *  mtdev_put_event(dev, ... SYN_MT_REPORT event ... );
 *  mtdev_put_event(dev, ... SYN_REPORT event ... );
 *
 *  while(!mtdev_empty(dev)) {
 *	struct input_event ev;
 *	mtdev_get_event(dev, &ev);
 *	... ev is now protocol B ...
 *  }
 */
struct mtdev;

/**
 * mtdev_new - allocate a new mtdev
 *
 * Allocate a new mtdev.
 *
 * Returns zero in case of memory allocation failure.
 */
struct mtdev *mtdev_new(void);

/**
 * mtdev_set_abs - enable axis and set abs axis information
 * @dev: the mtdev in use
 * @code: the ABS_MT code to set
 * @abs: abs axis information
 *
 * Enables the given axis on the device and changes the various axis ranges
 * to the data given here.
 */
void mtdev_set_abs(struct mtdev *dev, int code, const struct input_absinfo *abs);

/**
 * mtdev_put_event - put an event into the converter
 * @dev: the mtdev in use
 * @ev: the kernel input event to put
 *
 * Put a kernel event into the mtdev converter. The event should
 * come straight from the device.
 *
 * This call does not block; if the buffer becomes full, older events
 * are dropped. The buffer is guaranteed to handle several complete MT
 * packets.
 */
void mtdev_put_event(struct mtdev *dev, const struct input_event *ev);

/**
 * mtdev_empty - check if there are events to get
 * @dev: the mtdev in use
 *
 * Returns true if the processed event queue is empty, false otherwise.
 */
int mtdev_empty(struct mtdev *dev);

/**
 * mtdev_get_event - get processed events from mtdev
 * @dev: the mtdev in use
 * @ev: the input event to fill
 *
 * Get a processed event from mtdev. The events appear as if they came
 * from a type B device emitting MT slot events.
 *
 * The queue must be non-empty before calling this function.
 */
void mtdev_get_event(struct mtdev *dev, struct input_event* ev);

/**
 * mtdev_delete - free a previously allocated mtdev
 *
 * Frees the memory associated with the mtdev and invalidates the
 * mtdev pointer.
 */
void mtdev_delete(struct mtdev *dev);

#ifdef __cplusplus
}
#endif

#endif
