
PUBNUB_ROOT=libraries/pubnub
BUILD_ROOT=build
PUBNUB_INCLUDES=-I libraries/pubnub/posix -I libraries/pubnub

all: libraries testapp
libraries: pubnub

# pubnub libraries are dropped into build/ before they are linked by testapp
pubnub:
	make -C libraries/pubnub/posix -f posix.mk
	# make -C libraries/pubnub/openssl -f posix.mk

testapp:
	gcc -o build/bin/testapp $(PUBNUB_ROOT)/core/samples/pubnub_sync_sample.c $(BUILD_ROOT)/pubnub_sync.a $(PUBNUB_INCLUDES)

unittest:
	make -C core

clean:
	make -C libraries/pubnub/posix -f posix.mk clean
	rm build/*.a build/*.o
	# make -C libraries/pubnub/openssl -f posix.mk clean
	# make -C core clean
