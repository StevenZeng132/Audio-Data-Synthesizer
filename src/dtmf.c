#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include "const.h"
#include "audio.h"
#include "dtmf.h"
#include "dtmf_static.h"
#include "goertzel.h"
#include "debug.h"

#ifdef _STRING_H
#error "Do not #include <string.h>. You will get a ZERO."
#endif

#ifdef _STRINGS_H
#error "Do not #include <strings.h>. You will get a ZERO."
#endif

#ifdef _CTYPE_H
#error "Do not #include <ctype.h>. You will get a ZERO."
#endif

int lookAtLine(FILE *events_in, int *si, int *ei, int *x, int *symbol, int *fr, int *fc) {
	while (*x!=9) {
    		*x = fgetc(events_in);
    		if (*x ==9) {
    			break;
    		}
    		if (*x == EOF) {
    			return -2;
    		}
    		*si = *si *10;
    		*si += (*x-48);
    	}
    	*x = 0;
    	while (*x!=9) {
    		*x = fgetc(events_in);
    		if (*x ==9) {
    			break;
    		}
    		*ei = *ei * 10;
    		*ei += (*x-48);
    	}
    	debug("%d\n", *si);
    	debug("%d\n", *ei);
    	*symbol = fgetc(events_in);
    	debug("%d\n", *symbol);
    	debug("Hello");
    	*x = fgetc(events_in);
    	if (*symbol != 48 && *symbol != 49 && *symbol != 50 && *symbol != 51 && *symbol != 52 && *symbol !=53 &&  *symbol !=54 && *symbol !=55 && *symbol !=56 && *symbol !=57 && *symbol !=65 && *symbol !=66 && *symbol !=67 && *symbol !=68 && *symbol !=42 && *symbol !=35) {
    		return -1;
    	}

    	if (*symbol == 48) {
    		*fc = 1336;
    		*fr = 941;
    	}

    	if (*symbol == 49) {
    		*fc = 1209;
    		*fr = 697;
    	}

    	if (*symbol == 50) {
    		*fc = 1336;
    		*fr = 697;
    	}

    	if (*symbol == 51) {
    		*fc = 1477;
    		*fr = 697;
    	}

    	if (*symbol == 52) {
    		*fc = 1209;
    		*fr = 770;
    	}

    	if (*symbol == 53) {
    		*fc = 1336;
    		*fr = 770;
    	}

    	if (*symbol == 54) {
    		*fc = 1477;
    		*fr = 770;
    	}

    	if (*symbol == 55) {
    		*fc = 1209;
    		*fr = 852;
    	}

    	if (*symbol == 56) {
    		*fc = 1336;
    		*fr = 852;
    	}

    	if (*symbol == 57) {
    		*fc = 1477;
    		*fr = 852;
    	}

    	if (*symbol == 65) {
    		*fc = 1633;
    		*fr = 697;
    	}

    	if (*symbol == 66) {
    		*fc = 1633;
    		*fr = 770;
    	}

    	if (*symbol == 67) {
    		*fc = 1633;
    		*fr = 852;
    	}

    	if (*symbol == 51) {
    		*fc = 1633;
    		*fr = 941;
    	}

    	if (*symbol == 42) {
    		*fc = 1209;
    		*fr = 941;
    	}

    	if (*symbol == 35) {
    		*fc = 1477;
    		*fr = 941;
    	}
    	return 0;
}

/*
 * You may modify this file and/or move the functions contained here
 * to other source files (except for main.c) as you wish.
 *
 * IMPORTANT: You MAY NOT use any array brackets (i.e. [ and ]) and
 * you MAY NOT declare any arrays or allocate any storage with malloc().
 * The purpose of this restriction is to force you to use pointers.
 * Variables to hold the pathname of the current file or directory
 * as well as other data have been pre-declared for you in const.h.
 * You must use those variables, rather than declaring your own.
 * IF YOU VIOLATE THIS RESTRICTION, YOU WILL GET A ZERO!
 */

/**
 * DTMF generation main function.
 * DTMF events are read (in textual tab-separated format) from the specified
 * input stream and audio data of a specified duration is written to the specified
 * output stream.  The DTMF events must be non-overlapping, in increasing order of
 * start index, and must lie completely within the specified duration.
 * The sample produced at a particular index will either be zero, if the index
 * does not lie between the start and end index of one of the DTMF events, or else
 * it will be a synthesized sample of the DTMF tone corresponding to the event in
 * which the index lies.
 *
 *  @param events_in  Stream from which to read DTMF events.
 *  @param audio_out  Stream to which to write audio header and sample data.
 *  @param length  Number of audio samples to be written.
 *  @return 0 if the header and specified number of samples are written successfully,
 *  EOF otherwise.
 */
