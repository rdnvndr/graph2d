
All: base nls

clean: clean-base clean-nls

install: install-base install-pal install-nls 


base:
	@$(MAKE) -C src All

nls:
	@$(MAKE) -C po all

clean-base:
	@$(MAKE) -C src clean

clean-nls:
	@$(MAKE) -C po clean

install-base:
	@$(MAKE) -C src install

install-nls:
	@$(MAKE) -C po install

install-pal:
	mkdir /usr/local/share/graph2d
	mkdir /usr/local/share/graph2d/palettes
	cp palettes/*.pal /usr/local/share/graph2d/palettes
