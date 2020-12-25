all: images/original.png

images/original.png: images/original.pbm
	magick convert images/corrected_ra_01.pbm images/corrected_ra_01.png
	magick convert images/corrected_ra_05.pbm images/corrected_ra_05.png
	magick convert images/corrected_ra_10.pbm images/corrected_ra_10.png
	magick convert images/corrected_ra_15.pbm images/corrected_ra_15.png
	magick convert images/corrected_ra_20.pbm images/corrected_ra_20.png
	magick convert images/corrected_ra_25.pbm images/corrected_ra_25.png
	magick convert images/corrected_repeat.pbm images/corrected_repeat.png
	magick convert images/received_ra.pgm images/received_ra.png
	magick convert images/received_repeat.pgm images/received_repeat.png
	magick convert images/original.pbm images/original.png

images/original.pbm: repeat-accumulate
	./repeat-accumulate

repeat-accumulate: repeat-accumulate.cpp
	g++ -O3 -Wall -Wextra -std=c++11 -march=native -g -o repeat-accumulate repeat-accumulate.cpp

clean:
	-rm images/*.pbm images/*.pgm

