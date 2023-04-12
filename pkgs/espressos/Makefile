.PHONY: all build generate format

all: generate build

format:
	clang-format -i `find src -type f`

build:
	pio run

generate:
	protoc -I ../../api --eams_out=src/proto ../../api/*.proto
	clang-format -i src/proto/*.h
