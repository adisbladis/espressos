.PHONY: all build generate format

all: generate build

build:
	pio run

generate:
	@mkdir -p src/proto
	protoc -I ../../api --eams_out=src/proto ../../api/*.proto
