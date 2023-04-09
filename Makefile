.PHONY: all build generate

all: generate build

build:
	pio run

generate:
	protoc -I api --eams_out=src/proto api/*.proto
