
kelkBash: kelkBash.c
	gcc -g -o kelkBash kelkBash.c -lncurses

%.o: %.c
	gcc -g -c $< -o $@

clean:
	rm -f kelkBash

install: kelkBash
	@echo "We Loading Up kelkBash to /bin"
	cp kelkBash /bin

uninstall:
	@echo "We removing kelkBash from /bin"
	rm -f /bin/kelkBash
