
libzip_sources = files(
    'zip_add.c',
    'zip_add_dir.c',
    'zip_add_entry.c',
    'zip_algorithm_deflate.c',
    'zip_buffer.c',
    'zip_close.c',
    'zip_delete.c',
    'zip_dir_add.c',
    'zip_dirent.c',
    'zip_discard.c',
    'zip_entry.c',
    'zip_err_str.c',
    'zip_error.c',
    'zip_error_clear.c',
    'zip_error_get.c',
    'zip_error_get_sys_type.c',
    'zip_error_strerror.c',
    'zip_error_to_str.c',
    'zip_extra_field.c',
    'zip_extra_field_api.c',
    'zip_fclose.c',
    'zip_fdopen.c',
    'zip_file_add.c',
    'zip_file_error_clear.c',
    'zip_file_error_get.c',
    'zip_file_get_comment.c',
    'zip_file_get_external_attributes.c',
    'zip_file_get_offset.c',
    'zip_file_rename.c',
    'zip_file_replace.c',
    'zip_file_set_comment.c',
    'zip_file_set_encryption.c',
    'zip_file_set_external_attributes.c',
    'zip_file_set_mtime.c',
    'zip_file_strerror.c',
    'zip_filerange_crc.c',
    'zip_fopen.c',
    'zip_fopen_encrypted.c',
    'zip_fopen_index.c',
    'zip_fopen_index_encrypted.c',
    'zip_fread.c',
    'zip_fseek.c',
    'zip_ftell.c',
    'zip_get_archive_comment.c',
    'zip_get_archive_flag.c',
    'zip_get_encryption_implementation.c',
    'zip_get_file_comment.c',
    'zip_get_name.c',
    'zip_get_num_entries.c',
    'zip_get_num_files.c',
    'zip_hash.c',
    'zip_io_util.c',
    'zip_libzip_version.c',
    'zip_memdup.c',
    'zip_name_locate.c',
    'zip_new.c',
    'zip_open.c',
    'zip_progress.c',
    'zip_rename.c',
    'zip_replace.c',
    'zip_set_archive_comment.c',
    'zip_set_archive_flag.c',
    'zip_set_default_password.c',
    'zip_set_file_comment.c',
    'zip_set_file_compression.c',
    'zip_set_name.c',
    'zip_source_begin_write.c',
    'zip_source_begin_write_cloning.c',
    'zip_source_buffer.c',
    'zip_source_call.c',
    'zip_source_close.c',
    'zip_source_commit_write.c',
    'zip_source_compress.c',
    'zip_source_crc.c',
    'zip_source_error.c',
    'zip_source_filep.c',
    'zip_source_free.c',
    'zip_source_function.c',
    'zip_source_get_compression_flags.c',
    'zip_source_is_deleted.c',
    'zip_source_layered.c',
    'zip_source_open.c',
    'zip_source_pkware.c',
    'zip_source_read.c',
    'zip_source_remove.c',
    'zip_source_rollback_write.c',
    'zip_source_seek.c',
    'zip_source_seek_write.c',
    'zip_source_stat.c',
    'zip_source_supports.c',
    'zip_source_tell.c',
    'zip_source_tell_write.c',
    'zip_source_window.c',
    'zip_source_write.c',
    'zip_source_zip.c',
    'zip_source_zip_new.c',
    'zip_stat.c',
    'zip_stat_index.c',
    'zip_stat_init.c',
    'zip_strerror.c',
    'zip_string.c',
    'zip_unchange.c',
    'zip_unchange_all.c',
    'zip_unchange_archive.c',
    'zip_unchange_data.c',
    'zip_utf-8.c',
)

if host_machine.system() == 'windows'
    libzip_opsys_sources = files(
        'zip_source_win32handle.c',
        'zip_source_win32utf8.c',
        'zip_source_win32w.c',
    )

    # TODO: handle UWP
else
    libzip_opsys_sources = files(
        'zip_source_file.c',
    )
endif

libzip_extra_sources = []
libzip_optional_sources = []

if not cc.has_function('mkstemp')
    libzip_extra_sources += files('mkstemp.c')
endif

if use_bzip2
    libzip_optional_sources += files('zip_algorithm_bzip2.c')
endif

if crypto != 'none'
    libzip_optional_sources += files(
        'zip_crypto_@0@.c'.format(crypto),
        'zip_winzip_aes.c',
        'zip_source_winzip_aes_decode.c',
        'zip_source_winzip_aes_encode.c',
    )
endif

config_file = configure_file(
    configuration : config,
    input : config_template,
    output : 'config.h',
    format : 'cmake',
)

zipconf_file = configure_file(
    configuration : config,
    input : zipconf_template,
    output : 'zipconf.h',
    format : 'cmake',
)

have_visibility_kwarg = meson.version().version_compare('>=0.48.0')

if not have_visibility_kwarg
    libzip_cflags += cc.get_supported_arguments('-fvisibility=hidden')
endif

libzip = library('zip',
    libzip_sources,
    libzip_extra_sources,
    libzip_opsys_sources,
    libzip_optional_sources,

    config_file,
    zipconf_file,

    dependencies : libzip_deps,
    c_args : libzip_cflags,
    install : should_install,
    gnu_symbol_visibility : 'hidden',
    soversion : '5',
    version : '5.0',
    override_options : ['c_std=none'],
)

if should_install
    pkg = import('pkgconfig')
    pkg.generate(libzip,
        name : 'libzip',
        description : 'libzip - library for handling zip archives',
    )

    install_data(zipconf_file, install_dir : get_option('includedir'))
    install_data('zip.h', install_dir : get_option('includedir'))
endif

libzip_dep = declare_dependency(
    include_directories : include_directories('.'),
    link_with : libzip,
    dependencies : libzip_deps,
    version : meson.project_version(),
)
