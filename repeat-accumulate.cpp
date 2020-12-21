//
// Repeat-accumulate error correction code example
//
#include <algorithm>
#include <bitset>
#include <cmath>
#include <random>
#include <stdio.h>

const int Q = 3; // number of bit repetitions
const int B = 64 * 64; // number of information bits

std::bitset<64 * 64> get_original_bits(void)
{
  const char cat_pbm[] = "P1\n"
  "64 64\n"
  "0000000000000000000000000000000000000000000000000000000000000000\n"
  "0000000000000000000000000000000000000000000000000000000000000000\n"
  "0000000000000000000000000000000000000000000000000000000000000000\n"
  "0000000000000000000000000000000000000000000000000000000000000000\n"
  "0000000000000000000000000000000000000000000000000000000000000000\n"
  "0000000000000000000000000000000000000000000000000000000000000000\n"
  "0000000001111000000000000000000000000000000000000000000000000000\n"
  "0000000011111110000000000000000000000000000000000000000000000000\n"
  "0000000111111111100000000000000000000000000000000000000000000000\n"
  "0000001111111111100000000000000000000000000000000000000000000000\n"
  "0000011111000111100000000000000000000000000000000000000000000000\n"
  "0000011110000000000000000000000000000000000000000000000000000000\n"
  "0000011110000000000000000000000000000000000000000000000000000000\n"
  "0000011100000000000000000000000000000000000000000000000000000000\n"
  "0000111100000000000000000000000000000000000000000000000000000000\n"
  "0000111100000000000000000000000000000000000000000000000000000000\n"
  "0000111100000000000000000000000000000000000000000000000000000000\n"
  "0000111100000000000000000000000000000000000000000000000000000000\n"
  "0000011100000000000000000000000000000000000000000000000000000000\n"
  "0000011110000000000000000000000000000000000000000000000000000000\n"
  "0000011110000000000000000000000000000000000000000000000100000000\n"
  "0000011110000000000000000000000000000000000000000000001100000000\n"
  "0000001111000000000000000000000000000000000000000000011100000000\n"
  "0000001111000000000000000000000000000000000000000000111110000000\n"
  "0000000111100000000000000000000000000000000000000011111111000000\n"
  "0000000111110000000000000000000000000000000000000111111111100000\n"
  "0000000011111111111111111111111100000000000000001111111111110000\n"
  "0000000001111111111111111111111111111110000001111111111111111000\n"
  "0000000001111111111111111111111111111111111111111111111111111100\n"
  "0000000000111111111111111111111111111111111111111111111111111100\n"
  "0000000001111111111111111111111111111111111111111111111111111000\n"
  "0000000001111111111111111111111111111111111111111111111111111000\n"
  "0000000011111111111111111111111111111111111111111111111100000000\n"
  "0000000011111111111111111111111111111111111111111111111000000000\n"
  "0000000011111111111111111111111111111111111111111111110000000000\n"
  "0000000011111111111111111111111111111111111111111111110000000000\n"
  "0000000011111111111111111111111111111111111111111111100000000000\n"
  "0000000011111111111111111111111111111111111111111111100000000000\n"
  "0000000011111111111111111111111111111111111111111111100000000000\n"
  "0000000011111111111111111111111111111111111111111111100000000000\n"
  "0000000111111111111111111111111111111111111111111111100000000000\n"
  "0000001111111111111111111111111111111111111111111111100000000000\n"
  "0000111111111111111111111111111111111111111111111111100000000000\n"
  "0001111111111111111111111111111111111111111111111111100000000000\n"
  "0011111111111111111111111100000000111111111111111111110000000000\n"
  "0011111111110111111110000000000000111111000000111111110000000000\n"
  "0011111110000111111110000000000000111111000000011111110000000000\n"
  "0011110000000111111100000000000000111110000000001111111000000000\n"
  "0111100000000111111000000000000000111110000000001111111000000000\n"
  "0111000000000111111000000000000000111100000000000111111100000000\n"
  "0111000000000111110000000000000000111100000000000011111110000000\n"
  "1111000000000111100000000000000001111000000000000001111111000000\n"
  "1110000000000111110000000000000001111000000000000000011111100000\n"
  "1110000000000011110000000000000001110000000000000000001111110000\n"
  "1110000000000001111000000000000001110000000000000000000111111000\n"
  "0110000000000000111100000000000001111000000000000000000001111111\n"
  "0000000000000000011111000000000001111000000000000000000000111111\n"
  "0000000000000000001111000000000000000000000000000000000000000000\n"
  "0000000000000000000000000000000000000000000000000000000000000000\n"
  "0000000000000000000000000000000000000000000000000000000000000000\n"
  "0000000000000000000000000000000000000000000000000000000000000000\n"
  "0000000000000000000000000000000000000000000000000000000000000000\n"
  "0000000000000000000000000000000000000000000000000000000000000000\n"
  "0000000000000000000000000000000000000000000000000000000000000000\n";
  std::bitset<64 * 64> bits;
  for (int row = 0; row < 64; row++) {
    for (int col = 0; col < 64; col++) {
      bits[row * 64 + col] = cat_pbm[9 + row * 65 + col] == '0' ? 0 : 1;
    }
  }
  return bits;
}

