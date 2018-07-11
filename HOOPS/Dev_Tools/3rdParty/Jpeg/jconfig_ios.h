/* jconfig.h.  Generated automatically by configure.  */
/* jconfig.cfg --- source file edited by configure script */
/* see jconfig.doc for explanations */

#define HAVE_PROTOTYPES 
#define HAVE_UNSIGNED_CHAR 
#define HAVE_UNSIGNED_SHORT 
#undef void
#undef const
#undef CHAR_IS_UNSIGNED
#define HAVE_STDDEF_H 
#define HAVE_STDLIB_H 
#undef NEED_BSD_STRINGS
#undef NEED_SYS_TYPES_H
#undef NEED_FAR_POINTERS
#undef NEED_SHORT_EXTERNAL_NAMES
/* Define this if you get warnings about undefined structures. */
#undef INCOMPLETE_TYPES_BROKEN

#ifdef JPEG_INTERNALS

#undef RIGHT_SHIFT_IS_UNSIGNED
#define INLINE __inline__
/* These are for configuring the JPEG memory manager. */
#undef DEFAULT_MAX_MEM
#undef NO_MKTEMP

#endif /* JPEG_INTERNALS */

#ifdef JPEG_CJPEG_DJPEG

#define BMP_SUPPORTED		/* BMP image file format */
#define GIF_SUPPORTED		/* GIF image file format */
#define PPM_SUPPORTED		/* PBMPLUS PPM/PGM image file format */
#undef RLE_SUPPORTED		/* Utah RLE image file format */
#define TARGA_SUPPORTED		/* Targa image file format */

#undef TWO_FILE_COMMANDLINE
#undef NEED_SIGNAL_CATCHER
#undef DONT_USE_B_MODE

/* Define this if you want percent-done progress reports from cjpeg/djpeg. */
#undef PROGRESS_REPORT

#endif /* JPEG_CJPEG_DJPEG */

