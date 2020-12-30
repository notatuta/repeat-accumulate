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

// Transmit by column rather than by row to use less screen real estate
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
  for (int col = 0; col < 64; col++) {
    for (int row = 0; row < 64; row++) {
      bits[col * 64 + row] = cat_pbm[9 + row * 65 + col] == '0' ? 0 : 1;
    }
  }
  return bits;
}

void write_pbm(const char* filename, const std::bitset<64 * 64> bits)
{
  FILE* f = fopen(filename, "w");
  if (f) {
    fprintf(f, "P1\n64 64\n");
    for (int row = 0; row < 64; row++) {
      for (int col = 0; col < 64; col++) {
        fprintf(f, "%c", bits[col * 64 + row] ? '1' : '0');
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
    fprintf(f, "P2\n%d 64\n255\n", 64 * (Q + 1));
    for (int row = 0; row < 64; row++) {
      for (int col = 0; col < 64 * (Q + 1); col++) {
        double intensity = (2. - pieces[col * 64 + row]) * 256 / 4;
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

    // Decoder will need to know the order
    const int (&order() const)[B * Q] { return order_; }

    // Sends original information bits at the end
    std::bitset<B * (Q + 1)> encode(const std::bitset<B> input) const
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
      int reverse_order[B * Q];
      for (int o = 0; o < B * Q; o++) {
        reverse_order[order[o]] = o;
      }
      int data_bit_index[B * Q];
      int repeat_number[B * Q];
      for (int data_bit = 0, o = 0; data_bit < B; data_bit++) {
        for (int repeat = 0; repeat < Q; repeat++) {
          data_bit_index[reverse_order[o]] = data_bit;
          repeat_number[reverse_order[o]] = repeat;
          o++;
        }
      }
      for (int i = 0; i < B * Q; i++) {
        int source_bit = data_bit_index[i] + B * Q;
        if (i == 0) {
          cn_[0][0] = 0;
          cn_[0][1] = source_bit;
          cn_input_[0][0] = 0;
          cn_input_[source_bit][0] = 1;
          vn_input_[0][0] = 0;
          vn_input_[0][1] = repeat_number[i];
        } else {
          cn_[i][0] = i - 1; 
          cn_[i][1] = i;
          cn_[i][2] = source_bit;          
          cn_input_[i][0] = 1;
          vn_input_[i][0] = 1;
          vn_input_[i][1] = 0;
          vn_input_[i][2] = repeat_number[i];
        }
        if (i != B * Q - 1) {
          cn_input_[i][1] = 0;
        }
        vn_[i][0] = i;
        if (i != B * Q - 1) { // last bit variable node has only one input
          vn_[i][1] = i + 1;
        }
        vn_[source_bit][repeat_number[i]] = i;
        cn_input_[source_bit][repeat_number[i]] = i ? 2 : 1;
      }
    }

    int variable_node_degree(int node) const {
      if (node < B * Q - 1) {
        return 2;
      } else if (node == B * Q - 1) {
        return 1;
      } else {
        return Q;
      }
    }

    int check_node_degree(int node) const {
      return node ? 3 : 2;
    }

    std::bitset<B> decode(const double llr[B * (Q + 1)], int itemax = 100) const
    {
      double beta[B * Q + B][Q] = {}; // Message from check nodes to bit nodes, initially all zeroes
      double alpha[B * Q][3] = {};
      int ans[B * (Q + 1)];
      for (int iteration = 0; iteration < itemax; iteration++) {

        // Send messages from check nodes to bit nodes
        for (int j = 0; j < B * (Q + 1); j++) {
          double alpha_sum = 0.0;
          double alpha_tmp[Q];
          for (int m = 0; m < variable_node_degree(j); m++) { 
            int i = vn_[j][m];
            int k = cn_input_[j][m];
            alpha_sum += alpha[i][k];
            alpha_tmp[m] = alpha[i][k];
          }
          alpha_sum = llr[j] + alpha_sum;
          ans[j] = (alpha_sum > 0.0) ? 0 : 1;
          for (int m = 0; m < variable_node_degree(j); m++){
            beta[j][m] = alpha_sum - alpha_tmp[m];  
          }
        }

        // Stop iterating if all parity checks pass 
        bool all_parity_checks_ok = true;
        for (int i = 0; i < B * Q; i++) {
          int check_bit = 0;
          for (int k = 0; k < check_node_degree(i); k++) {
            check_bit = check_bit ^ ans[cn_[i][k]];
          }
          if (check_bit != 0) {
            all_parity_checks_ok = false; 
            break;
          }
        }
        if (all_parity_checks_ok) {
          break;
        }

        // Send messages from bit nodes to check nodes
        if (iteration > 0) {
          for (int i = 0; i < B * Q; i++) {
            for (int n = 0; n < check_node_degree(i); n++) { // will update alpha for this check node
              double beta_prod = 1.0;
              for (int k = 0; k < check_node_degree(i); k++) { // loop over variable nodes connected to this one
                if (k != n) { // except itself
                  int j = cn_[i][k];
                  int m = vn_input_[i][k];
                  beta_prod *= tanh(0.5 * beta[j][m]);
                }
              }
              if (beta_prod > 1 - 1e-15) {
                beta_prod = 1 - 1e-15;
              } else if (beta_prod < -1 + 1e-15) {
                beta_prod = -1 + 1e-15;
              }
              alpha[i][n] = 2 * atanh(beta_prod);
            }
          }
        }
    
      }
      std::bitset<B> output;
      for (int i = 0; i < B; i++) {
        output[i] = ans[B * Q + i];
      }
      return output;
    }

  private:
    int vn_[B * (Q + 1)][Q]; // Which check node this variable node is connected to
    int cn_input_[B * (Q + 1)][Q]; // Which input of the check node this connection goes to
    int cn_[B * Q][3]; // Which variable node this check node is connected to
    int vn_input_[B * Q][3]; // Which input of the variable node this connection goes to
};

// Simple repeat encoder (sends original Q + 1 times)
std::bitset<B * (Q + 1)> repeat_encode(const std::bitset<B> input)
{
  std::bitset<B * (Q + 1)> output;
  for (int i = 0; i < B; i++) {
    for (int j = 0; j <= Q; j++) {
      output[i + B * j] = input[i];
    }
  }
  return output;
}

std::bitset<B> simple_average_decode(const double received_repeat[B * (Q + 1)])
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
  write_pbm("images/original.pbm", original);

  // Additive white gaussian noise
  double noise[B * (Q + 1)];
  std::default_random_engine gen(42);
  std::normal_distribution<double> dist{0, sqrt(variance)};
  for (int i = 0; i < B * (Q + 1); i++) {
    noise[i] = dist(gen);
  }

  auto repeat_code = repeat_encode(original);

  RepeatAccumulateEncoder ra_encoder;
  auto ra_code = ra_encoder.encode(original);

  // Simulate transmission over noisy channel
  // Encode each 0 bit as -1, 1 bit as +1, add noise
  double received_repeat[B * (Q + 1)];
  for (int i = 0; i < B * (Q + 1); i++) {
    received_repeat[i] = 2. * repeat_code[i] - 1 + noise[i];
  }
  write_pgm("images/received_repeat.pgm", received_repeat);

  double received_ra[B * (Q + 1)];
  for (int i = 0; i < B * (Q + 1); i++) {
	  received_ra[i] = 2. * ra_code[i] - 1 + noise[i];
  }
  write_pgm("images/received_ra.pgm", received_ra);

  auto corrected_repeat = simple_average_decode(received_repeat);
  write_pbm("images/corrected_repeat.pbm", corrected_repeat);

  double llr[B * (Q + 1)];
  for (int i = 0; i < B * (Q + 1); i++) {
	  llr[i] = -2.0 * received_ra[i] / variance;
  }

  RepeatAccumulateDecoder ra_decoder(ra_encoder.order());
      
  // Try different number of iterations to illustrate progress
  auto corrected_ra = ra_decoder.decode(llr, 1); write_pbm("images/corrected_ra_01.pbm", corrected_ra);
  corrected_ra = ra_decoder.decode(llr, 5); write_pbm("images/corrected_ra_05.pbm", corrected_ra);
  corrected_ra = ra_decoder.decode(llr, 10); write_pbm("images/corrected_ra_10.pbm", corrected_ra);
  corrected_ra = ra_decoder.decode(llr, 15); write_pbm("images/corrected_ra_15.pbm", corrected_ra);
  corrected_ra = ra_decoder.decode(llr, 20); write_pbm("images/corrected_ra_20.pbm", corrected_ra);
  corrected_ra = ra_decoder.decode(llr, 25); write_pbm("images/corrected_ra_25.pbm", corrected_ra);

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