void write_pbm(const char* filename, const std::bitset<64 * 64> bits)
{
  FILE* f = fopen(filename, "w");
  if (f) {
    fprintf(f, "P1\n64 64\n");
    int i = 0;
    for (int row = 0; row < 64; row++) {
      for (int col = 0; col < 64; col++) {
        fprintf(f, "%c", bits[i++] ? '1' : '0');
      }
      fprintf(f, "\n");
    }
    fclose(f);
  } else {
    fprintf(stderr, "Cannot open file %s\n", filename);
  }
}

void write_pgm(const char* filename, const double pieces[64 * 64 * (Q + 1)])
{
  FILE* f = fopen(filename, "w");
  if (f) {
    fprintf(f, "P2\n128 128\n255\n");
    for (int row = 0; row < 128; row++) {
      for (int col = 0; col < 128; col++) {
        int grid_row = row / 64;
        int grid_col = col / 64;
        int frame_row = row % 64;
        int frame_col = col % 64;
        int index = (grid_row * 2 + grid_col) * 64 * 64 + frame_row * 64 + frame_col;
        double intensity = (2. - pieces[index]) * 256 / 4;
        if (intensity < 0) intensity = 0;
        if (intensity > 255) intensity = 255;
        fprintf(f, "%d ", (int)intensity); // Black is 0, white is 255
      }
      fprintf(f, "\n");
    }
    fclose(f);
  } else {
    fprintf(stderr, "Cannot open file %s\n", filename);
  }
}

class RepeatAccumulateEncoder
{
  public:
    RepeatAccumulateEncoder()
    {
      // Start with ordered sequence,
      for (int i = 0; i < B * Q; i++) {
        order_[i] = i;
      }
      // then shuffle it
      std::default_random_engine gen(123);
      std::shuffle(order_, order_ + B * Q, gen);
    }

    // Decoder will need to use the same order
    const int (&order())[B * Q] { return order_; }

    // Sends original information bits at the end
    std::bitset<B * (Q + 1)> encode(const std::bitset<B> input)
    {
      // Repeat every input bit Q times
      std::bitset<Q * B> repeated;
      for (int i = 0; i < B; i++) {
        for (int j = 0; j < Q; j++) {
          repeated[Q * i + j] = input[i];
        }
      }
  
      // Interleave
      std::bitset<B * (Q + 1)> output;
      for (int i = 0; i < B * Q; i++) {
        output[i] = repeated[order_[i]];
      }
  
      // Accumulate
      bool d = false;
      for (int i = 0; i < B * Q; i++) {
        d ^= output[i];
        output[i] = d;
      }

      // Append systematic part (original data)
      for (int i = 0; i < B; i++) { 
        output[B * Q + i] = input[i];
      }

      return output;
    }

private:
    int order_[B * Q];
};

