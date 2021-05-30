static TTF_Font *load_font() {
    char *p;
    const char *fname = "fm.ttf";
    const size_t size = 12;
    TTF_Font *font;
    /* start SDL_ttf */
    if(TTF_Init()==-1)
    {
	printf("TTF_Init: %s\n", TTF_GetError());
	exit(2);
    }
    font=TTF_OpenFont(fname, size);
    if(!font)
    {
 	printf("TTF_OpenFont: %s\n", TTF_GetError());
	exit(3);
    }
	printf("size                    : %d\n",size);
	printf("TTF_FontHeight          : %d\n",TTF_FontHeight(font));
	printf("TTF_FontAscent          : %d\n",TTF_FontAscent(font));
	printf("TTF_FontDescent         : %d\n",TTF_FontDescent(font));
	printf("TTF_FontLineSkip        : %d\n",TTF_FontLineSkip(font));
	printf("TTF_FontFaceIsFixedWidth: %d\n",TTF_FontFaceIsFixedWidth(font));
	{
		char *str=TTF_FontFaceFamilyName(font);
		if(!str)
			str="(null)";
		printf("TTF_FontFaceFamilyName  : \"%s\"\n",str);
	}
	{
		char *str=TTF_FontFaceStyleName(font);
		if(!str)
			str="(null)";
		printf("TTF_FontFaceStyleName   : \"%s\"\n",str);
	}
if(TTF_GlyphIsProvided(font,'g'))
	{
		int minx, maxx, miny, maxy, advance;
		TTF_GlyphMetrics(font,'g', &minx, &maxx, &miny, &maxy, &advance);
		printf("TTF_GlyphMetrics('g'):\n\tminx=%d\n\tmaxx=%d\n\tminy=%d\n\tmaxy=%d\n\tadvance=%d\n",
				minx, maxx, miny, maxy, advance);
	}
	else
		printf("TTF_GlyphMetrics('g'): unavailable in font!\n");

	/* set window title and icon name, using filename and stuff */
	p=strrchr(fname,'/');
	if(!p)
		p=strrchr(fname,'\\');
	if(!p)
		p=strrchr(fname,':');
	if(!p)
		p=fname;
	else
		p++;

	return font;
}

