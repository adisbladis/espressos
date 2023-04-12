.PHONY: espressos web

all: espressos web

espressos:
	$(MAKE) -C pkgs/espressos

web:
	cd pkgs/espressos-web && npm run generate && npm run build