class RepeatAccumulateDecoder
{
  public:
    RepeatAccumulateDecoder(const int (&order)[B * Q])
    {
      // Degrees of variable nodes
      for (int i = 0; i < B * Q - 1; i++) {
        vd_[i] = 2;
      }
      vd_[B * Q - 1] = 1; 
      for (int i = B * Q; i < B * (Q + 1); i++) {
        vd_[i] = Q;
      }

      // Degrees of check nodes
      cd_[0] = 2;
      for (int i = 1; i < B * Q; i++) {
        cd_[i] = 3;
      }

      // Variable nodes
      for (int i = 0; i < B * Q; i++) {
        for (int k = 0; k < vd_[i]; k++) {
          vn_[i][k] = i + k;
        }
        vn_[order[i] / Q + B * Q][order[i] % Q] = i;
      }

      // Check nodes
      for (int i = 0; i < B * Q; i++) {
        for (int k = 0; k < cd_[i]; k++) {
          int j = (k == cd_[i] - 1) ? (order[i] / Q) + B * Q : i - k;
          cn_[i][k] = j;
          for (int m = 0; m < vd_[j]; m++) {
            if (vn_[j][m] == i) {
              vn_back_[j][m] = k;
              cn_back_[i][k] = m;
              goto found;
            }
          }
          perror("Can't tie back connection\n");
          exit(-1);
found:    ;
        }
      }
    }

    std::bitset<B> decode(const double llr[B * (Q + 1)], int itemax = 100)
    {
      double beta[B * Q + B][Q] = {};
      double alpha[B * Q][3] = {};
      int tmp_ans[B * (Q + 1)];
      for (int iteration = 0; iteration < itemax; iteration++) {
        if (iteration > 0) {
          for (int i = 0; i < B * Q; i++) {
            for (int n = 0; n < cd_[i]; n++) { // will update alpha for this node
              double beta_prod = 1.0;
              for (int k = 0; k < cd_[i]; k++) { // loop over nodes connected to this one
                if (k != n) { // not count connections to itself
                  int focus = cn_[i][k];
                  int tmp = cn_back_[i][k];
                  beta_prod *= tanh(0.5 * beta[focus][tmp]);
                }
              }
              if (beta_prod > 0.9999999999999999) {
                beta_prod = 0.9999999999999999;
              } else if (beta_prod < -0.9999999999999999) {
                beta_prod = -0.9999999999999999;
              }
              alpha[i][n] = 2.0 * atanh(beta_prod);
            }
          }
        }
    
        for (int i = 0; i < B * (Q + 1); i++) {
          double alpha_sum = 0.0;
          double alpha_tmp[Q];
    
          for (int k = 0; k < vd_[i]; k++) { 
            int focus = vn_[i][k];
            int tmp = vn_back_[i][k];
            alpha_sum += alpha[focus][tmp];
            alpha_tmp[k] = alpha[focus][tmp];
          }

          alpha_sum = llr[i] + alpha_sum;
          if (alpha_sum > 0.0) { 
            tmp_ans[i] = 0;
          } else{ 
            tmp_ans[i] = 1;
          }
  
          for (int k = 0; k < vd_[i]; k++){
            beta[i][k] = alpha_sum - alpha_tmp[k];  
          }
        }

        if (iteration > 0) {
          bool parity_check_ok = true;
          for (int i = 0; i < B * Q; i++) {
            int check_bit = 0;
            for (int j = 0; j < cd_[i]; j++) {
              check_bit = check_bit ^ tmp_ans[cn_[i][j]];
            }
            if (check_bit != 0) {
              parity_check_ok = false; 
              break;
            }
          }
          if (parity_check_ok) {
            break;
          }
        }
      }
      std::bitset<B> ans;
      for (int i = 0; i < B; i++) {
        ans[i] = tmp_ans[B * Q + i];
      }
      return ans;
    }

  private:
    int vn_[B * (Q + 1)][Q], vn_back_[B * (Q + 1)][Q];
    int cn_[B * Q][3], cn_back_[B * Q][3];
    int vd_[B * (Q + 1)], cd_[B * Q];
};

