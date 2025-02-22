SOURCEFILES = ../core/pubnub_pubsubapi.c ../core/pubnub_coreapi.c ../core/pubnub_coreapi_ex.c ../core/pubnub_ccore_pubsub.c ../core/pubnub_ccore.c ../core/pubnub_netcore.c  ../lib/sockets/pbpal_sockets.c ../lib/sockets/pbpal_resolv_and_connect_sockets.c ../core/pubnub_alloc_std.c ../core/pubnub_assert_std.c ../core/pubnub_generate_uuid.c ../core/pubnub_blocking_io.c ../posix/posix_socket_blocking_io.c ../core/pubnub_timers.c ../core/pubnub_json_parse.c  ../lib/md5/md5.c ../lib/base64/pbbase64.c ../core/pubnub_helper.c pubnub_version_posix.c pubnub_generate_uuid_posix.c pbpal_posix_blocking_io.c ../core/pubnub_generate_uuid_v3_md5.c  ../core/pubnub_free_with_timeout_std.c  ../core/pubnub_subscribe_v2.c msstopwatch_monotonic_clock.c ../core/pubnub_url_encode.c

OBJFILES = $(BUILD_DIR)/pubnub_pubsubapi.o $(BUILD_DIR)/pubnub_coreapi.o $(BUILD_DIR)/pubnub_coreapi_ex.o $(BUILD_DIR)/pubnub_ccore_pubsub.o $(BUILD_DIR)/pubnub_ccore.o $(BUILD_DIR)/pubnub_netcore.o  $(BUILD_DIR)/pbpal_sockets.o $(BUILD_DIR)/pbpal_resolv_and_connect_sockets.o $(BUILD_DIR)/pubnub_alloc_std.o $(BUILD_DIR)/pubnub_assert_std.o $(BUILD_DIR)/pubnub_generate_uuid.o $(BUILD_DIR)/pubnub_blocking_io.o $(BUILD_DIR)/posix_socket_blocking_io.o $(BUILD_DIR)/pubnub_timers.o $(BUILD_DIR)/pubnub_json_parse.o  $(BUILD_DIR)/md5.o $(BUILD_DIR)/pbbase64.o $(BUILD_DIR)/pubnub_helper.o  $(BUILD_DIR)/pubnub_version_posix.o  $(BUILD_DIR)/pubnub_generate_uuid_posix.o $(BUILD_DIR)/pbpal_posix_blocking_io.o $(BUILD_DIR)/pubnub_generate_uuid_v3_md5.o  $(BUILD_DIR)/pubnub_free_with_timeout_std.o $(BUILD_DIR)/pubnub_subscribe_v2.o $(BUILD_DIR)/msstopwatch_monotonic_clock.o $(BUILD_DIR)/pubnub_url_encode.o

ifndef USE_IPV6
USE_IPV6 = 1
endif

ifndef USE_PROXY
USE_PROXY = 1
endif

ifndef USE_DNS_SERVERS
USE_DNS_SERVERS = 1
endif

ifndef USE_GZIP_COMPRESSION
USE_GZIP_COMPRESSION = 1
endif

ifndef RECEIVE_GZIP_RESPONSE
RECEIVE_GZIP_RESPONSE = 1
endif

ifndef USE_ADVANCED_HISTORY
USE_ADVANCED_HISTORY = 1
endif


ifeq ($(USE_PROXY), 1)
SOURCEFILES += ../core/pubnub_proxy.c ../core/pubnub_proxy_core.c ../core/pbhttp_digest.c ../core/pbntlm_core.c ../core/pbntlm_packer_std.c
OBJFILES += $(BUILD_DIR)/pubnub_proxy.o $(BUILD_DIR)/pubnub_proxy_core.o $(BUILD_DIR)/pbhttp_digest.o $(BUILD_DIR)/pbntlm_core.o $(BUILD_DIR)/pbntlm_packer_std.o
endif

ifeq ($(USE_DNS_SERVERS), 1)
SOURCEFILES += ../core/pubnub_dns_servers.c ../posix/pubnub_dns_system_servers.c ../lib/pubnub_parse_ipv4_addr.c
OBJFILES += $(BUILD_DIR)/pubnub_dns_servers.o $(BUILD_DIR)/pubnub_dns_system_servers.o $(BUILD_DIR)/pubnub_parse_ipv4_addr.o
endif

ifeq ($(USE_IPV6), 1)
SOURCEFILES += ../lib/pubnub_parse_ipv6_addr.c
OBJFILES += $(BUILD_DIR)/pubnub_parse_ipv6_addr.o
endif

ifeq ($(USE_GZIP_COMPRESSION), 1)
SOURCEFILES += ../lib/miniz/miniz_tdef.c ../lib/miniz/miniz.c ../lib/pbcrc32.c ../core/pbgzip_compress.c
OBJFILES += $(BUILD_DIR)/miniz_tdef.o $(BUILD_DIR)/miniz.o $(BUILD_DIR)/pbcrc32.o $(BUILD_DIR)/pbgzip_compress.o
endif

