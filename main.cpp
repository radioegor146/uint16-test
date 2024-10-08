#include <cstdio>

#include <cstdint>
#include <cstring>

#define __NMH_M1 UINT32_C(0xF0D9649B)

static const uint32_t __NMH_M1_V[32] = {
    __NMH_M1, __NMH_M1, __NMH_M1, __NMH_M1, __NMH_M1, __NMH_M1, __NMH_M1,
    __NMH_M1, __NMH_M1, __NMH_M1, __NMH_M1, __NMH_M1, __NMH_M1, __NMH_M1,
    __NMH_M1, __NMH_M1, __NMH_M1, __NMH_M1, __NMH_M1, __NMH_M1, __NMH_M1,
    __NMH_M1, __NMH_M1, __NMH_M1, __NMH_M1, __NMH_M1, __NMH_M1, __NMH_M1,
    __NMH_M1, __NMH_M1, __NMH_M1, __NMH_M1,
};

uint32_t nmhash32_broken(volatile char* entropy) {

  uint32_t accX[32];
  memcpy(accX, (void*)entropy, sizeof(accX));

  size_t i;

  for (i = 0; i < 64; ++i) {
    ((uint16_t*)accX)[i] *= ((uint16_t*)__NMH_M1_V)[i];
  }

  uint32_t acc = 0;
  for (i = 0; i < 32; ++i)
    acc += accX[i];

  return (acc);
}

const char entropy[] =
    "rwgk8M1uxM6XX6c3teQX2yaw8FQWArmcWUSBJ8dcQQJWHYC9Wt2BmpvETxwhYcJTheTbjf49"
    "SVRaDJhbEZCq7ki1D6KxpKQSjgwqsiHGSgHLxvPG5kcRnBhjJ1YC8kuh";

int main(int argc, const char** argv) {
  printf("%d\n", nmhash32_broken((volatile char*)entropy) != UINT32_C(0x249abaee));
  return 0;
}