#ifndef REDEFINE_JPEG_NAMES
#define REDEFINE_JPEG_NAMES 1
#define jpeg_CreateCompress hoops_jpeg_CreateCompress
#define jpeg_abort_compress hoops_jpeg_abort_compress
#define jpeg_destroy_compress hoops_jpeg_destroy_compress
#define jpeg_finish_compress hoops_jpeg_finish_compress
#define jpeg_suppress_tables hoops_jpeg_suppress_tables
#define jpeg_write_m_byte hoops_jpeg_write_m_byte
#define jpeg_write_m_header hoops_jpeg_write_m_header
#define jpeg_write_marker hoops_jpeg_write_marker
#define jpeg_write_tables hoops_jpeg_write_tables
#define jpeg_start_compress hoops_jpeg_start_compress
#define jpeg_write_raw_data hoops_jpeg_write_raw_data
#define jpeg_write_scanlines hoops_jpeg_write_scanlines
#define jinit_c_coef_controller hoops_jinit_c_coef_controller
#define jinit_color_converter hoops_jinit_color_converter
#define jinit_forward_dct hoops_jinit_forward_dct
#define jinit_huff_encoder hoops_jinit_huff_encoder
#define jpeg_gen_optimal_table hoops_jpeg_gen_optimal_table
#define jpeg_make_c_derived_tbl hoops_jpeg_make_c_derived_tbl
#define jinit_compress_master hoops_jinit_compress_master
#define jinit_c_main_controller hoops_jinit_c_main_controller
#define jinit_marker_writer hoops_jinit_marker_writer
#define jinit_c_master_control hoops_jinit_c_master_control
#define jpeg_abort hoops_jpeg_abort
#define jpeg_alloc_huff_table hoops_jpeg_alloc_huff_table
#define jpeg_alloc_quant_table hoops_jpeg_alloc_quant_table
#define jpeg_destroy hoops_jpeg_destroy
#define jpeg_add_quant_table hoops_jpeg_add_quant_table
#define jpeg_default_colorspace hoops_jpeg_default_colorspace
#define jpeg_quality_scaling hoops_jpeg_quality_scaling
#define jpeg_set_colorspace hoops_jpeg_set_colorspace
#define jpeg_set_defaults hoops_jpeg_set_defaults
#define jpeg_set_linear_quality hoops_jpeg_set_linear_quality
#define jpeg_set_quality hoops_jpeg_set_quality
#define jpeg_simple_progression hoops_jpeg_simple_progression
#define jinit_phuff_encoder hoops_jinit_phuff_encoder
#define jinit_c_prep_controller hoops_jinit_c_prep_controller
#define jinit_downsampler hoops_jinit_downsampler
#define jpeg_copy_critical_parameters hoops_jpeg_copy_critical_parameters
#define jpeg_write_coefficients hoops_jpeg_write_coefficients
#define jpeg_CreateDecompress hoops_jpeg_CreateDecompress
#define jpeg_abort_decompress hoops_jpeg_abort_decompress
#define jpeg_consume_input hoops_jpeg_consume_input
#define jpeg_destroy_decompress hoops_jpeg_destroy_decompress
#define jpeg_finish_decompress hoops_jpeg_finish_decompress
#define jpeg_has_multiple_scans hoops_jpeg_has_multiple_scans
#define jpeg_input_complete hoops_jpeg_input_complete
#define jpeg_read_header hoops_jpeg_read_header
#define jpeg_finish_output hoops_jpeg_finish_output
#define jpeg_read_raw_data hoops_jpeg_read_raw_data
#define jpeg_read_scanlines hoops_jpeg_read_scanlines
#define jpeg_start_decompress hoops_jpeg_start_decompress
#define jpeg_start_output hoops_jpeg_start_output
#define jpeg_stdio_dest hoops_jpeg_stdio_dest
#define jpeg_stdio_src hoops_jpeg_stdio_src
#define jinit_d_coef_controller hoops_jinit_d_coef_controller
#define jinit_color_deconverter hoops_jinit_color_deconverter
#define jinit_inverse_dct hoops_jinit_inverse_dct
#define jinit_huff_decoder hoops_jinit_huff_decoder
#define jpeg_fill_bit_buffer hoops_jpeg_fill_bit_buffer
#define jpeg_huff_decode hoops_jpeg_huff_decode
#define jpeg_make_d_derived_tbl hoops_jpeg_make_d_derived_tbl
#define jinit_input_controller hoops_jinit_input_controller
#define jinit_d_main_controller hoops_jinit_d_main_controller
#define jinit_marker_reader hoops_jinit_marker_reader
#define jpeg_resync_to_restart hoops_jpeg_resync_to_restart
#define jpeg_save_markers hoops_jpeg_save_markers
#define jpeg_set_marker_processor hoops_jpeg_set_marker_processor
#define jinit_master_decompress hoops_jinit_master_decompress
#define jpeg_calc_output_dimensions hoops_jpeg_calc_output_dimensions
#define jpeg_new_colormap hoops_jpeg_new_colormap
#define jinit_merged_upsampler hoops_jinit_merged_upsampler
#define jinit_phuff_decoder hoops_jinit_phuff_decoder
#define jinit_d_post_controller hoops_jinit_d_post_controller
#define jinit_upsampler hoops_jinit_upsampler
#define jpeg_read_coefficients hoops_jpeg_read_coefficients
#define jpeg_std_error hoops_jpeg_std_error
#define jpeg_fdct_float hoops_jpeg_fdct_float
#define jpeg_fdct_ifast hoops_jpeg_fdct_ifast
#define jpeg_fdct_islow hoops_jpeg_fdct_islow
#define jpeg_idct_float hoops_jpeg_idct_float
#define jpeg_idct_ifast hoops_jpeg_idct_ifast
#define jpeg_idct_islow hoops_jpeg_idct_islow
#define jpeg_idct_1x1 hoops_jpeg_idct_1x1
#define jpeg_idct_2x2 hoops_jpeg_idct_2x2
#define jpeg_idct_4x4 hoops_jpeg_idct_4x4
#define jinit_memory_mgr hoops_jinit_memory_mgr
#define jpeg_free_large hoops_jpeg_free_large
#define jpeg_free_small hoops_jpeg_free_small
#define jpeg_get_large hoops_jpeg_get_large
#define jpeg_get_small hoops_jpeg_get_small
#define jpeg_mem_available hoops_jpeg_mem_available
#define jpeg_mem_init hoops_jpeg_mem_init
#define jpeg_mem_term hoops_jpeg_mem_term
#define jpeg_open_backing_store hoops_jpeg_open_backing_store
#define jinit_1pass_quantizer hoops_jinit_1pass_quantizer
#define jinit_2pass_quantizer hoops_jinit_2pass_quantizer
#define jcopy_block_row hoops_jcopy_block_row
#define jcopy_sample_rows hoops_jcopy_sample_rows
#define jdiv_round_up hoops_jdiv_round_up
#define jround_up hoops_jround_up
#define jzero_far hoops_jzero_far
#define jpeg_std_message_table hoops_jpeg_std_message_table
#define jpeg_natural_order hoops_jpeg_natural_order


#endif

