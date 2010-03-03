#include "stdafx.h"
#include "ft.h"
#include "font_man.h"
#include "setting.h"
#include FT_LCD_FILTER_H

FT_Library ft_lib;
FTC_Manager ft_cache_man;
FTC_ImageCache ft_glyph_cache;

void initialize_freetype()
{
	FT_Error ft_error;

	ft_error = FT_Init_FreeType(&ft_lib);
	assert(ft_error == 0);

	ft_error = FT_Library_SetLcdFilter(ft_lib, (FT_LcdFilter) gdimm_setting::instance().get_setting_items().lcd_filter);
	assert(ft_error == 0);

	ft_error = FTC_Manager_New(ft_lib, 0, 0, 0, face_requester, NULL, &ft_cache_man);
	assert(ft_error == 0);

	ft_error = FTC_ImageCache_New(ft_cache_man, &ft_glyph_cache);
	assert(ft_error == 0);
}

void destroy_freetype()
{
	FT_Error ft_error;

	FTC_Manager_Done(ft_cache_man);
	
	ft_error = FT_Done_FreeType(ft_lib);
	assert(ft_error == 0);
}

FT_Error face_requester(FTC_FaceID face_id, FT_Library library, FT_Pointer request_data, FT_Face *aface)
{
	FT_Open_Args args;
	args.flags = FT_OPEN_STREAM;
	args.stream = gdimm_font_man::instance().get_font_stream((long) face_id);
	
	return FT_Open_Face(library, &args, 0, aface);
}