# List all C files here
_BUFFALOSRC = atmosphere.c \
			 state_estimation.c \
			 geometry/mat3.c \
			 geometry/vec3.c

BUFFALOSRC := $(_BUFFALOSRC:%=$(BUFFALOPATH)/src/%)
BUFFALOINC := $(BUFFALOPATH)/include
