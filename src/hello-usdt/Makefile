all: hello-usdt
	@echo ======== read probe info with readelf ========
	@readelf -n $<
	@echo
	@echo ======== read probe info with tplist-bpfcc ========
	@if [ `id -u` != 0 ]; then \
		SUDO=sudo; \
	fi; \
	$$SUDO tplist-bpfcc -l $(PWD)/$<
	@echo

hello-usdt: hello.c
	gcc -o $@ $<

.PHONY: clean
clean:
	@rm -f hello-usdt