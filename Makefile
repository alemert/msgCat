################################################################################
# makefile for message catalog
################################################################################

MAKE_INCLUDE_PATH=mk.inc

CATALOG.H = $(addprefix include/msgcat/,lgstd.h lgmqm.h lgxym.h lgmqe.h)
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
ROLLOUT_INC = ctl.h msgcat/lgmqm.h  msgcat/lgstd.h msgcat/lgxym.h \
                    msgcat/lgmqe.h

# ------------------------------------------------------------------------------
# general includes
# ------------------------------------------------------------------------------
include $(MAKE_INCLUDE_PATH)/general.modules.mk

$(OBJECT_PATH)/ctl.o: var/vers/include/version.h
CCOPT += -Ivar/vers/include

# ------------------------------------------------------------------------------
# additional internal rules
# ------------------------------------------------------------------------------

$(CAT_SRC_PATH)/catalogrc.c : $(CATALOG.H)
	$(TOOL_SCRIPT)/crtRcRc.pl $@ $^ 

$(CAT_OBJ_PATH)/catalogrc.o : $(CAT_SRC_PATH)/catalogrc.c $(MAKEFILE)
	$(CC) $(CCOPT) $< -o $@

cleanlocal :

# ------------------------------------------------------------------------------
# tests
# ------------------------------------------------------------------------------
#TEST = t_ctl_000
include $(MAKE_INCLUDE_PATH)/test.mk