int dtmf_generate(FILE *events_in, FILE *audio_out, uint32_t length) {
    // TO BE IMPLEMENTED
    debug("dtmf_generate");
    int data_size = length *2;
    AUDIO_HEADER hp = {779316836, 24, data_size, 3, 8000,1 };
    audio_write_header(audio_out, &hp);
    int si=0;
    int ei=0;
    int x = 0;
    int symbol;
    int fc;
    int fr;
    int check =0;
    for (int i=0; i<length; i++) {
    	if (i == ei) {
    		si =0;
    		ei=0;
    		int return1 = lookAtLine(events_in, &si, &ei, &x, &symbol, &fr, &fc);
    		x=0;
    		if (return1 == -1) {
    			return EOF;
    		}
    		if (return1 == -2) {
    			for (int j=i; j<length; j++) {
    				audio_write_sample(audio_out, 0);
    			}
    			return 0;
    		}
    	}

    	if (length < ei) {
    		return EOF;
    	}

    	//debug("%d\n", si);
    	//debug("%d\n", ei);

    	double cos1 = cos ( 2 * M_PI * fr * i / 8000);
    	double cos2 = cos (2 * M_PI * fc * i /8000);

    	int16_t cos3 = (cos1 + cos2) * 0.5 * INT16_MAX;

    	//debug("%d\n", cos3);

    	audio_write_sample(audio_out, cos3);
    	check ++;
    }
    debug("%d\n", check);
    return 0;
}

/**
 * DTMF detection main function.
 * This function first reads and validates an audio header from the specified input stream.
 * The value in the data size field of the header is ignored, as is any annotation data that
 * might occur after the header.
 *
 * This function then reads audio sample data from the input stream, partititions the audio samples
 * into successive blocks of block_size samples, and for each block determines whether or not
 * a DTMF tone is present in that block.  When a DTMF tone is detected in a block, the starting index
 * of that block is recorded as the beginning of a "DTMF event".  As long as the same DTMF tone is
 * present in subsequent blocks, the duration of the current DTMF event is extended.  As soon as a
 * block is encountered in which the same DTMF tone is not present, either because no DTMF tone is
 * present in that block or a different tone is present, then the starting index of that block
 * is recorded as the ending index of the current DTMF event.  If the duration of the now-completed
 * DTMF event is greater than or equal to MIN_DTMF_DURATION, then a line of text representing
 * this DTMF event in tab-separated format is emitted to the output stream. If the duration of the
 * DTMF event is less that MIN_DTMF_DURATION, then the event is discarded and nothing is emitted
 * to the output stream.  When the end of audio input is reached, then the total number of samples
 * read is used as the ending index of any current DTMF event and this final event is emitted
 * if its length is at least MIN_DTMF_DURATION.
 *
 *   @param audio_in  Input stream from which to read audio header and sample data.
 *   @param events_out  Output stream to which DTMF events are to be written.
 *   @return 0  If reading of audio and writing of DTMF events is sucessful, EOF otherwise.
 */
int dtmf_detect(FILE *audio_in, FILE *events_out) {
    // TO BE IMPLEMENTED
    return EOF;
}

/**
 * @brief Validates command line arguments passed to the program.
 * @details This function will validate all the arguments passed to the
 * program, returning 0 if validation succeeds and -1 if validation fails.
 * Upon successful return, the operation mode of the program (help, generate,
 * or detect) will be recorded in the global variable `global_options`,
 * where it will be accessible elsewhere in the program.
 * Global variables `audio_samples`, `noise file`, `noise_level`, and `block_size`
 * will also be set, either to values derived from specified `-t`, `-n`, `-l` and `-b`
 * options, or else to their default values.
 *
 * @param argc The number of arguments passed to the program from the CLI.
 * @param argv The argument strings passed to the program from the CLI.
 * @return 0 if validation succeeds and -1 if validation fails.
 * @modifies global variable "global_options" to contain a bitmap representing
 * the selected program operation mode, and global variables `audio_samples`,
 * `noise file`, `noise_level`, and `block_size` to contain values derived from
 * other option settings.
 */
