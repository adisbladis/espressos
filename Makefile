.PHONY: generate

generate:
	protoc -I api --eams_out=src/proto api/api.proto
