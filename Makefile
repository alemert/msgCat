################################################################################
# makefile for test util
################################################################################

MAKE_INCLUDE_PATH=mk.inc

CATALOG.H = $(addprefix include/msgcat/,lgstd.h lgmqm.h )
SRC.O += $(CAT_OBJ_PATH)/catalogrc.o

# ------------------------------------------------------------------------------
# Compiler and BIT
# ------------------------------------------------------------------------------
CC=gcc
BIT = 64

# ------------------------------------------------------------------------------
# debugger
# ------------------------------------------------------------------------------
DBGOPT = -g

# ------------------------------------------------------------------------------
# sources
# ------------------------------------------------------------------------------
SOURCES = ctl.c 

# ------------------------------------------------------------------------------
# main source
# ------------------------------------------------------------------------------
MAIN = main.c

# ------------------------------------------------------------------------------
# BINARY
# ------------------------------------------------------------------------------
BINARY = catalogrc

# ------------------------------------------------------------------------------
# libraries dynamic & static
# ------------------------------------------------------------------------------
LIBRARY = libmsgcat.so
ARCHIVE = libmsgcat.a

# ------------------------------------------------------------------------------
# rollout includes
# ------------------------------------------------------------------------------
ROLLOUT_INC = ctl.h msgcat/lgmqm.h  msgcat/lgstd.h

# ------------------------------------------------------------------------------
# general includes
# ------------------------------------------------------------------------------
include $(MAKE_INCLUDE_PATH)/general.modules.mk

# ------------------------------------------------------------------------------
# additional internal rules
# ------------------------------------------------------------------------------

$(CAT_SRC_PATH)/catalogrc.c : $(CATALOG.H)
	bin/crtRcRc.pl $@ $^ 

$(CAT_OBJ_PATH)/catalogrc.o : $(CAT_SRC_PATH)/catalogrc.c $(MAKEFILE)
	$(CC) $(CCOPT) $< -o $@

cleanlocal :

# ------------------------------------------------------------------------------
# tests
# ------------------------------------------------------------------------------
#TEST = t_ctl_000
include $(MAKE_INCLUDE_PATH)/test.mk

