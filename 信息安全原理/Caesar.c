#include <math.h>
#include <stdio.h>
#include <string.h>

int main() {
  char input[128] = {0};
  scanf("%s", input);
  for (int i = 1; i < 26; i++) {
    for (int j = 0; input[j] != 0; j++) {
      input[j] = (input[j] + 1 - 'A') % 26 + 'A';
    }
    printf("key: +%d\t", i);
    for (int j = 0; j < strlen(input); j++) {
      printf("%c", input[j] - ('A' - 'a'));
    }
    printf("\n");
  }
  return 0;
}
// please encrypt your name with the same key and upload to learning in zju