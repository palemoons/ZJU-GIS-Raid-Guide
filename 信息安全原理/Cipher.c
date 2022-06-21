#include <math.h>
#include <stdio.h>

long long modpow(long long a, long long b, long long m) {
  a %= m;
  long long res = 1;
  while (b > 0) {
    if (b & 1) res = res * a % m;
    a = a * a % m;
    b >>= 1;
  }
  return res;
}

int main() {
  int p = 0, g = 0, pA = 0, pB = 0;
  // input
  printf("p: ");
  scanf("%d", &p);
  printf("g: ");
  scanf("%d", &g);
  printf("A's private key: ");
  scanf("%d", &pA);
  printf("B's private key: ");
  scanf("%d", &pB);

  long long kuA = modpow(g, pA, p);
  long long kuB = modpow(g, pB, p);
  long long s = modpow(kuB, pA, p);
  printf("%lld\n%lld\n%lld\n", kuA, kuB, s);
  return 0;
}