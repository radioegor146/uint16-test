#include <cstdbool>
#include <cstdint>
#include <cstdio>
#include <cstring>

#if NMH_VECTOR > 0
#define NMH_ACC_ALIGN 64
#elif defined(__BIGGEST_ALIGNMENT__)
#define NMH_ACC_ALIGN __BIGGEST_ALIGNMENT__
#elif defined(__SDCC)
#define NMH_ACC_ALIGN 1
#else
#define NMH_ACC_ALIGN 16
#endif

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L) /* C11+ */
#include <cstdalign>
#define NMH_ALIGN(n) alignas(n)
#elif defined(__GNUC__)
#define NMH_ALIGN(n) __attribute__((aligned(n)))
#elif defined(_MSC_VER)
#define NMH_ALIGN(n) __declspec(align(n))
#else
#define NMH_ALIGN(n) /* disabled */
#endif

NMH_ALIGN(NMH_ACC_ALIGN)
static const uint32_t NMH_ACC_INIT[32] = {
    UINT32_C(0xB8FE6C39), UINT32_C(0x23A44BBE), UINT32_C(0x7C01812C),
    UINT32_C(0xF721AD1C), UINT32_C(0xDED46DE9), UINT32_C(0x839097DB),
    UINT32_C(0x7240A4A4), UINT32_C(0xB7B3671F), UINT32_C(0xCB79E64E),
    UINT32_C(0xCCC0E578), UINT32_C(0x825AD07D), UINT32_C(0xCCFF7221),
    UINT32_C(0xB8084674), UINT32_C(0xF743248E), UINT32_C(0xE03590E6),
    UINT32_C(0x813A264C), UINT32_C(0x3C2852BB), UINT32_C(0x91C300CB),
    UINT32_C(0x88D0658B), UINT32_C(0x1B532EA3), UINT32_C(0x71644897),
    UINT32_C(0xA20DF94E), UINT32_C(0x3819EF46), UINT32_C(0xA9DEACD8),
    UINT32_C(0xA8FA763F), UINT32_C(0xE39C343F), UINT32_C(0xF9DCBBC7),
    UINT32_C(0xC70B4F1D), UINT32_C(0x8A51E04B), UINT32_C(0xCDB45931),
    UINT32_C(0xC89F7EC9), UINT32_C(0xD9787364),
};

#define __NMH_M1 UINT32_C(0xF0D9649B)
#define __NMH_M2 UINT32_C(0x29A7935D)
#define __NMH_M3 UINT32_C(0x55D35831)

NMH_ALIGN(NMH_ACC_ALIGN)
static const uint32_t __NMH_M1_V[32] = {
    __NMH_M1, __NMH_M1, __NMH_M1, __NMH_M1, __NMH_M1, __NMH_M1, __NMH_M1,
    __NMH_M1, __NMH_M1, __NMH_M1, __NMH_M1, __NMH_M1, __NMH_M1, __NMH_M1,
    __NMH_M1, __NMH_M1, __NMH_M1, __NMH_M1, __NMH_M1, __NMH_M1, __NMH_M1,
    __NMH_M1, __NMH_M1, __NMH_M1, __NMH_M1, __NMH_M1, __NMH_M1, __NMH_M1,
    __NMH_M1, __NMH_M1, __NMH_M1, __NMH_M1,
};

bool NMHash32Broken() {
  const char entropy[] =
      "rwgk8M1uxM6XX6c3teQX2yaw8FQWArmcWUSBJ8dcQQJWHYC9Wt2BmpvETxwhYcJTheTbjf49"
      "SVRaDJhbEZCq7ki1D6KxpKQSjgwqsiHGSgHLxvPG5kcRnBhjJ1YC8kuh";

  NMH_ALIGN(NMH_ACC_ALIGN)
  uint32_t accX[sizeof(NMH_ACC_INIT) / sizeof(*NMH_ACC_INIT)];
  static_assert(sizeof(entropy) >= sizeof(accX),
                "Need more entropy in entropy[]");
  memcpy(accX, entropy, sizeof(accX));

  const size_t nbGroups = sizeof(NMH_ACC_INIT) / sizeof(*NMH_ACC_INIT);
  size_t i;

  for (i = 0; i < nbGroups * 2; ++i) {
    ((uint16_t*)accX)[i] *= ((uint16_t*)__NMH_M1_V)[i];
  }

  // XXX: no memory barrier takes place here, just like in NMHASH32_long_round_scalar()
  // and it affects the `acc` result.

  uint32_t acc = 0;
  for (i = 0; i < nbGroups; ++i)
    acc += accX[i];

  return (acc != UINT32_C(0x249abaee));
}

int main() {
  printf("Test result: %d\n", NMHash32Broken());
  return 0;
}