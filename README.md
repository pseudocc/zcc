# ZCC

C containers imitating C++ standard containers with strict type check.

## C Standard

GNU 11

## Documentation

manual pages (NOT STARTED).

## VECTOR

Example usage, for more, take a look at the UT cases under `test/zvec/`.

```c
zvec_t(int) vec = zvec_new(int);

zvec_push_back(vec, 2);
zvec_insert(vec, zvec_begin(vec), 1);

// use C++ like iterator to access the items
for (zvec_it(int) it = zvec_begin(vec); it < zvec_end(vec); it++)
  printf("%d ", *it); // 1 2

// use direct index to access the items
int* data = zvec_data(vec);
for (int i = 0; i < zvec_size(vec); i++)
  printf("%d %d ", zvec_at(vec, i), data[i]); // 1 1 2 2
```
