#include <klib.h>
#include <klib-macros.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s)
{
  size_t len = 0;
  while (s[len] != '\0')
  {
    ++len;
  }
  return len;
}
size_t strnlen(const char *s, size_t maxlen)
{
  size_t len = 0;
  while (s[len] != '\0')
  {
    ++len;
  }
  if (len < maxlen)
    return len;
  else
    return maxlen;
}

char *strcpy(char *dst, const char *src)
{
  int i;
  for (i = 0; src[i] != '\0'; ++i)
  {
    dst[i] = src[i];
  }
  dst[i] = '\0';
  return dst;
}

char *strncpy(char *dst, const char *src, size_t n)
{
  int i = 0;
  for (i = 0; i < n && src[i] != '\0'; ++i)
  {
    dst[i] = src[i];
  }
  dst[i] = '\0';
  return dst;
}

char *strcat(char *dst, const char *src)
{
  int i, j;
  for (i = 0; dst[i] != '\0'; ++i)
  {
  }
  for (j = 0; src[j] != '\0'; ++j, ++i)
  {
    dst[i] = src[j];
  }
  dst[i] = '\0';
  return dst;
}

int strcmp(const char *s1, const char *s2)
{
  int i;
  for (i = 0; s1[i] != '\0' && s2[i] != '\0'; ++i)
  {
    if (s1[i] < s2[i])
      return -1;
    if (s1[i] > s2[i])
      return 1;
  }
  if (s1[i] != '\0')
    return -1;
  if (s2[i] != '\0')
    return 1;
  return 0;
}

int strncmp(const char *s1, const char *s2, size_t n)
{
  int i;
  for (i = 0; i < n && s1[i] != '\0' && s2[i] != '\0'; ++i)
  {
    if (s1[i] < s2[i])
      return -1;
    if (s1[i] > s2[i])
      return 1;
  }
  if (i == n)
    return 0;
  return (int)(s1[i]) - (int)(s2[i]);
}

void *memset(void *s, int c, size_t n)
{
  char *target = (char *)s;
  for (int i = 0; i < n; ++i)
    target[i] = (char)c;
  return s;
}

void *memmove(void *dst, const void *src, size_t n)
{
  char *c_dst = (char *)dst;
  char *c_src = (char *)src;
  if (dst < src)
  {
    for (int i = 0; i < n; ++i)
      c_dst[i] = c_src[i];
  }
  else if (dst > src)
  {
    for (int i = n - 1; i >= 0; --i)
      c_dst[i] = c_src[i];
  }
  return dst;
}

void *memcpy(void *out, const void *in, size_t n)
{
  char *c_out = (char *)out;
  const char *c_in = (const char *)in;
  for (int i = 0; i < n; ++i)
  {
    c_out[i] = c_in[i];
  }
  return out;
}

int memcmp(const void *s1, const void *s2, size_t n)
{
  const char *c_s1 = (const char *)s1;
  const char *c_s2 = (const char *)s2;
  int i;
  for (i = 0; i < n; ++i)
  {
    if (c_s1[i] != c_s2[i])
      return (int)(c_s1[i]) - (int)(c_s2[i]);
  }
  if (i == n)
    return 0;
  return (int)(c_s1[i]) - (int)(c_s2[i]);
}

#endif
