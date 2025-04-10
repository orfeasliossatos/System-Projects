## ======================================================================
## partial Makefile provided to students
##

LIBMONGOOSEDIR = libmongoose/
## don't forget to export LD_LIBRARY_PATH pointing to it

.PHONY: clean new newlibs style \
feedback feedback-VM-CO clone-ssh clean-fake-ssh \
submit1 submit2 submit

CFLAGS += -std=c11 -Wall -pedantic -g $$(pkg-config vips --cflags)
VIPS_CFLAGS += $$(pkg-config vips --cflags)
VIPS_LIBS   += $$(pkg-config vips --libs) -lm

# a bit more checks if you'd like to (uncomment)
# CFLAGS += -Wextra -Wfloat-equal -Wshadow                         \
# -Wpointer-arith -Wbad-function-cast -Wcast-align -Wwrite-strings \
# -Wconversion -Wunreachable-code

# ----------------------------------------------------------------------
# feel free to update/modifiy this part as you wish

TARGETS := imgStoreMgr
CHECK_TARGETS := tests/test-imgStore-implementation
OBJS :=
RUBS = $(OBJS) core

all:: $(TARGETS)

imgStoreMgr: error.o imgst_list.o imgStoreMgr.o tools.o util.o imgst_create.o \
imgst_delete.o image_content.o dedup.o imgst_insert.o imgst_read.o
	gcc $(CFLAGS) error.o imgst_list.o imgStoreMgr.o tools.o util.o imgst_create.o \
imgst_delete.o image_content.o dedup.o imgst_insert.o imgst_read.o $(VIPS_LIBS) -lssl -lcrypto \
-o imgStoreMgr

error.o: error.c
dedup.o: dedup.c dedup.h imgStore.h error.h 
imgStoreMgr.o: imgStoreMgr.c util.h imgStore.h error.h
tools.o: tools.c imgStore.h error.h
util.o: util.c
imgst_create.o: imgst_create.c imgStore.h error.h
imgst_insert.o: imgst_insert.c imgStore.h error.h dedup.h image_content.h
	gcc -std=c11 $(VIPS_CFLAGS) -g -Wall -pedantic -c imgst_insert.c -lssl -lcrypto
imgst_list.o: imgst_list.c imgStore.h error.h
imgst_delete.o: imgst_delete.c imgStore.h error.h
imgst_read.o: imgst_read.c imgStore.h error.h image_content.h
image_content.o: image_content.c image_content.h imgStore.h error.h

# ----------------------------------------------------------------------
# This part is to make your life easier. See handouts how to make use of it.

## ======================================================================
## Tests

# target to run black-box tests
check:: all
	@if ls tests/*.*.sh 1> /dev/null 2>&1; then \
	    for file in tests/*.*.sh; do [ -x $$file ] || echo "Launching $$file"; ./$$file || exit 1; done; \
	fi

# all those libs are required on Debian, adapt to your box
$(CHECK_TARGETS): LDLIBS += -lcheck -lm -lrt -pthread -lsubunit

check:: CFLAGS += -I.
check:: $(CHECK_TARGETS)
	export LD_LIBRARY_PATH=.; $(foreach target,$(CHECK_TARGETS),./$(target) &&) true

clean::
	-@/bin/rm -f *.o *~ $(CHECK_TARGETS)

new: clean all

static-check:
	CCC_CC=$(CC) scan-build -analyze-headers --status-bugs -maxloop 64 make -j1 new

style:
	astyle -n -o -F  -A8 -xt0 *.[ch]

## ======================================================================
## Feedback

IMAGE=chappeli/pps21-feedback:week09
## Note: vous pouvez changer le tag latest pour week04, ou week05, etc.

REPO := $(shell git config --get remote.origin.url)
SSH_DIR := $(HOME)/.ssh

feedback:
	@echo Will use $(REPO) inside container
	@docker pull $(IMAGE)
	@docker run -it --rm -e REPO=$(REPO) -v $(SSH_DIR):/opt/.ssh $(IMAGE)

clone-ssh:
	@-$(eval SSH_DIR := $(HOME)/.$(shell date "+%s;$$"|sha256sum|cut -c-32))
	@cp -r $(HOME)/.ssh/. $(SSH_DIR)

clean-fake-ssh:
	@case $(SSH_DIR) in $(HOME)/\.????????????????????????????????) $(RM) -fr $(SSH_DIR) ;; *) echo "Dare not remove \"$(SSH_DIR)\"" ;; esac

feedback-VM-CO: clone-ssh feedback clean-fake-ssh

## ======================================================================
## Submit

SUBMIT_SCRIPT=../provided/submit.sh
submit1: $(SUBMIT_SCRIPT)
	@$(SUBMIT_SCRIPT) 1

submit2: $(SUBMIT_SCRIPT)
	@$(SUBMIT_SCRIPT) 2

submit:
	@printf 'what "make submit"??\nIt'\''s either "make submit1" or "make submit2"...\n'

