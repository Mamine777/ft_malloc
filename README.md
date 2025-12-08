# 🧠 ft_malloc — Custom Memory Allocator (malloc / free / realloc)

`ft_malloc` is a full re-implementation of the C memory allocator using `mmap()`, custom memory zones, manual block management, and thread synchronization.

This project reproduces the logic of real allocators (like glibc, jemalloc, ptmalloc) and provides full control over memory:

- Page allocation using `mmap()`
- Tiny / Small / Large zones
- Block splitting & coalescing
- Custom free lists
- In-place `realloc` expansion
- Thread-safe allocation (`pthread_mutex`)
- Manual pointer arithmetic

Understanding and implementing this project gives deep knowledge of how memory allocators work behind the scenes.

---

## 🚀 Features

### 🔹 Three Allocation Zones

To reduce fragmentation and optimize speed, allocations are divided into size classes:

| Zone  | Max Size     | Behavior |
|-------|--------------|----------|
| **TINY**  | ≤ 128 bytes  | Stored inside pre-allocated memory pages |
| **SMALL** | ≤ 4096 bytes | Stored inside larger pre-allocated pages |
| **LARGE** | > 4096 bytes | Each request gets its own `mmap()` region |

---

### 🔹 Zone Structure

Each zone is one large chunk allocated via `mmap()`:

