##
# x86/paging.s
#
# So I tried to do this in C to make things more
# readable, but I failed since I'm tired. For me,
# the x86 is a little more intuitve since we're
# not doing too much to throw data between the two
# languages.
#
# Author: Dennis J. McWherter, Jr.
# MIT Lincoln Laboratory
##
.intel_syntax noprefix

# We know how virtual memory works, right??
# Quick refresher (more detail in Intel Manual figure 4-2):
#
# Consider a 32-bit virtual address "addr," then:
#   (addr >> 22) == page directory index (top 10 bits)
#   (addr >> 12) & 0x3ff == page table index (middle 10 bits)
#   addr & 0xfff == page index (lower 12 bits)
# 10 bits + 10 bits + 12 bits == full 32-bit address
.globl KERNEL_VIRT_BASE
.set KERNEL_VIRT_BASE, 0xc0000000
.set KERNEL_PDE_IDX, (KERNEL_VIRT_BASE >> 22)
.extern stack

##
# Data section (used for paging tricks)
#
# We start with a direct map so we can
# actually load stuff, but then we remove
# this mapping. View intel manual table 4-5
# for what the bits mean.
#
# Intel Manual: Figure 4-2 is briefest overview.
#
# Explanation of what the heck I'm doing for paging:
#
# First, we need to create a direct map from virt to phys addr
# If we don't do this, then the CPU won't be able
# to fetch the next instruction immediately after
# enabling paging.
#
# This is where the thought comes in. Though it's not
# *too* difficult, you have to realize that we need
# to map through TWO tables. To understand the mapping,
# first consider where our kernel boots to in physical
# memory: address 0x00100000
#
# We want the same virtual address to map to that physical
# address in order to get our next instruction (we can remove
# this mapping once in the higher half). Here is how we come
# to the mapping:
#
# Top 10 bits are page directory index.
# Middle 10 bits are page table index.
# Bottom 12 bits are offset into physical memory.
#
# 0x00100000 = 0000 0000 0001 0000 0000 0000 0000 0000
# Split up by address encoding: |00 0000 0000|01 0000 0000|0000 0000 0000|
# Top 10 bits    = 0 (no-brainer)
# Middle 10 bits = 01 0000 0000 = 0x100
# Bottom 12 bits = 0 (no-brainer)
#
# This gives us a lot of information. First, for this particular
# virtual address, this shows us the EXACT path it will travel
# through our pages tables to translate this to a physical
# address; we just need to manipulate it so it is direct
# to physical memory. 
#
# This means we need to set index 0 in the page directory
# and index 0x100 in the page table to translate properly.
# Notice that we are not concerned with the bottom 12-bits (offset)
# since it will take care of itself if we map correctly.
#
# Figures 4-5 and 4-6 help with this.
##
.section .data
.align 0x1000 # 4KB
##
# Explanation:
# The page directory is nothing more than an array
# of 1024 int's. These int's are encoded in a particular
# way for the CPU (refer to manual references above).
# 
# All we care about here, for now, is initializing
# everything to 0 except the entries we want to map
# right away. We will initialize these with the lower
# 2 bits set (again, encoding) and in the routine
# we will calculate the upper bits (physical address to
# corresponding page table. That said, we will actually
# perform initialization in the init method below,
# here we just set everything to 0.
##
.globl page_directory
page_directory:
.rept 1024
  .long 0
.endr

##
# Explanation
#
# We have up to 1024 different page tables (i.e. 1 for
# each index in the page directory). Each page table--
# similarly to the page directory-- is nothing more than
# an array of 1024 int's which map to a physical page
# in memory. We will initialize these entires similarly
# to the pde.
##
.globl page_tables
page_tables:
# All page table entries. Initialize to 0
.rept 1024
  .rept 1024
    .long 0
  .endr
.endr

##
# Important paging methods
##
##
# Explanation:
#
# This method initializes and enables paging for us.
# We will first setup our pde and pte's and then we
# will go through the simple process of moving
# the proper addresses into the control registers to
# enable paging. Let's go.
##
.section .text
.globl paging_enable
paging_enable:
  # Follow calling conventions, I guess :P
  push ebp
  mov ebp, esp
  push ebx

  # First thing's first-- get our PDE's setup.
  lea ecx, [page_directory]

  # Calculate the table address for the PDE to index
  # These address MUST be physical for the CPU to locate
  # them.
  lea eax, [page_tables]
  or  eax, 0x00000003 # Control bits for page
  mov [ecx], eax # This is the direct-mapping
  mov [ecx+(KERNEL_PDE_IDX*4)], eax # Map for 0xc0000000 

  # Actually update the PTE
  xor eax, 0x00000003 # Clear control bits
  mov ebx, 0x00100003 # Only first 4KB to get our feet off the ground
  mov [eax+0x400], ebx # Remember this offset was calculated
                       # Above. Also sizeof(long) == 4
                       # So index (0x100) * 4 = 0x400 offset

  # Now that we have code working, do the same thing
  # For our stack until we can run the memory manager
  # to get things working properly.
  # These are only temporarily hardcoded
  # NOTE: I calculated these indices in a similar
  # manner as above.
  # Use new page table idx 1
  lea eax, [page_tables+4096] # 1024 entries/table * 4B/entry
  lea ecx, [page_directory+8]
  or  eax, 0x00000003
  mov [ecx], eax
  mov [ecx+((KERNEL_PDE_IDX)*4)], eax
  xor eax, 0x00000003

  # Setup translations
  #mov ebx, 0x00907003
  lea ebx, [stack]
  and ebx, 0xfffff000 # Clear offset bits
  or  ebx, 0x00000003 # Control bits
  mov ecx, ebx
  and ecx, 0x003ff000 # Get the pte idx
  shr ecx, 0x0c # Shift to isolate idx
  mov [eax+ecx*4], ebx
  # Hack: I noticed we also get +0x1000, so map that too
  # to be safe.
  add ecx, 0x01 # Increment the index by 1
  shl ecx, 0x0c # Shift to index position
  and ebx, 0xffc00fff # Clear the index in address
  or  ebx, ecx  # Update index
  shr ecx, 0x0c # Back to index only
  mov [eax+ecx*4], ebx

#  mov ecx, 0x03 # Initialize 4KB worth (whole stack)
#stack_init_loop:
#  mov edx, ebx
#  and edx, 0x003ff000 # Take middle 10 bits
#  shr edx, 0x0c # Shift 12 bits to get just the pte idx
#  #mov [eax+0x107*4], ebx
#  #mov ebx, 0x00908003
#  #mov [eax+0x108*4], ebx
#  inc ecx
#  cmp ecx, 0x1000 # Initialize 4KB worth
#  test ecx, ecx
#  jne stack_init_loop

  # Now we're finally ready to actually enable paging
  # First, load address of page directory into cr3
  lea eax, [page_directory]
  mov cr3, eax

  # Now, do the following to enable paging :)
  mov eax, cr0
  or  eax, 0x80000000
  mov cr0, eax

  # That's it! Paging is enabled.
  # Notice, we can continue here now
  # because we created a direct mapping
  # into memory. After we jump into
  # the higher half, we no longer need this.

  pop ebx
  pop ebp
  ret
  # w00t. I just poppopret'd.

