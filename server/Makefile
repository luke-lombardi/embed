all:
	make -C libraries/pubnub/posix -f posix.mk
	# make -C libraries/pubnub/openssl -f posix.mk

unittest:
	make -C core

clean:
	make -C libraries/pubnub/posix -f posix.mk clean
	# make -C libraries/pubnub/openssl -f posix.mk clean
	# make -C core clean
