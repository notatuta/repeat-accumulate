## Repeat-Accumulate Error Correction Code Example

Repeat-Accumulate error correction code is a flavor of [LDPC](https://en.wikipedia.org/wiki/Low-density_parity-check_code) with very simple encoder. Despite simplicity, it still approaches the theoretical [Shannon limit](https://en.wikipedia.org/wiki/Noisy-channel_coding_theorem).

In this toy example we will be transmitting the following 64x64 black and white image (scaled up 2x for clarity), column by column, from left to right:

![Original 64x64 bitmap, scaled up 2x for clarity](images/original.png)

Most real data will have less redundancy, but here redundancy is good because it will help see the errors.

As a baseline, try simplest possible error correction first: send the same data four times. To simulate channel noise, pull random numbers from normal distribution and add to the transmitted signal. In this example signal to noise ratio is kept at pretty sporty 1 dB.

![Repeated four times, with noise added](images/received_repeat.png)

On the receiving end, sum log likelihood ratios for each received bit to recover the original:

![Corrected by averaging four transmissions](images/corrected_repeat.png)

There are still over two hundred bits that flipped. Simple averaging works about the same. Not great.

Enter the **Repeat-Accumulate algorithm**:

* Repeat each input bit *Q* times

* Apply a random (but fixed) permutation, the same for each codeword. Receiver will need to know this permutation to decode

* Transmit running total of the permuted bits, modulo 2. If bits are *b<sub>1</sub>*, *b<sub>2</sub>*, *b<sub>3</sub>*, etc. then send *b<sub>1</sub>*, then *b<sub>1</sub>&oplus;b<sub>2</sub>*, then *b<sub>1</sub>&oplus;b<sub>2</sub>&oplus;b<sub>3</sub>*, and so on

In this example RA code is used as a systematic code with *Q=3*. First 64x64 transmitted *data bits* contain a copy of the orginal, followed by 64x64x3 *parity bits* that were created using the above logic.

Instead of sending all parity bits, it's possible to send every *A*th parity bit, with little change in decoding logic, but in this example *A=1*.

Here is the transmitted message after adding exactly the same noise as in the first example:

![Systematic repeat-accumulate code, with noise added](images/received_ra.png)

Decoding algorithm is iterative. Intermediate results after 1, 10, 20, 30, 40 iterations, and final decoded image:

![RA decoder output after 1 iteration](images/corrected_ra_01.png) ![RA decoder output after 10 iterations](images/corrected_ra_10.png) ![RA decoder output after 10 iterations](images/corrected_ra_10.png) ![RA decoder output after 20 iterations](images/corrected_ra_20.png) ![RA decoder output after 30 iterations](images/corrected_ra_30.png) ![RA decoder output after 40 iterations](images/corrected_ra_40.png) ![RA decoder output after 61 iterations](images/corrected_ra.png)

Decoding algorithm is borrowed from LDPC codes. It's using [passing messages](https://en.wikipedia.org/wiki/Belief_propagation) back and forth on [Tanner graph](https://en.wikipedia.org/wiki/Tanner_graph), in a way similar to [Viterbi algorithm](https://en.wikipedia.org/wiki/Viterbi_algorithm).

To build Tanner graph, start by noting that with *A=1* it is possible to get one original (permuted) bit by XORing two consecutive transmitted parity bits. For example, if *b<sub>1</sub>&oplus;b<sub>2</sub>* is followed by *b<sub>1</sub>&oplus;b<sub>2</sub>&oplus;b<sub>3</sub>*, then *(b<sub>1</sub>&oplus;b<sub>2</sub>)&oplus;(b<sub>1</sub>&oplus;b<sub>2</sub>&oplus;b<sub>3)</sub>=b<sub>3</sub>*. And if we XOR it with corresponding data bit, we should get a zero.

Similarly, if *A>1* then by XORing two censecutive parity bits we get XOR of *A* original (permuted) bits. And if we XOR that with corresponding data bits, we should again get a zero.

This allows to construct one parity check node for each parity bit. All parity check nodes have *A+2* inputs, except the first one that has *A+1* inputs (because first parity bit is transmitted as is).

![Tanner graph example](images/tanner-graph.png)

In this simplified Tanner graph with three bits of data and *Q=2*, the squares at the top represent check nodes, and circles at the bottom represent variable nodes.

Variable nodes are of two kinds: *parity bits* with two inputs (except the last one that has one input), and *data bits* with *Q* inputs. Here variable nodes 0 through 2 (white circles) are data bits, and 3 through 8 (gray circles) are parity bits.

Once all parity checks are satisfied (or we reach maximum number of iterations, which usually means that some errors cannot be corrected), compare values held by variable nodes for data bits with zero to recover original bits.

Random interleaver has nice theoretical properties, *on average* and *in the limit*, but intuitively it's clear that not all random interleavers are equally good. For example, interleaver that does nothing is theoretically possible, albeit unlikely, and cannot be expected to work well because one would want the interleaver to avoid placing copies of the same bit close together.

One way to enforce this requirement is to use *S*-random interleaver, where no input symbols within distance *S* appear within a distance of *S* in the output, with *S = Q*. The code in this example uses Fisher-Yates shuffle. Before every swap it checks if the resulting interleaver is *S*-random, and if it is not, takes a step back and retries.

More details on RA and other error correcting codes can be found [here](http://www.inference.org.uk/itprnn/book.pdf) (link goes straight to PDF).
