.PHONY: all test build generate format

all: generate build

build:
	pio run

test:
	pio test -e native

generate:
	@mkdir -p lib/proto
	protoc -I ../../api --eams_out=lib/proto ../../api/*.proto