int validargs(int argc, char **argv)
{
	if (argc == 1) {
		return -1;
	}

	char *secondArg = *(argv+1);
	debug("%s\n", secondArg);
	char *secondArg2 = *(argv+1)+1;
	char *secondArg3 = *(argv+1)+2;

	int num = (int)(*secondArg);
	int num2 = (int) (*secondArg2);
	int num3 = (int) (*secondArg3);

	/*
	-h argument
	*/
	if (num == 45 && num2 == 104 && num3 == 0) {
		global_options = global_options | 1;
		return 0;
	}

	/*
	-g argument
	*/
	if (num == 45 && num2 == 103 && num3 ==0) {
		debug("%s\n", "-g");

		if (argc == 2) {
			audio_samples = 8000;
			noise_level = 0;
			global_options = global_options | 2;
			return 0;
		}

		if (argc == 4) {
			debug("-g4");
			char *thirdArg = *(argv+2);
			char *thirdArg2 = *(argv+2)+1;
			char *thirdArg3 = *(argv+2)+2;
			num = (int) (*thirdArg);
			num2 = (int) (*thirdArg2);
			num3 = (int) (*thirdArg3);
			/*
			-t
			*/
			if (num == 45 && num2 == 116 && num3 == 0) {

				int MESC=0;
				char *storage = *(argv+3);
				while (((int)(*storage)) != 0) {
					if ( ((int)(*storage)) < 48 ||  ((int)(*storage)) > 57) {
						return -1;
					}
					MESC*=10;
					MESC += ((int)(*storage)) - 48;
					storage+=1;
				}
				MESC *= 8;
				if (MESC <0) {
					return -1;
				}
				audio_samples = MESC;
				noise_level = 0;
				global_options = global_options | 2;
				return 0;
			}
			/*
			-n
			*/
			if (num == 45 && num2 == 110 && num3 ==0) {
				debug("%s\n", "-n");
				char *storage = *(argv + 3);
				audio_samples = 8000;
				noise_level = 0;
				noise_file = storage;
				global_options = global_options | 2;
				return 0;

			}
			/*
			-l
			*/
			if (num == 45 && num2 == 108 && num3 ==0) {
				int lvl = 0;
				char *storage = *(argv + 3);
				while (((int)(*storage)) != 0) {
					if ( ((int)(*storage)) < 48 ||  ((int)(*storage)) > 57) {
						return -1;
					}
					lvl*=10;
					lvl += ((int)(*storage)) - 48;
					storage+=1;
				}
				if (lvl < -30 || lvl > 30) {
					return -1;
				}

				audio_samples = 8000;
				noise_level = lvl;
				global_options = global_options | 2;
				return 0;

			}
		}

		if (argc == 6) {
			char *thirdArg = *(argv + 2);
			char *fourthArg = *(argv + 3);
			char *fifthArg = *(argv + 4);
			char *sixthArg = *(argv+5);

			char firstChar = *(thirdArg+0);
			char secondChar = *(thirdArg+1);
			char thirdChar = *(thirdArg+2);

			/*
			-g -t -?
			*/
			if (firstChar == 45 && secondChar == 116 && thirdChar == 0) {
				debug("%s\n", "-g -t -?");
				int MESC = 0;
				while (((int)(*fourthArg)) != 0) {
					if ( ((int)(*fourthArg)) < 48 ||  ((int)(*fourthArg)) > 57) {
						return -1;
					}
					MESC*=10;
					MESC += ((int)(*fourthArg)) - 48;
					fourthArg+=1;
				}

				MESC*=8;
				if (MESC < 0) {
					return -1;
				}
				audio_samples = MESC;

				char firstChar1 = *(fifthArg+0);
				char secondChar1 = *(fifthArg+1);
				char thirdChar1 = *(fifthArg+2);
				/*
				-g -t -n
				*/
				if (firstChar1 == 45 && secondChar1 == 110 && thirdChar1 == 0) {
					debug("%s\n", "-g -t -n");
					noise_level =0;
					noise_file = sixthArg;
					global_options = global_options | 2;
					return 0;

				}
				/*
				-g -t -l
				*/
				if (firstChar1 == 45 && secondChar1 == 108 && thirdChar1 == 0) {
					debug("%s\n", "-g -t -l");
					int lvl = 0;
					while (((int)(*sixthArg)) != 0) {
						if ( ((int)(*sixthArg)) < 48 ||  ((int)(*sixthArg)) > 57) {
							return -1;
						}
						lvl*=10;
						lvl += ((int)(*sixthArg)) - 48;
						sixthArg+=1;
					}
					if (lvl < -30 || lvl > 30) {
						return -1;
					}
					noise_level = lvl;
					global_options = global_options | 2;
					noise_file = NULL;
					return 0;
				}
				return -1;
			}

			/*
			-g -n -?
			*/
			if (firstChar == 45 && secondChar == 110 && thirdChar == 0) {
				debug("%s\n", "-g -n -?");
				noise_file = fourthArg;

				char firstChar1 = *(fifthArg+0);
				char secondChar1 = *(fifthArg+1);
				char thirdChar1 = *(fifthArg+2);
				/*
				-g -n -t
				*/
				if (firstChar1 == 45 && secondChar1 == 116 && thirdChar1 == 0) {
					debug("%s\n", "-g -n -t");
					int MESC=0;

					while (((int)(*sixthArg)) != 0) {
						if ( ((int)(*sixthArg)) < 48 ||  ((int)(*sixthArg)) > 57) {
							return -1;
						}
						MESC*=10;
						MESC += ((int)(*sixthArg)) - 48;
						sixthArg+=1;
					}
					MESC*=8;
					if (MESC < 0) {
						return -1;
					}
					audio_samples = MESC;
					noise_level = 0;
					global_options = global_options | 2;
					return 0;



				}

				/*
				-g -n -l
				*/
				if (firstChar1 == 45 && secondChar1 == 108 && thirdChar1 == 0) {
					debug("%s\n", "-g -n -l");
					int lvl = 0;
					while (((int)(*sixthArg)) != 0) {
						if ( ((int)(*sixthArg)) < 48 ||  ((int)(*sixthArg)) > 57) {
							return -1;
						}
						lvl*=10;
						lvl += ((int)(*sixthArg)) - 48;
						sixthArg+=1;
					}
					if (lvl < -30 || lvl > 30) {
						return -1;
					}
					audio_samples = 8000;
					noise_level = lvl;
					global_options = global_options | 2;
					return 0;
				}

				return -1;

			}

			/*
			-g -l -?
			*/
			if (firstChar == 45 && secondChar == 108 && thirdChar == 0) {
				debug("%s\n", "-g -l -?");
				int lvl = 0;
				while (((int)(*fourthArg)) != 0) {
					if ( ((int)(*fourthArg)) < 48 ||  ((int)(*fourthArg)) > 57) {
						return -1;
					}
					lvl*=10;
					lvl += ((int)(*fourthArg)) - 48;
					fourthArg+=1;
				}
				if (lvl < -30 || lvl > 30) {
					return -1;
				}
				noise_level = lvl;
				char firstChar1 = *(fifthArg+0);
				char secondChar1 = *(fifthArg+1);
				char thirdChar1 = *(fifthArg+2);

				/*
				-g -l -n
				*/
				if (firstChar1 == 45 && secondChar1 == 110 && thirdChar1 == 0) {
					debug("%s\n", "-g -l -n");
					noise_file = sixthArg;
					audio_samples = 8000;
					global_options = global_options | 2;
					return 0;

				}

				/*
				-g -l -t
				*/
				if (firstChar1 == 45 && secondChar1 == 116 && thirdChar1 == 0) {
					debug("%s\n", "-g -l -t");
					int MESC=0;

					while (((int)(*sixthArg)) != 0) {
						if ( ((int)(*sixthArg)) < 48 ||  ((int)(*sixthArg)) > 57) {
							return -1;
						}
						MESC*=10;
						MESC += ((int)(*sixthArg)) - 48;
						sixthArg+=1;
					}
					MESC*=8;
					if (MESC < 0) {
						return -1;
					}
					audio_samples = MESC;
					noise_file = NULL;
					global_options = global_options | 2;
					return 0;

				}

				return -1;
			}

		}

		if (argc == 8) {
			char *thirdArg = *(argv + 2);
			char *fourthArg = *(argv + 3);
			char *fifthArg = *(argv + 4);
			char *sixthArg = *(argv+5);
			char *seventhArg = *(argv+6);
			char *eighthArg = *(argv+7);

			char firstChar = *(thirdArg+0);
			char secondChar = *(thirdArg+1);
			char thirdChar = *(thirdArg+2);



			//-g -t ? ?
			if (firstChar == 45 && secondChar == 116 && thirdChar==0) {
				debug("%s\n", "-g -t ? ?");
				char firstChar1 = *(fifthArg+0);
				char secondChar1 = *(fifthArg+1);
				char thirdChar1 = *(fifthArg+2);
				char firstChar2 = *(seventhArg+0);
				char secondChar2 =*(seventhArg+1);
				char thirdChar2 = *(seventhArg+2);

				int MESC = 0;
				while (((int)(*fourthArg)) != 0) {
					if ( ((int)(*fourthArg)) < 48 ||  ((int)(*fourthArg)) > 57) {
						return -1;
					}
					MESC*=10;
					MESC += ((int)(*fourthArg)) - 48;
					fourthArg+=1;
				}

				MESC*=8;
				if (MESC < 0) {
					return -1;
				}
				audio_samples = MESC;

				// -g -t -n -l
				if (firstChar1 == 45 && secondChar1 == 110 && thirdChar1 == 0 && firstChar2 == 45 && secondChar2 == 108 && thirdChar2 == 0) {
					debug("%s\n", "-g -t -n -l");
					noise_file = sixthArg;
					int lvl = 0;
					while (((int)(*eighthArg)) != 0) {
						if ( ((int)(*eighthArg)) < 48 ||  ((int)(*eighthArg)) > 57) {
							return -1;
						}
						lvl*=10;
						lvl += ((int)(*eighthArg)) - 48;
						eighthArg+=1;
					}
					if (lvl < -30 || lvl > 30) {
						return -1;
					}
					noise_level = lvl;
					global_options = global_options | 2;
					return 0;

				}

				// -g -t -l -n
				if (firstChar1 == 45 && secondChar1 == 108 && thirdChar1 == 0 && firstChar2 == 45 && secondChar2 == 110 && thirdChar2 == 0) {
					debug("%s\n", "-g -t -l -n");
					noise_file = eighthArg;
					int lvl = 0;
					while (((int)(*sixthArg)) != 0) {
						if ( ((int)(*sixthArg)) < 48 ||  ((int)(*sixthArg)) > 57) {
							return -1;
						}
						lvl*=10;
						lvl += ((int)(*sixthArg)) - 48;
						sixthArg+=1;
					}
					if (lvl < -30 || lvl > 30) {
						return -1;
					}
					noise_level = lvl;
					global_options = global_options | 2;
					return 0;

				}
				return -1;

			}

			//-g -n ? ?
			if (firstChar == 45 && secondChar == 110 && thirdChar == 0) {
				debug("%s\n", "-g -n ? ?");
				char firstChar1 = *(fifthArg+0);
				char secondChar1 = *(fifthArg+1);
				char thirdChar1 = *(fifthArg+2);
				char firstChar2 = *(seventhArg+0);
				char secondChar2 =*(seventhArg+1);
				char thirdChar2 = *(seventhArg+2);

				noise_file = fourthArg;

				// -g -n -t -l
				if (firstChar1 == 45 && secondChar1 == 116 && thirdChar1 == 0 && firstChar2 == 45 && secondChar2 == 108 && thirdChar2 == 0){
					debug("%s\n", "-g -n -t -l");
					int MESC = 0;
					while (((int)(*sixthArg)) != 0) {
						if ( ((int)(*sixthArg)) < 48 ||  ((int)(*sixthArg)) > 57) {
							return -1;
						}
						MESC*=10;
						MESC += ((int)(*sixthArg)) - 48;
						sixthArg+=1;
					}

					MESC*=8;
					if (MESC < 0) {
						return -1;
					}
					audio_samples = MESC;
					int lvl = 0;
					while (((int)(*eighthArg)) != 0) {
						if ( ((int)(*eighthArg)) < 48 ||  ((int)(*eighthArg)) > 57) {
							return -1;
						}
						lvl*=10;
						lvl += ((int)(*eighthArg)) - 48;
						eighthArg+=1;
					}
					if (lvl < -30 || lvl > 30) {
						return -1;
					}
					noise_level = lvl;
					global_options = global_options | 2;
					return 0;


				}

				// -g -n -l -t
				if (firstChar1 == 45 && secondChar1 == 108 && thirdChar1 == 0 && firstChar2 == 45 && secondChar2 == 116 && thirdChar2 == 0){
					debug("%s\n", "-g -n -l -t");
					int lvl = 0;
					while (((int)(*sixthArg)) != 0) {
						if ( ((int)(*sixthArg)) < 48 ||  ((int)(*sixthArg)) > 57) {
							return -1;
						}
						lvl*=10;
						lvl += ((int)(*sixthArg)) - 48;
						sixthArg+=1;
					}
					if (lvl < -30 || lvl > 30) {
						return -1;
					}
					noise_level = lvl;
					int MESC = 0;
					while (((int)(*eighthArg)) != 0) {
						if ( ((int)(*eighthArg)) < 48 ||  ((int)(*eighthArg)) > 57) {
							return -1;
						}
						MESC*=10;
						MESC += ((int)(*eighthArg)) - 48;
						eighthArg+=1;
					}

					MESC*=8;
					if (MESC < 0) {
						return -1;
					}
					audio_samples = MESC;
					global_options = global_options | 2;
					return 0;
				}

				return -1;
			}

			//-g -l ? ?
			if (firstChar == 45 && secondChar == 108 && thirdChar == 0) {
				debug("%s\n", "-g -l ? ?");
				char firstChar1 = *(fifthArg+0);
				char secondChar1 = *(fifthArg+1);
				char thirdChar1 = *(fifthArg+2);
				char firstChar2 = *(seventhArg+0);
				char secondChar2 =*(seventhArg+1);
				char thirdChar2 = *(seventhArg+2);

				int lvl = 0;
				while (((int)(*fourthArg)) != 0) {
					if ( ((int)(*fourthArg)) < 48 ||  ((int)(*fourthArg)) > 57) {
						return -1;
					}
					lvl*=10;
					lvl += ((int)(*fourthArg)) - 48;
					fourthArg+=1;
				}
				if (lvl < -30 || lvl > 30) {
					return -1;
				}
				noise_level = lvl;

				// -g -l -n -t
				if (firstChar1 == 45 && secondChar1 == 110 && thirdChar1 == 0 && firstChar2 == 45 && secondChar2 == 116 && thirdChar2 == 0){
					debug("%s\n", "-g -l -n -t");
					noise_file = sixthArg;
					int MESC = 0;
					while (((int)(*eighthArg)) != 0) {
						if ( ((int)(*eighthArg)) < 48 ||  ((int)(*eighthArg)) > 57) {
							return -1;
						}
						MESC*=10;
						MESC += ((int)(*eighthArg)) - 48;
						eighthArg+=1;
					}

					MESC*=8;
					if (MESC < 0) {
						return -1;
					}
					audio_samples = MESC;
					global_options = global_options | 2;
					return 0;
				}

				// -g -l -t -n
				if (firstChar1 == 45 && secondChar1 == 116 && thirdChar1 == 0 && firstChar2 == 45 && secondChar2 == 110 && thirdChar2 == 0){
					debug("%s\n", "-g -l -t -n");
					noise_file = eighthArg;
					int MESC = 0;
					while (((int)(*sixthArg)) != 0) {
						if ( ((int)(*sixthArg)) < 48 ||  ((int)(*sixthArg)) > 57) {
							return -1;
						}
						MESC*=10;
						MESC += ((int)(*sixthArg)) - 48;
						sixthArg+=1;
					}

					MESC*=8;
					if (MESC < 0) {
						return -1;
					}
					audio_samples = MESC;
					global_options = global_options | 2;
					return 0;
				}
				return -1;
			}

			return -1;

		}
	}
	/*
	-d argument
	*/
	if (num == 45 && num2 == 100 && num3 == 0) {

		if (argc == 2) {
			block_size = 100;
			global_options = global_options | 2;
			return 0;
		}

		if (argc == 4) {
			char *thirdArg = *(argv+2);
			char *thirdArg2 = *(argv+2)+1;
			char *thirdArg3 = *(argv+2)+2;
			num = (int) (*thirdArg);
			num2 = (int) (*thirdArg2);
			num3 = (int) (*thirdArg3);

			if (num != 45 || num2 != 98 || num3 != 0) {
				return -1;
			}

			int bs=0;
			char *storage = *(argv+3);

			while(((int)(*storage)) != 0) {
				if ( ((int)(*storage)) < 48 ||  ((int)(*storage)) > 57) {
					return -1;
				}
				bs*=10;
				bs += ((int)(*storage)) - 48;
				storage+=1;
			}
			if (bs < 10 || bs > 1000) {
				return -1;
			}
			block_size = bs;
			global_options = global_options | 4;
			return 0;
		}

		return -1;
	}



    // TO BE IMPLEMENTED
    return -1;
}
