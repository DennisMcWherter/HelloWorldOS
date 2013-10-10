/**
 * kernel/mem/kheap.h
 *
 * Simple kernel heap. Works more like a dynamic stack.
 * This is a "free-less" version of the WaterMark allocator
 * explained here:
 * http://wiki.osdev.org/Memory_Allocation#A_very_very_simple_Memory_Manager
 *
 * This is what actually keeps track of our physical address space.
 *
 * Author: Dennis J. McWherter, Jr.
 */

#ifndef KERNEL_MEM_KHEAP_H__
#define KERNEL_MEM_KHEAP_H__

/**
 * Our kernel heap looks different than a
 * typical process heap since we reserve
 * our kernel memory and we have a fixed
 * stack in this reserved space. That is,
 * we lay this out as follows
 *
 *
 * ------------------
 * |                |
 * |       ^        |
 * |       |        |
 * |      Heap      |
 * ------------------
 * |                |
 * | Kernel Resv'd  |
 * | Incl. stack    |
 * |                |
 * ------------------
 */

/**
 * Simple method to bootstrap our kernel
 * heap allocator
 *
 * @return  1 on success 0 otherwise
 */
int kheap_init();

/**
 * simple malloc method (does not align anything)
 *
 * NOTE: Since we do not free individually, we do not
 *       track this memory.
 *
 * @param size  Number of bytes to allocate
 * @return  Pointer to the physical memory address or
 *          0 if out of memory
 */
void* kheap_malloc(int size);

/**
 * Placement malloc. That is, allocate size bytes
 * from physical address pl. Due to its side-effects
 * we use this instead of free.
 *
 * WARNING: This is dangerous if not used properly
 *          as you _WILL_ overwrite previously allocated
 *          memory
 *
 * NOTE: This will update your internal malloc position.
 *       That is, the next time you use malloc(), it will
 *       start at address (pl+size)
 *
 * @param size  Number of bytes to allocate
 * @param pl    Placement address to allocate
 * @return  The physical address of the allocated block
 *          or 0 if not enough space (i.e. relocate stuff)
 */
void* kheap_malloc_pl(int size, unsigned pl);

/**
 * Free all current memory
 */
void kheap_free_all();

#endif /** KERNEL_MEM_KHEAP_H__ */

