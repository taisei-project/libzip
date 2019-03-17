project('libzip', 'c',
    version : '1.6.1',
    meson_version : '>=0.48.0',
    license : 'BSD-3-Clause',
)

cc = meson.get_compiler('c')

libzip_version = meson.project_version().split('.')
libzip_version_major = libzip_version[0]
libzip_version_minor = libzip_version[1]
libzip_version_micro = libzip_version[2]

config = configuration_data()
config.set('PACKAGE', meson.project_name())
config.set('PACKAGE_VERSION', meson.project_version())
config.set('PACKAGE_VERSION_MAJOR', libzip_version_major)
config.set('PACKAGE_VERSION_MINOR', libzip_version_minor)
config.set('PACKAGE_VERSION_MICRO', libzip_version_micro)
config.set('VERSION', meson.project_version())

if get_option('default_library') == 'static'
    config.set('ZIP_STATIC', true)
endif

dep_zlib = dependency('zlib', version : '>=1.1.2', required : true, fallback : ['zlib', 'zlib_dep'])

libzip_deps = [dep_zlib]
libzip_cflags = []

should_install = not meson.is_subproject()

use_bzip2 = false
if get_option('enable_bzip2') != 'false'
    require_bzip2 = get_option('enable_bzip2') == 'true'
    dep_bzip2 = dependency('bzip2', required : require_bzip2)

    if dep_bzip2.found()
        libzip_deps += dep_bzip2
        use_bzip2 = true
        config.set('HAVE_LIBBZ2', true)
    else
        warning('bzip2 library not found; bzip2 support disabled')
    endif
endif

use_lzma = false
if get_option('enable_lzma') != 'false'
    require_lzma = get_option('enable_lzma') == 'true'
    dep_lzma = dependency('liblzma', required : require_lzma)

    if dep_lzma.found()
        libzip_deps += dep_lzma
        use_lzma = true
        config.set('HAVE_LIBLZMA', true)
    else
        warning('lzma library not found; LZMA support disabled')
    endif
endif

crypto = 'none'
have_commoncrypto = false
have_gnutls = false
have_mbedtls = false
have_openssl = false
have_windows_crypto = false

if get_option('enable_crypto') != 'false'
    if get_option('enable_commoncrypto') != 'false'
        require_commoncrypto = get_option('enable_commoncrypto') == 'true'
        have_commoncrypto = cc.has_header('CommonCrypto/CommonCrypto.h')

        if require_commoncrypto
            assert(have_commoncrypto, 'CommonCrypto/CommonCrypto.h not found')
        endif
    endif

    if get_option('enable_gnutls') != 'false'
        require_gnutls = get_option('enable_gnutls') == 'true'
        gnutls_dep = dependency('gnutls', required : require_gnutls)
        nettle_dep = dependency('nettle', required : require_gnutls)
        have_gnutls = gnutls_dep.found() and nettle_dep.found()
    endif

    if get_option('enable_mbedtls') != 'false'
        require_mbedtls = get_option('enable_mbedtls') == 'true'
        mbedtls_dep = cc.find_library('mbedtls', required : require_mbedtls)
        have_mbedtls = cc.has_header('mbedtls/aes.h') and mbedtls_dep.found()

        if require_mbedtls
            assert(have_mbedtls, 'mbedtls/aes.h not found')
        endif
    endif

    if get_option('enable_openssl') != 'false'
        require_openssl = get_option('enable_openssl') == 'true'
        openssl_dep = dependency('libcrypto', required : require_openssl)
        have_openssl = openssl_dep.found()
    endif

    if get_option('enable_windows_crypto') != 'false' and host_machine.system() == 'windows'
        have_windows_crypto = true
    endif

    if have_commoncrypto
        config.set('HAVE_CRYPTO', true)
        config.set('HAVE_COMMONCRYPTO', true)
        crypto = 'commoncrypto'
    elif have_windows_crypto
        config.set('HAVE_CRYPTO', true)
        config.set('HAVE_WINDOWS_CRYPTO', true)
        crypto = 'win'
    elif have_gnutls
        config.set('HAVE_CRYPTO', true)
        config.set('HAVE_GNUTLS', true)
        libzip_deps += [gnutls_dep, nettle_dep]
        crypto = 'gnutls'
    elif have_openssl
        config.set('HAVE_CRYPTO', true)
        config.set('HAVE_OPENSSL', true)
        libzip_deps += openssl_dep
        crypto = 'openssl'
    elif have_mbedtls
        config.set('HAVE_CRYPTO', true)
        config.set('HAVE_MBEDTLS', true)
        libzip_deps += mbedtls_dep
        crypto = 'mbedtls'
    elif get_option('enable_crypto') == 'true'
        warning('neither Common Crypto, GnuTLS, mbed TLS, OpenSSL, nor Windows Cryptography found or enabled')
    else
        warning('neither Common Crypto, GnuTLS, mbed TLS, OpenSSL, nor Windows Cryptography found or enabled; AES support disabled')
    endif
