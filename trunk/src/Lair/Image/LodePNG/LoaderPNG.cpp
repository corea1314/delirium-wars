
#include "LoaderPNG.h"

#include "lodepng.h"

#include "../../Lair.h"

bool ImageLoaderPNG::Load( Image** out_pImage, const std::string& in_szFilename )
{
	const char* filename = in_szFilename.c_str();

	unsigned char* buffer;
	unsigned char* image;
	size_t buffersize, imagesize;
	LodePNG_Decoder decoder;

	LodePNG_loadFile(&buffer, &buffersize, filename); /*load the image file with given filename*/
	LodePNG_Decoder_init(&decoder);
	LodePNG_Decoder_decode(&decoder, &image, &imagesize, buffer, buffersize); /*decode the png*/

	/*load and decode*/
	/*if there's an error, display it, otherwise display information about the image*/
	if(decoder.error) 
	{
		Lair::GetLogMan()->Log("ImageLoaderPNG", "error %u: %s\n", decoder.error, LodePNG_error_text(decoder.error) );
		
		free(image);
		free(buffer);
		LodePNG_Decoder_cleanup(&decoder);

		return false;
	}
	else
	{
		*out_pImage = new Image( decoder.infoPng.width, decoder.infoPng.height, LodePNG_InfoColor_getBpp(&decoder.infoPng.color)/8, image );

		/*
		printf("w: %d\n", );
		printf("h: %d\n", );
		printf("bitDepth: %d\n", decoder.infoPng.color.bitDepth);
		printf("bpp: %d\n", );
		printf("colorChannels: %d\n", LodePNG_InfoColor_getChannels(&decoder.infoPng.color));
		printf("paletteSize: %d\n", decoder.infoPng.color.palettesize);
		printf("colorType: %d\n", decoder.infoPng.color.colorType);
		printf("compressionMethod: %d\n", decoder.infoPng.compressionMethod);
		printf("filterMethod: %d\n", decoder.infoPng.filterMethod);
		printf("interlaceMethod: %d\n", decoder.infoPng.interlaceMethod);
		for(i = 0; i < decoder.infoPng.text.num; i++)
			printf("%s: %s\n",  decoder.infoPng.text.keys[i], decoder.infoPng.text.strings[i]);
		for(i = 0; i < decoder.infoPng.itext.num; i++)
			printf("%s (%s %s) : %s\n",  decoder.infoPng.itext.keys[i], decoder.infoPng.itext.langtags[i], decoder.infoPng.itext.transkeys[i], decoder.infoPng.itext.strings[i]);
		if(decoder.infoPng.time_defined)
		{
			printf("modification time: %d-%02d-%02d %02d:%02d:%02d\n", decoder.infoPng.time.year, decoder.infoPng.time.month, 
				decoder.infoPng.time.day, decoder.infoPng.time.hour, decoder.infoPng.time.minute, decoder.infoPng.time.second);
		}
		if(decoder.infoPng.phys_defined)
		{
			printf("physical size: %d %d %d\n", decoder.infoPng.phys_x, decoder.infoPng.phys_y, (int)decoder.infoPng.phys_unit);
		}
		*/
	}

	// cleanup decoder
//	free(image);
	free(buffer);
	LodePNG_Decoder_cleanup(&decoder);

	return true;
};