ifeq ($(RECEIVE_GZIP_RESPONSE), 1)
SOURCEFILES += ../lib/miniz/miniz_tinfl.c ../core/pbgzip_decompress.c
OBJFILES += $(BUILD_DIR)/miniz_tinfl.o $(BUILD_DIR)/pbgzip_decompress.o
endif

ifeq ($(USE_ADVANCED_HISTORY), 1)
SOURCEFILES += ../core/pbcc_advanced_history.c ../core/pubnub_advanced_history.c
OBJFILES += $(BUILD_DIR)/pbcc_advanced_history.o $(BUILD_DIR)/pubnub_advanced_history.o
endif

OS := $(shell uname)
ifeq ($(OS),Darwin)
SOURCEFILES += monotonic_clock_get_time_darwin.c
OBJFILES += $(BUILD_DIR)/monotonic_clock_get_time_darwin.o
LDLIBS=-lpthread
else
SOURCEFILES += monotonic_clock_get_time_posix.c
OBJFILES += $(BUILD_DIR)/monotonic_clock_get_time_posix.o
LDLIBS=-lrt -lpthread
endif

CFLAGS =-g -Wall -D PUBNUB_THREADSAFE -D PUBNUB_LOG_LEVEL=PUBNUB_LOG_LEVEL_WARNING -D PUBNUB_PROXY_API=$(USE_PROXY) -D PUBNUB_USE_IPV6=$(USE_IPV6) #x86_64-apple-darwin18.2.0
# -g enables debugging, remove to get a smaller executable
# -fsanitize-address Use AddressSanitizer

INCLUDES=-I .. -I .
BUILD_DIR=../../../build

all: pubnub_sync.a pubnub_callback.a

SYNC_INTF_SOURCEFILES=../core/pubnub_ntf_sync.c ../core/pubnub_sync_subscribe_loop.c ../core/srand_from_pubnub_time.c
SYNC_INTF_OBJFILES=$(BUILD_DIR)/pubnub_ntf_sync.o $(BUILD_DIR)/pubnub_sync_subscribe_loop.o $(BUILD_DIR)/srand_from_pubnub_time.o

pubnub_sync.a: $(SOURCEFILES) $(SYNC_INTF_SOURCEFILES)
	gcc -c $(CFLAGS) $(INCLUDES) $(SOURCEFILES) $(SYNC_INTF_SOURCEFILES)
	mv *.o $(BUILD_DIR)
	# mv *.dSYM $(BUILD_DIR)
	/usr/bin/ar rcs $(BUILD_DIR)/pubnub_sync.a $(OBJFILES) $(SYNC_INTF_OBJFILES)

CALLBACK_INTF_SOURCEFILES=pubnub_ntf_callback_posix.c pubnub_get_native_socket.c ../core/pubnub_timer_list.c ../lib/sockets/pbpal_ntf_callback_poller_poll.c ../lib/sockets/pbpal_adns_sockets.c ../lib/pubnub_dns_codec.c ../core/pbpal_ntf_callback_queue.c ../core/pbpal_ntf_callback_admin.c ../core/pbpal_ntf_callback_handle_timer_list.c  ../core/pubnub_callback_subscribe_loop.c
CALLBACK_INTF_OBJFILES=$(BUILD_DIR)/pubnub_ntf_callback_posix.o $(BUILD_DIR)/pubnub_get_native_socket.o $(BUILD_DIR)/pubnub_timer_list.o $(BUILD_DIR)/pbpal_ntf_callback_poller_poll.o $(BUILD_DIR)/pbpal_adns_sockets.o $(BUILD_DIR)/pubnub_dns_codec.o $(BUILD_DIR)/pbpal_ntf_callback_queue.o $(BUILD_DIR)/pbpal_ntf_callback_admin.o $(BUILD_DIR)/pbpal_ntf_callback_handle_timer_list.o $(BUILD_DIR)/pubnub_callback_subscribe_loop.o

pubnub_callback.a: $(SOURCEFILES) $(CALLBACK_INTF_SOURCEFILES)
	gcc -c $(CFLAGS) -D PUBNUB_CALLBACK_API $(INCLUDES) $(SOURCEFILES) $(CALLBACK_INTF_SOURCEFILES)
	mv *.o $(BUILD_DIR)
	# mv *.dSYM $(BUILD_DIR)
	/usr/bin/ar rcs $(BUILD_DIR)/pubnub_callback.a $(OBJFILES) $(CALLBACK_INTF_OBJFILES)

# pubnub_sync_sample: ../core/samples/pubnub_sync_sample.c pubnub_sync.a
# 	gcc -o $@ $(CFLAGS) $(INCLUDES) ../core/samples/pubnub_sync_sample.c pubnub_sync.a $(LDLIBS)
	
# metadata: ../core/samples/metadata.c pubnub_sync.a
# 	gcc -o $@ $(CFLAGS) $(INCLUDES) ../core/samples/metadata.c pubnub_sync.a $(LDLIBS)

# pubnub_sync_subloop_sample: ../core/samples/pubnub_sync_subloop_sample.c pubnub_sync.a
# 	gcc -o $@ $(CFLAGS) $(INCLUDES) ../core/samples/pubnub_sync_subloop_sample.c pubnub_sync.a $(LDLIBS)

