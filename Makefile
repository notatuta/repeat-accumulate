all: images/original.png images/tanner-graph.png

images/tanner-graph.png: images/tanner-graph.pic
	pic2plot images/tanner-graph.pic -n -f 0.035 -T png --bitmap-size "900x900" | magick convert png:- -crop "900x350+0+0" images/tanner-graph.png	

images/original.png: images/original.pbm
	magick convert images/corrected_ra_01.pbm -scale "200%" images/corrected_ra_01.png
	magick convert images/corrected_ra_10.pbm -scale "200%" images/corrected_ra_10.png
	magick convert images/corrected_ra_20.pbm -scale "200%" images/corrected_ra_20.png
	magick convert images/corrected_ra_30.pbm -scale "200%" images/corrected_ra_30.png
	magick convert images/corrected_ra_40.pbm -scale "200%" images/corrected_ra_40.png
	magick convert images/corrected_ra.pbm -scale "200%" images/corrected_ra.png
	magick convert images/corrected_repeat.pbm -scale "200%" images/corrected_repeat.png
	magick convert images/received_ra.pgm -scale "200%" images/received_ra.png
	magick convert images/received_repeat.pgm -scale "200%" images/received_repeat.png
	magick convert images/original.pbm -scale "200%" images/original.png

images/original.pbm: repeat-accumulate
	./repeat-accumulate

repeat-accumulate: repeat-accumulate.cpp
	g++ -O3 -Wall -Wextra -std=c++11 -march=native -g -o repeat-accumulate repeat-accumulate.cpp

clean:
	-rm images/*.pbm images/*.pgm

