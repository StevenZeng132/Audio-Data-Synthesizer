#include <stdio.h>

#include "audio.h"
#include "debug.h"

int audio_read_header(FILE *in, AUDIO_HEADER *hp) {
    // TO BE IMPLEMENTED
    debug("hello");
    int magic_num=0;
    int data_offset=0;
    int data_size=0;
    int encoding=0;
    int sample_rate=0;
    int channels=0;
    for (int i=0; i<4; i++) {
    	magic_num = magic_num <<8;
    	magic_num += fgetc(in);
    }

    for(int i=0; i<4; i++) {
    	data_offset = data_offset <<8;
    	data_offset +=fgetc(in);
    }

    for(int i=0; i<4; i++) {
    	data_size = data_size<<8;
    	data_size +=fgetc(in);
    }

    for(int i=0; i<4; i++) {
    	encoding = encoding<<8;
    	encoding +=fgetc(in);
    }

    for(int i=0; i<4; i++) {
    	sample_rate = sample_rate<<8;
    	sample_rate += fgetc(in);
    }

    for(int i=0; i<4; i++) {
    	channels = channels <<8;
    	channels += fgetc(in);
    }

    if (magic_num != 779316836) {
    	return EOF;
    }

    if (data_offset != 24) {
    	return EOF;
    }

    if (data_size == 4294967295) {
    	return EOF;
    }

    if (encoding != 3) {
    	return EOF;
    }

    if (sample_rate != 8000) {
    	return EOF;
    }

    if (channels != 1) {
    	return EOF;
    }

    (*hp).magic_number = magic_num;
    (*hp).data_offset = data_offset;
    (*hp).data_size = data_size;
    (*hp).encoding = encoding;
    (*hp).sample_rate = sample_rate;
    (*hp).channels = channels;
    debug("%x\n", (*hp).magic_number);
    debug("%x\n", (*hp).data_offset);
    debug("%x\n", (*hp).data_size);
    debug("%x\n", (*hp).encoding);
    debug("%x\n", (*hp).sample_rate);
    debug("%x\n", (*hp).channels);
    return 0;
}

int audio_write_header(FILE *out, AUDIO_HEADER *hp) {
    // TO BE IMPLEMENTED
    debug("audio_write_header");
    int magic_num = ((*hp).magic_number);
  	int data_offset = ((*hp).data_offset);
  	int data_size = ((*hp).data_size);
  	int encoding = ((*hp).encoding);
  	int sample_rate = ((*hp).sample_rate);
  	int channels = ((*hp).channels);

  	if (magic_num != 779316836) {
    	return EOF;
    }

    if (data_offset != 24) {
    	return EOF;
    }

    if (data_size == 4294967295) {
    	return EOF;
    }

    if (encoding != 3) {
    	return EOF;
    }

    if (sample_rate != 8000) {
    	return EOF;
    }

    if (channels != 1) {
    	return EOF;
    }
    for(int i=3; i> -1; i--) {
    	int plug = ((magic_num >> (8*i)) & 0xff);
    	fputc(plug, out);
    }

    for(int i=3; i> -1; i--) {
    	int plug = ((data_offset >> (8*i)) & 0xff);
    	fputc(plug, out);
    }

    for(int i=3; i> -1; i--) {
    	int plug = ((data_size >> (8*i)) & 0xff);
    	fputc(plug, out);
    }

    for(int i=3; i> -1; i--) {
    	int plug = ((encoding >> (8*i)) & 0xff);
    	fputc(plug, out);
    }

    for(int i=3; i> -1; i--) {
    	int plug = ((sample_rate >> (8*i)) & 0xff);
    	fputc(plug, out);
    }

    for(int i=3; i> -1; i--) {
    	int plug = ((channels >> (8*i)) & 0xff);
    	fputc(plug, out);
    }
    return 0;
}

int audio_read_sample(FILE *in, int16_t *samplep) {
    // TO BE IMPLEMENTED
    debug("audio_read_sample");
    for(int i=0; i<24; i++) {
    	int y =fgetc(in);
    	if (y<0) {
    		return EOF;
    	}
    }

    int16_t x = 0;
    x+=fgetc(in);
    if (x < 0) {
    	return EOF;
    }
    x = x <<8;
    x+=fgetc(in);
    debug("%x\n", x);
    samplep = &x;
    debug("%x\n", *samplep);
    return 0;
}

int audio_write_sample(FILE *out, int16_t sample) {
    // TO BE IMPLEMENTED
    //debug("audio_write_sample");
    //debug("%x\n", sample);
    if (sample > 65535 || sample < 0) {
    	return EOF;
    }
    int x = sample >> 8;
    fputc(x, out);
    int y = sample & 0xff;
    fputc(y, out);
    return 0;
}
