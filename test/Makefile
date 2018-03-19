CPATH=$(shell pwd)

sub_dirs = client server

define mk_targ
	@for sub_dir in $(sub_dirs);\
	do \
		echo "Enter dir $(CPATH)/$$sub_dir.."; \
		cd $(CPATH)/$$sub_dir;\
		make $1;\
		echo "Leave dir $(CPATH)/$$sub_dir..";\
		cd $(CPATH);\
		echo "";\
	done
endef

define mk_lib
	@echo "make in $(CPATH)/`echo $1` ...";
	@if [ -x "$(CPATH)/`echo $1`" ];\
	then \
		cd $(CPATH)/`echo $1`;\
		make rebuild;\
		cd $(CPATH);\
	else \
		echo "$(CPATH)/`echo $1` not found";\
	fi
endef

all:
	$(call mk_targ, all)

clean:
	$(call mk_targ, clean)
	
rebuild : clean all

debug :
	$(call mk_targ, debug)

release: 
	$(call mk_targ, release)
	
client:
	$(call mk_lib, client)

server:
	$(call mk_lib, server)
