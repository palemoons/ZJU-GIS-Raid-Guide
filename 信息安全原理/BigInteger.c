#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int BASE = 1000000000;
int DIGIT = 9;

int len(int* arr) {
  for (int i = 999; i >= 0; i--) {
    if (arr[i] != 0) return i + 1;
  }
  return 0;
}

int llen(long* arr) {
  for (int i = 999; i >= 0; i--) {
    if (arr[i] != 0) return i + 1;
  }
  return 0;
}

void print(int* arr) {
  if (len(arr) == 0) {
    printf("0");
  } else {
    printf("%d", arr[len(arr) - 1]);
    for (int i = len(arr) - 2; i >= 0; i--) {
      printf("%09d", abs(arr[i]));
    }
  }
  printf("\n");
}

int isNoSmallerThan(int* num1, int* num2) {
  int len1 = len(num1), len2 = len(num2);
  if (len1 == len2) {
    for (int i = len1 - 1; i >= 0; i--) {
      if (num1[i] < num2[i]) {
        return 0;
      }
      if (num1[i] > num2[i]) return 1;
    }
  } else if (len1 < len2) {
    return 0;
  }
  return 1;
}

void add(int* num1, int* num2, int* result) {
  for (int i = 0; i < 1000; i++) result[i] = 0;
  int len1 = len(num1), len2 = len(num2);
  int maxlen = len1 > len2 ? len1 : len2;
  for (int i = 0, flag = 0; i < maxlen + 1; i++) {
    int tmp = num1[i] + num2[i] + (flag ? 1 : 0);
    if (tmp >= BASE) {
      flag = 1;
      tmp -= BASE;
    } else
      flag = 0;
    result[i] = tmp;
  }
}

void minus(int* num1, int* num2, int f, int* result) {
  int len1 = len(num1);
  for (int i = 0; i < 1000; i++) result[i] = 0;
  if (isNoSmallerThan(num1, num2) && isNoSmallerThan(num2, num1)) return;

  if (isNoSmallerThan(num2, num1)) {
    minus(num2, num1, -1, result);
    return;
  }
  for (int i = 0, flag = 0; i < len1; i++) {
    int tmp = num1[i] - flag - num2[i];
    if (tmp < 0) {
      flag = 1;
      tmp += BASE;
    } else
      flag = 0;
    result[i] = tmp * f;
  }
}

void multi(int* num1, int* num2, int* result) {
  long temp[1000];
  long t1[1000], t2[1000];
  for (int i = 0; i < 1000; i++) {
    temp[i] = 0;
    t1[i] = num1[i];
    t2[i] = num2[i];
  }
  for (int i = 0; i < len(num1); i++) {
    for (int j = 0; j < len(num2); j++) {
      temp[i + j] += t1[i] * t2[j];
    }
  }
  for (int i = 0; i < llen(temp); i++) {
    if (temp[i] >= BASE) {
      temp[i + 1] += temp[i] / BASE;
      temp[i] = temp[i] % BASE;
    }
  }
  for (int i = 0; i < llen(temp); i++) {
    result[i] = (int)temp[i];
  }
}

void divide(int* num1, int* num2, int* result, int* mod) {  // num1 / num2
  int len1 = len(num1), len2 = len(num2);
  if (len1 == 0) return;
  if (isNoSmallerThan(num2, num1)) return;

  int res[1000];
  for (int i = 0; i < 1000; i++) res[i] = 0;

  int tmp[1000], r1[1000], r2[1000];
  for (int i = 0; i < 1000; i++) {
    r1[i] = num1[i];
    r2[i] = num2[i];
  }
  while (1) {
    for (int i = 0; i < 1000; i++) {
      tmp[i] = num2[i];
      r2[i] = 0;
    }
    int l1 = len(r1), l2 = len(tmp);

    int delta = 0;
    if (r1[l1 - 1] >= tmp[l2 - 1])
      delta = 1;
    else
      delta = 2;

    for (int i = l1 - delta; l1 > l2 && i > l1 - l2 - delta; i--) {
      tmp[i] = tmp[i - (l1 - l2 - delta + 1)];
    }
    for (int i = l1 - l2 - delta; i >= 0; i--) {
      tmp[i] = 0;
    }

    minus(r1, tmp, 1, r2);
    result[l1 - l2 + 1 - delta]++;
    for (int i = 0; i < 1000; i++) r1[i] = r2[i];

    if (!isNoSmallerThan(r2, num2)) break;
  }
  for (int i = 0; i < len(r1); i++) mod[i] = r1[i];
  return;
}

int main() {
  int num1[1000], num2[1000];
  char n1[1024] = {0}, n2[1024] = {0};
  for (int i = 0; i < 1000; i++) num1[i] = num2[i] = 0;
  scanf("%s", n1);
  scanf("%s", n2);
  for (int i = strlen(n1) - 1, d = 0; i >= 0; i--, d++) {  // 小端
    num1[d / 9] += pow(10, d % 9) * (n1[i] - '0');
  }
  for (int i = strlen(n2) - 1, d = 0; i >= 0; i--, d++) {  // 小端
    num2[d / 9] += pow(10, d % 9) * (n2[i] - '0');
  }

  int r1[1000], r2[1000], r3[1000], r4[1000], r5[1000];
  for (int i = 0; i < 1000; i++) r1[i] = r2[i] = r3[i] = r4[i] = r5[i] = 0;
  add(num1, num2, r1);
  minus(num1, num2, 1, r2);
  multi(num1, num2, r3);
  divide(num1, num2, r4, r5);
  printf("%s + %s = ", n1, n2);
  print(r1);// result of plus
  printf("%s - %s = ", n1, n2);
  print(r2);// result of minus
  printf("%s * %s = ", n1, n2);
  print(r3);// result of multiplex
  printf("%s / %s = ", n1, n2);
  print(r4);// result of division
  printf("%s mod %s = ", n1, n2);
  print(r5);// result of mod
  return 0;
}