// Simple repeat encoder (sends original Q + 1 times)
std::bitset<B * (Q + 1)> repeat_encoder(const std::bitset<B> input)
{
  std::bitset<B * (Q + 1)> output;
  for (int i = 0; i < B; i++) {
    for (int j = 0; j <= Q; j++) {
      output[i + B * j] = input[i];
    }
  }
  return output;
}

std::bitset<B> simple_average_decoder(const double received_repeat[B * (Q + 1)])
{
  std::bitset<B> corrected_repeat;
  for (int i = 0; i < B; i++) {
    double sum = 0;
    for (int j = 0; j <= Q; j++) {
      sum += received_repeat[i + B * j];
    }
    if (sum > 0) {
      corrected_repeat[i] = 1;
    } else {
      corrected_repeat[i] = 0;
    }
  }
  return corrected_repeat;
}

int main(void)
{
  const double rate = 1. / (Q + 1);
  const double ebn0 = 2.; // Signal to noise ratio in dB
  const double variance = 1.0 / (rate * pow(10.0, ebn0 / 10.0)) * 0.5;
  printf("Coding rate = %lf, SNR = %g dB\n", rate, ebn0);
    
  auto original = get_original_bits();
  write_pbm("original.pbm", original);

  // Additive white gaussian noise
  double noise[B * (Q + 1)];
  std::default_random_engine gen(42);
  std::normal_distribution<double> dist{0, sqrt(variance)};
  for (int i = 0; i < B * (Q + 1); i++) {
    noise[i] = dist(gen);
  }

  auto repeat_code = repeat_encoder(original);

  RepeatAccumulateEncoder ra_encoder;
  auto ra_code = ra_encoder.encode(original);

  // Simulate transmission over noisy channel
  // Encode each 0 bit as -1, 1 bit as +1, add noise
  double received_repeat[B * (Q + 1)];
  for (int i = 0; i < B * (Q + 1); i++) {
    received_repeat[i] = 2. * repeat_code[i] - 1 + noise[i];
  }
  write_pgm("received_repeat.pgm", received_repeat);

  double received_ra[B * (Q + 1)];
  for (int i = 0; i < B * (Q + 1); i++) {
	  received_ra[i] = 2. * ra_code[i] - 1 + noise[i];
  }
  write_pgm("received_ra.pgm", received_ra);

  auto corrected_repeat = simple_average_decoder(received_repeat);
  write_pbm("corrected_repeat.pbm", corrected_repeat);

  double llr[B * (Q + 1)];
  for (int i = 0; i < B * (Q + 1); i++) {
	  llr[i] = -2.0 * received_ra[i] / variance;
  }

  RepeatAccumulateDecoder ra_decoder(ra_encoder.order());
      
  // Try different number of iterations to illustrate progress
  auto corrected_ra = ra_decoder.decode(llr, 1); write_pbm("corrected_ra_01.pbm", corrected_ra);
  corrected_ra = ra_decoder.decode(llr, 5); write_pbm("corrected_ra_05.pbm", corrected_ra);
  corrected_ra = ra_decoder.decode(llr, 10); write_pbm("corrected_ra_10.pbm", corrected_ra);
  corrected_ra = ra_decoder.decode(llr, 15); write_pbm("corrected_ra_15.pbm", corrected_ra);
  corrected_ra = ra_decoder.decode(llr, 20); write_pbm("corrected_ra_20.pbm", corrected_ra);
  corrected_ra = ra_decoder.decode(llr, 25); write_pbm("corrected_ra_25.pbm", corrected_ra);

  // Count errors
  int error_count_repeat = 0, error_count_ra = 0;
  for (int i = 0; i < B; i++) {
	  if (original[i] != corrected_repeat[i]) {
      error_count_repeat++;
    }
    if (original[i] != corrected_ra[i]) {
      error_count_ra++;
    }
  }
  printf("%d simple repeat errors, %d RA errors\n", error_count_repeat, error_count_ra);
  return 0;
}