endif

if crypto == 'none'
    message('No crypto support')
else
    message('Crypto backend: @0@'.format(crypto))
endif

if cc.get_id() == 'msvc'
    libzip_cflags += ['-D_CRT_SECURE_NO_WARNINGS', '-D_CRT_NONSTDC_NO_DEPRECATE']
endif

if host_machine.system() == 'windows'
    # TODO: port UWP support, not sure how to do that properly

    if have_windows_crypto
        libzip_deps += cc.find_library('bcrypt')
    endif

    libzip_deps += cc.find_library('advapi32')
endif

libzip_cflags += '-DHAVE_CONFIG_H'

check_functions = [
    '_chmod',
    '_close',
    '_dup',
    '_fdopen',
    '_fileno',
    '_open',
    '_setmode',
    '_snprintf',
    '_strdup',
    '_stricmp',
    '_strtoi64',
    '_strtoui64',
    '_umask',
    '_unlink',
    'arc4random',
    'clonefile',
    'explicit_bzero',
    'explicit_memset',
    'fileno',
    'fseeko',
    'ftello',
    'getprogname',
    'localtime_r',
    'open',
    'setmode',
    'snprintf',
    'strcasecmp',
    'strdup',
    'stricmp',
    'strtoll',
    'strtoull',
]

foreach function : check_functions
    config.set('HAVE_@0@'.format(function.to_upper()), cc.has_function(function))
endforeach

config.set('HAVE_FTS_H', (
    cc.has_header('sys/types.h') and
    cc.has_header('sys/stat.h') and
    cc.has_header('fts.h')
))

config.set('HAVE_STDBOOL_H', cc.has_header('stdbool.h'))
config.set('HAVE_STRINGS_H', cc.has_header('strings.h'))
config.set('HAVE_UNISTD_H', cc.has_header('unistd.h'))

stdint_includes = ''

foreach header : ['inttypes.h', 'stdint.h', 'sys/types.h']
    if cc.has_header(header)
        stdint_includes = '@0@#include <@1@>\n'.format(stdint_includes, header)
        config.set('HAVE_@0@_LIBZIP'.format(header.underscorify().to_upper()), true)
    endif
endforeach

check_types = [
    '__int16',
    '__int32',
    '__int64',
    '__int8',
    'int',
    'int16_t',
    'int32_t',
    'int64_t',
    'int8_t',
    'long long',
    'long',
    'off_t',
    'short',
    'size_t',
    'ssize_t',
    'uint16_t',
    'uint32_t',
    'uint64_t',
    'uint8_t',
]

foreach type : check_types
    tsize = cc.sizeof(type, prefix : stdint_includes)
    tupper = type.underscorify().to_upper()
    config.set('@0@_LIBZIP'.format(tupper), tsize)
    config.set('HAVE_@0@_LIBZIP'.format(tupper), tsize > -1)
endforeach

config.set('SIZEOF_OFF_T', config.get('OFF_T_LIBZIP'))

config.set('HAVE_FICLONERANGE', cc.has_header_symbol('linux/fs.h', 'FICLONERANGE'))
config.set('HAVE_NULLABLE', cc.compiles('''
int foo(char * _Nullable bar);
int main(int argc, char *argv[]) { }''', name : '_Nullable test'))

config.set('WORDS_BIGENDIAN', host_machine.endian() == 'big')

