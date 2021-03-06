CLEANFILES = .libs/mod_chxj *~

libmod_chxj_la_SOURCES = mod_chxj.c \
    qs_parse_string.c \
    qs_log.c \
    qs_malloc.c \
    qs_parse_tag.c \
    qs_parse_attr.c \
    qs_ignore_sp.c \
    qs_parse_file.c \
    chxj_load_device_data.c \
    chxj_load_emoji_data.c  \
    chxj_tag_util.c  \
    chxj_specified_device.c  \
    chxj_xhtml_mobile_1_0.c \
    chxj_hdml.c \
    chxj_chtml10.c \
    chxj_chtml20.c \
    chxj_chtml30.c  \
    chxj_chtml40.c  \
    chxj_chtml50.c  \
    chxj_ixhtml10.c \
    chxj_jhtml.c \
    chxj_jxhtml.c \
    chxj_iphone.c \
    chxj_android.c \
    chxj_str_util.c  \
    chxj_img_conv.c  \
    chxj_dump.c \
    chxj_img_conv_format.c \
    chxj_qr_code.c \
    chxj_calc_ecc.c \
    chxj_apply_convrule.c \
    chxj_encoding.c \
    chxj_node_convert.c \
    chxj_url_encode.c \
    chxj_preg_replace.c \
    chxj_cookie.c \
    chxj_mysql.c \
    chxj_memcache.c \
    chxj_dbm.c \
    chxj_buffered_write.c \
    chxj_apache.c \
    chxj_serf.c \
    chxj_dump_string.c \
    chxj_css.c \
    chxj_add_device_env.c \
    chxj_conv_z2h.c \
    chxj_conv_z2h_num.c \
    chxj_header_inf.c \
    chxj_jreserved_tag.c \
    chxj_google.c

SERF_OBJECTS = serf/buckets/aggregate_buckets.o \
          serf/buckets/request_buckets.o \
          serf/context.o \
          serf/buckets/buckets.o \
          serf/buckets/simple_buckets.o \
          serf/buckets/file_buckets.o \
          serf/buckets/mmap_buckets.o \
          serf/buckets/socket_buckets.o \
          serf/buckets/response_buckets.o \
          serf/buckets/headers_buckets.o \
          serf/buckets/allocator.o \
          serf/buckets/dechunk_buckets.o \
          serf/buckets/deflate_buckets.o \
          serf/buckets/limit_buckets.o \
          serf/buckets/ssl_buckets.o \
          serf/buckets/barrier_buckets.o \
          serf/buckets/chunk_buckets.o


libmod_chxj_la_CFLAGS = -O2 -prefer-pic @QDEFS@ @DDEFS@ -I. -I../include -I@with_apache_header@  -I./serf -I./css/include
AM_CFLAGS = @CFLAGS@ -O2 -prefer-pic @QDEFS@ @DDEFS@ -I. -I../include -I@with_apache_header@
libmod_chxj_la_LIBADD = serf/.libs/libserf-0.a css/src/.libs/libscss.a $(SERF_OBJECTS)
lib_LTLIBRARIES = libmod_chxj.la

make_so:
	@if test ! -L mod_chxj.so ; then ln -s .libs/libmod_chxj.so mod_chxj.so ; fi

serf/.libs/libserf-0.a:
	(cd serf; CFLAGS=-fPIC CXXFLAGS=-fPIC ./configure --enable-shared --with-apr=`dirname @APR_CONFIG@` --with-apr-util=`dirname @APU_CONFIG@` @OPENSSL_PATH@; LDFLAGS=-all-static make)

css/src/.libs/libscss.a:
	(cd css; ./configure --with-apr-config=@APR_CONFIG@ --with-apu-config=@APU_CONFIG@; make)

clean:
	rm -fr .libs
	rm -f *.o *.lo *.la
	rm -f mod_chxj.so
	rm -f serf/libserf-0.la
	@(cd serf; make clean)
	@(cd css; make clean)

install: make_so
	@@APXS@ -i -n "chxj" @PACKAGE_NAME@.so 

activate: make_so
	@@APXS@ -i -a -n "chxj" @PACKAGE_NAME@.so