# pubnub_sync_publish_retry: ../core/samples/pubnub_sync_publish_retry.c pubnub_sync.a
# 	gcc -o $@ $(CFLAGS) $(INCLUDES) ../core/samples/pubnub_sync_publish_retry.c pubnub_sync.a $(LDLIBS)

# pubnub_publish_via_post_sample: ../core/samples/pubnub_publish_via_post_sample.c pubnub_sync.a
# 	gcc -o $@ $(CFLAGS) $(INCLUDES) ../core/samples/pubnub_publish_via_post_sample.c pubnub_sync.a $(LDLIBS)

# pubnub_advanced_history_sample: ../core/samples/pubnub_advanced_history_sample.c pubnub_sync.a
# 	gcc -o $@ $(CFLAGS) $(INCLUDES) ../core/samples/pubnub_advanced_history_sample.c pubnub_sync.a $(LDLIBS)

# cancel_subscribe_sync_sample: ../core/samples/cancel_subscribe_sync_sample.c pubnub_sync.a
# 	gcc -o $@ $(CFLAGS) $(INCLUDES) ../core/samples/cancel_subscribe_sync_sample.c pubnub_sync.a $(LDLIBS)

# pubnub_callback_sample: ../core/samples/pubnub_callback_sample.c pubnub_callback.a
# 	gcc -o $@ -D PUBNUB_CALLBACK_API $(CFLAGS) $(INCLUDES) ../core/samples/pubnub_callback_sample.c pubnub_callback.a $(LDLIBS)

# pubnub_callback_subloop_sample: ../core/samples/pubnub_callback_subloop_sample.c pubnub_callback.a
# 	gcc -o $@ -D PUBNUB_CALLBACK_API $(CFLAGS) $(INCLUDES) ../core/samples/pubnub_callback_subloop_sample.c pubnub_callback.a $(LDLIBS)

# subscribe_publish_callback_sample: ../core/samples/subscribe_publish_callback_sample.c pubnub_callback.a
# 	gcc -o $@ -D PUBNUB_CALLBACK_API $(CFLAGS) $(INCLUDES) ../core/samples/subscribe_publish_callback_sample.c pubnub_callback.a $(LDLIBS)

# subscribe_publish_from_callback: ../core/samples/subscribe_publish_from_callback.c pubnub_callback.a
# 	gcc -o $@ -D PUBNUB_CALLBACK_API $(CFLAGS) $(INCLUDES) ../core/samples/subscribe_publish_from_callback.c pubnub_callback.a $(LDLIBS)

# publish_callback_subloop_sample: ../core/samples/publish_callback_subloop_sample.c pubnub_callback.a
# 	gcc -o $@ -D PUBNUB_CALLBACK_API $(CFLAGS) $(INCLUDES) ../core/samples/publish_callback_subloop_sample.c pubnub_callback.a $(LDLIBS)

# publish_queue_callback_subloop: ../core/samples/publish_queue_callback_subloop.c pubnub_callback.a
# 	gcc -o $@ -D PUBNUB_CALLBACK_API $(CFLAGS) $(INCLUDES) ../core/samples/publish_queue_callback_subloop.c pubnub_callback.a $(LDLIBS)

# pubnub_fntest: ../core/fntest/pubnub_fntest.c ../core/fntest/pubnub_fntest_basic.c ../core/fntest/pubnub_fntest_medium.c fntest/pubnub_fntest_posix.c fntest/pubnub_fntest_runner.c pubnub_sync.a
# 	gcc -o $@ $(CFLAGS) $(INCLUDES) ../core/fntest/pubnub_fntest.c ../core/fntest/pubnub_fntest_basic.c ../core/fntest/pubnub_fntest_medium.c  fntest/pubnub_fntest_posix.c fntest/pubnub_fntest_runner.c pubnub_sync.a $(LDLIBS) -lpthread

# CONSOLE_SOURCEFILES=../core/samples/console/pubnub_console.c ../core/samples/console/pnc_helpers.c ../core/samples/console/pnc_readers.c ../core/samples/console/pnc_subscriptions.c

# pubnub_console_sync: $(CONSOLE_SOURCEFILES) ../core/samples/console/pnc_ops_sync.c pubnub_sync.a
# 	gcc -o $@ $(CFLAGS) $(INCLUDES) $(CONSOLE_SOURCEFILES) ../core/samples/console/pnc_ops_sync.c  pubnub_sync.a $(LDLIBS)

# pubnub_console_callback: $(CONSOLE_SOURCEFILES) ../core/samples/console/pnc_ops_callback.c pubnub_callback.a
# 	gcc -o $@ $(CFLAGS) -D PUBNUB_CALLBACK_API $(INCLUDES) $(CONSOLE_SOURCEFILES) ../core/samples/console/pnc_ops_callback.c pubnub_callback.a $(LDLIBS)


clean:
	rm $(BUILD_DIR)/pubnub_sync.a $(BUILD_DIR)/pubnub_callback.a
