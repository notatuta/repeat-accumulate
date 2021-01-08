//
// Repeat-accumulate error correction code example
//
#include <algorithm>
#include <bitset>
#include <cmath>
#include <random>
#include <stdexcept>
#include <stdio.h>

// B * Q should be a multiple of A
const int Q = 3; // number of bit repetitions
const int B = 64 * 64; // number of information bits
const int A = 1; // number of accumulated bits

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

void write_pgm(const char* filename, const double pieces[64 * 64 * (Q / A + 1)])
{
  FILE* f = fopen(filename, "w");
  if (f) {
    fprintf(f, "P2\n%d 64\n255\n", 64 * (Q / A + 1));
    for (int row = 0; row < 64; row++) {
      for (int col = 0; col < 64 * (Q / A + 1); col++) {
        double intensity = (2. - pieces[col * 64 + row] / 2) * 256 / 4;
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
      unsigned seed = 123;
      std::mt19937_64 gen;
      gen.seed(seed);

      // Start with ordered sequence
      for (int i = 0; i < B * Q; i++) {
        order_[i] = i;
      }

      // Fisher-Yates shuffle with one extra condition: swap only if resulting interleaver is S-random 
      // (no input symbols within distance S appear within a distance of S in the output) with S == Q
      for (int i = 0; i < B * Q - 1; i++) {
        std::uniform_int_distribution<int> dist(i, B * Q - 1);
        auto j = dist(gen);
        bool is_s_random = true;
        for (int back = 0; back < Q && i >= back; back++) {
          int distance = order_[i - back] - order_[j];
          if (distance < Q && distance > -Q) {
            is_s_random = false;
            break;
          }
        }
        if (is_s_random) {
          auto tmp = order_[i];
          order_[i] = order_[j];
          order_[j] = tmp;
        } else {
          if (i) { // Step back and try again
            i--; 
          } else {
            throw std::logic_error("Could not make interleaver S-random");
          }
        }
      }
    }

    // Decoder will need to know the order
    const int (&order() const)[B * Q] { return order_; }

    std::bitset<B + B * Q / A> encode(const std::bitset<B> input) const
    {
      std::bitset<B + B * Q / A> output;
 
      // Systematic part (data bits) first
      for (int i = 0; i < B; i++) { 
        output[i] = input[i];
      }

      // Followed by parity bits
      bool d = false;
      for (int i = 0, j = B; i < B * Q; i++) {
        d ^= input[order_[i] / Q];
        if (i % A == A - 1) {
          output[j] = d;
          j++;
        }
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
      // Data bits
      for (int i = 0; i < B * Q / A; i++) { // check node, and also parity bit
        for (int a = 0; a < A; a++) { // accumulated bit
          int k = i ? a + 2 : a + 1; // check node input
          int j = order[i * A + a] / Q; // variable node connected to this check node (from data bit index)
          int m = order[i * A + a] % Q; // variable node input (from bit repeat number)
          cn_[i][k] = j;
          vn_input_[i][k] = m;
          vn_[j][m] = i;
          cn_input_[j][m] = k;
        }
      }
      // Parity bits
      for (int j = B; j < B + B * Q / A; j++) { // variable node
        for (int m = 0; m < variable_node_degree(j); m++) { // variable node input
          int i = j - B + m; // check node connected to this variable node
          int k = j - B ? 1 - m : 0;// check node input
          cn_[i][k] = j;
          vn_input_[i][k] = m;
          vn_[j][m] = i;
          cn_input_[j][m] = k;
        }
      }
    }

    int variable_node_degree(int node) const {
      if (node < 0) {
        throw std::invalid_argument("Negative variable node index");
      } else if (node < B) {
        return Q;
      } else if (node < B + B * Q / A - 1) {
        return 2;
      } else if (node == B + B * Q / A - 1) {
        return 1;
      } else {
        throw std::invalid_argument("Variable node index too high");
      }
    }

    int check_node_degree(int node) const {
      if (node < 0) {
        throw std::invalid_argument("Negative check node index");
      } else if (node == 0) {
        return A + 1;
      } else if (node < B * Q / A) {
        return A + 2;
      } else {
        throw std::invalid_argument("Check node index too high");
      }
    }

    std::bitset<B> decode(const double llr[B + B * Q / A], int itemax = 1000) const
    {
      double beta[B + B * Q / A][Q] = {}; // Messages from check nodes to bit nodes, initially all zeroes
      double alpha[B * Q / A][A + 2] = {}; // Messages from bit nodes to check nodes, initially all zeroes
      int ans[B + B * Q / A]; // Log likelihoods of transmitted bits
      for (int iteration = 0; iteration < itemax; iteration++) {

        // Send messages from check nodes to bit nodes
        for (int j = 0; j < B + B * Q / A; j++) {
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
        for (int i = 0; i < B * Q / A; i++) {
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
          printf("All parity checks passed at iteration %d\n", iteration);
          break;
        }

        // Send messages from bit nodes to check nodes
        if (iteration > 0) {
          for (int i = 0; i < B * Q / A; i++) {
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
        output[i] = ans[i];
      }
      return output;
    }

  private:
    int vn_[B + B * Q / A][Q]; // Which check node this variable node is connected to
    int cn_input_[B + B * Q / A][Q]; // Which input of the check node this connection goes to
    int cn_[B * Q / A][A + 2]; // Which variable node this check node is connected to
    int vn_input_[B * Q / A][A + 2]; // Which input of the variable node this connection goes to
};

// Simple repeat encoder
std::bitset<B * (1 + Q / A)> repeat_encode(const std::bitset<B> input)
{
  std::bitset<B * (1 + Q / A)> output;
  for (int i = 0; i < B; i++) {
    for (int j = 0; j < 1 + Q / A; j++) {
      output[i + B * j] = input[i];
    }
  }
  return output;
}

// MLE repeat decoder: sum log likelihoods, compare with zero
std::bitset<B> repeat_decode(const double received_repeat[B * (1 + Q / A)])
{
  std::bitset<B> corrected_repeat;
  for (int i = 0; i < B; i++) {
    double sum = 0;
    for (int j = 0; j < 1 + Q / A; j++) {
      sum += received_repeat[i + B * j];
    }
    if (sum > 0) {
      corrected_repeat[i] = 0;
    } else {
      corrected_repeat[i] = 1;
    }
  }
  return corrected_repeat;
}

int main(void)
{
  const double rate = 1. / ((double)Q / A + 1);
  const double ebn0 = 1.; // Signal to noise ratio in dB
  const double variance = 1.0 / (rate * pow(10.0, ebn0 / 10.0)) * 0.5;
  printf("Coding rate = %g, SNR = %g dB\n", rate, ebn0);
    
  auto original = get_original_bits();
  write_pbm("images/original.pbm", original);

  // Additive white gaussian noise
  double noise[B + B * Q / A];
  std::default_random_engine gen(42);
  std::normal_distribution<double> dist{0, sqrt(variance)};
  for (int i = 0; i < B + B * Q / A; i++) {
    noise[i] = dist(gen);
  }

  // Encode the same original using both methods
  auto repeat_code = repeat_encode(original);
  RepeatAccumulateEncoder ra_encoder;
  auto ra_code = ra_encoder.encode(original);

  // Simulate transmission over noisy channel
  // Encode each 0 bit as -1, 1 bit as +1, add noise
  double received_repeat[B + B * Q / A];
  double received_ra[B + B * Q / A];
  for (int i = 0; i < B + B * Q / A; i++) {
    received_repeat[i] = 2. * repeat_code[i] - 1 + noise[i];
    received_ra[i] = 2. * ra_code[i] - 1 + noise[i];
  }
  write_pgm("images/received_repeat.pgm", received_repeat);
  write_pgm("images/received_ra.pgm", received_ra);

  // Log likelihood ratio for received bits 
  // Divide PDFs of normal distribution with means +1 and -1 and given variance, then take the log
  double llr_repeat[B + B * Q / A];
  double llr_ra[B + B * Q / A];
  for (int i = 0; i < B + B * Q / A; i++) {
    llr_repeat[i] = -2.0 * received_repeat[i] / variance;
    llr_ra[i] = -2.0 * received_ra[i] / variance;
  }

  // Now decode what we received
  auto corrected_repeat = repeat_decode(llr_repeat);
  write_pbm("images/corrected_repeat.pbm", corrected_repeat);

  // Try different number of iterations to illustrate progress
  RepeatAccumulateDecoder ra_decoder(ra_encoder.order());
  auto 
  corrected_ra = ra_decoder.decode(llr_ra, 1);  write_pbm("images/corrected_ra_01.pbm", corrected_ra);
  corrected_ra = ra_decoder.decode(llr_ra, 10); write_pbm("images/corrected_ra_10.pbm", corrected_ra);
  corrected_ra = ra_decoder.decode(llr_ra, 20); write_pbm("images/corrected_ra_20.pbm", corrected_ra);
  corrected_ra = ra_decoder.decode(llr_ra, 30); write_pbm("images/corrected_ra_30.pbm", corrected_ra);
  corrected_ra = ra_decoder.decode(llr_ra, 40); write_pbm("images/corrected_ra_40.pbm", corrected_ra);
  corrected_ra = ra_decoder.decode(llr_ra);     write_pbm("images/corrected_ra.pbm",    corrected_ra);

  // Count errors
  int error_count_repeat = (original ^ corrected_repeat).count();
  int error_count_ra = (original ^ corrected_ra).count();
  printf("%d simple repeat errors, %d RA errors\n", error_count_repeat, error_count_ra);
  return 0;
}
