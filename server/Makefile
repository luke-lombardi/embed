BUILD_ROOT=build

PUBNUB_ROOT=libraries/pubnub
PUBNUB_INCLUDES=-I libraries/pubnub/posix -I libraries/pubnub

APP_SRC_ROOT=src
APP_SRC=$(APP_SRC_ROOT)/pubnub_sync_sample.c

INCLUDES=-I $(APP_SRC_ROOT)

all: libraries app
libraries: pubnub

# pubnub libraries are dropped into build/ before they are linked by testapp
pubnub:
	make -C libraries/pubnub/posix -f posix.mk
	# make -C libraries/pubnub/openssl -f posix.mk

app:
	gcc -o build/bin/app $(APP_SRC) $(BUILD_ROOT)/pubnub_sync.a $(PUBNUB_INCLUDES) $(INCLUDES)

unittest:
	make -C core

clean:
	make -C libraries/pubnub/posix -f posix.mk clean
	rm build/*.a build/*.o
	rm build/bin/*
	# make -C libraries/pubnub/openssl -f posix.mk clean
	# make -C core clean

run:
	build/bin/app
