#ifndef DEFS_HPP
#define DEFS_HPP

#define NOMOVE(T)                                                              \
  T(T &&) noexcept = delete;                                                   \
  T &operator=(T &&) noexcept = delete;

#define NOCOPYMOVE(T)                                                          \
  T(const T &) = delete;                                                       \
  T &operator=(const T &) = delete;                                            \
  NOMOVE(T)

#endif
