#include <math.h>
#include <stdio.h>
#include <string.h>

int main() {
  char input[512] = {0};
  char output[512] = {0};
  char d[512][512] = {0};
  char secret[512] = {0};
  // char s[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i',
  //               'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
  //               's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
  double freq[26] = {0.08167, 0.01492, 0.02782, 0.04253, 0.12705, 0.02228,
                     0.02015, 0.06094, 0.06996, 0.00153, 0.00772, 0.04025,
                     0.02406, 0.06749, 0.07507, 0.01929, 0.0009,  0.05987,
                     0.06327, 0.09056, 0.02758, 0.00978, 0.02360, 0.0015,
                     0.01974, 0.00074};
  int l = 1;  // the number of substrings

  scanf("%s", input);
  while (1) {
    double coincidence[512] = {0};
    int n, m;
    double index = 0;
    for (int i = 0; i < 512; i++) {
      for (int j = 0; j < 512; j++) {
        d[i][j] = 0;
      }
    }
    // guess the length of the secret key
    for (int i = 0; i < strlen(input); i++) {  // get substring
      n = i % l;
      m = i / l;
      d[n][m] = input[i];
    }
    for (int i = 0; i < l; i++) {
      int alphabet[26] = {0};
      int len = strlen(d[i]);
      for (int j = 0; j < len; j++) {
        alphabet[d[i][j] - 'a']++;
      }
      for (int j = 0; j < 26; j++) {
        coincidence[i] += alphabet[j] * (alphabet[j] - 1);
      }
      coincidence[i] /= len * (len - 1);
      printf("%lf\t", coincidence[i]);
      index += coincidence[i];
    }
    index = index / l;
    printf("%lf\n", index);
    if (index > 0.06 && index < 0.07) {
      break;
    }
    l++;
  }
  printf("The length of the secret key: %d\n", l);
  // decrypt the secret key
  for (int i = 0; i < l; i++) {
    int len = (int)strlen(d[i]);// length of the substring
    int offset = 0;
    double maxchi = 0;  // max chi
    int alphabet[26] = {0};
    for (int j = 0; j < len; j++) {
      alphabet[d[i][j] - 'a']++;
    }
    for (int j = 0; j < 26; j++) {  //j: offset
      double chi = 0;
      for (int k = 0; k < 26; k++) {
        chi += (alphabet[(k + j) % 26] / (double)len) * freq[k];
      }
      if (chi > maxchi) {
        maxchi = chi;
        offset = j;
      }
    }
    secret[i] = offset + 'a';
    printf("\n");
  }
  for (int i = 0; i < (int)strlen(input); i++) {
    output[i] = (d[i % l][i / l] - secret[i % l] + 26) % 26 + 'a';
  }
  printf("The secret key: %s\n", secret);
  printf("The output: %s\n", output);
  return 0;
}
//it is essential to seek out enemy agents 
//who have come to conduct espionage against you and to bribe them to
//serve you give them instruction sand care for them
//thus doubled agents are recruite dandused suntzu the art of war