if config.get('HAVE_STDINT_H_LIBZIP')
    config.set('LIBZIP_TYPES_INCLUDE', '#include <stdint.h>')
elif config.get('HAVE_INTTYPES_H_LIBZIP')
    config.set('LIBZIP_TYPES_INCLUDE', '#include <inttypes.h>')
elif config.get('HAVE_SYS_TYPES_H_LIBZIP')
    config.set('LIBZIP_TYPES_INCLUDE', '#include <sys/types.h>')
endif

if config.get('HAVE_INT8_T_LIBZIP')
    config.set('ZIP_INT8_T', 'int8_t')
elif config.get('HAVE___INT8_LIBZIP')
    config.set('ZIP_INT8_T', '__int8')
else
    config.set('ZIP_INT8_T', 'signed char')
endif

if config.get('HAVE_UINT8_T_LIBZIP')
    config.set('ZIP_UINT8_T', 'uint8_t')
elif config.get('HAVE___INT8_LIBZIP')
    config.set('ZIP_UINT8_T', 'unsigned __int8')
else
    config.set('ZIP_UINT8_T', 'unsigned char')
endif

if config.get('HAVE_INT16_T_LIBZIP')
    config.set('ZIP_INT16_T', 'int16_t')
elif config.get('HAVE___INT16_LIBZIP')
    config.set('ZIP_INT16_T', '__int16')
elif config.get('SHORT_LIBZIP') == 2
    config.set('ZIP_INT16_T', 'short')
endif

if config.get('HAVE_UINT16_T_LIBZIP')
    config.set('ZIP_UINT16_T', 'uint16_t')
elif config.get('HAVE___INT16_LIBZIP')
    config.set('ZIP_UINT16_T', 'unsigned __int16')
elif config.get('SHORT_LIBZIP') == 2
    config.set('ZIP_UINT16_T', 'unsigned short')
endif

if config.get('HAVE_INT32_T_LIBZIP')
    config.set('ZIP_INT32_T', 'int32_t')
elif config.get('HAVE___INT32_LIBZIP')
    config.set('ZIP_INT32_T', '__int32')
elif config.get('INT_LIBZIP') == 4
    config.set('ZIP_INT32_T', 'int')
elif config.get('LONG_LIBZIP') == 4
    config.set('ZIP_INT32_T', 'long')
endif

if config.get('HAVE_UINT32_T_LIBZIP')
    config.set('ZIP_UINT32_T', 'uint32_t')
elif config.get('HAVE___INT32_LIBZIP')
    config.set('ZIP_UINT32_T', 'unsigned __int32')
elif config.get('INT_LIBZIP') == 4
    config.set('ZIP_UINT32_T', 'unsigned int')
elif config.get('LONG_LIBZIP') == 4
    config.set('ZIP_UINT32_T', 'unsigned long')
endif

if config.get('HAVE_INT64_T_LIBZIP')
    config.set('ZIP_INT64_T', 'int64_t')
elif config.get('HAVE___INT64_LIBZIP')
    config.set('ZIP_INT64_T', '__int64')
elif config.get('LONG_LIBZIP') == 8
    config.set('ZIP_INT64_T', 'long')
elif config.get('LONG_LONG_LIBZIP') == 8
    config.set('ZIP_INT64_T', 'long long')
endif

if config.get('HAVE_UINT64_T_LIBZIP')
    config.set('ZIP_UINT64_T', 'uint64_t')
elif config.get('HAVE___INT64_LIBZIP')
    config.set('ZIP_UINT64_T', 'unsigned __int64')
elif config.get('LONG_LIBZIP') == 8
    config.set('ZIP_UINT64_T', 'unsigned long')
elif config.get('LONG_LONG_LIBZIP') == 8
    config.set('ZIP_UINT64_T', 'unsigned long long')
endif

if config.get('HAVE_NULLABLE')
    config.set('ZIP_NULLABLE_DEFINES', '')
else
    config.set('ZIP_NULLABLE_DEFINES', '#define _Nullable\n#define _Nonnull')
endif

config_template = files('cmake-config.h.in')
zipconf_template = files('cmake-zipconf.h.in')

subdir('lib')

# TODO: docs, examples, tools, etc.
