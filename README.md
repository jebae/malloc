# malloc

malloc project clone UNIX dynamic memory allocation function `malloc`, `free`, `realloc`.
In basic, `malloc` use `mmap` to allocate private purposed page without mapping to specific file. `free` uses `munmap` to deallocate page.
To prevent memory external fragmentation, this project use multi level buddy block algorithm.

<br/><br/>

## Requirements

`malloc` use [libft](https://github.com/jebae/libft).

```
# download libft
git clone https://github.com/jebae/libft.git

# directory structure
root/
 libs/
  libft
```

<br/><br/>

## Install

```
make
```

<br/><br/>

## Usage

When build completed `libft_malloc.so` is built at project directory.

```
# compile with libftft_malloc.so
gcc -L <malloc directory path> -lft_malloc <your source>

# execute with specified DYLD_LIBRARY_PATH
DYLD_LIBRARY_PATH=<malloc directory path> ./executable
```

<br/><br/>

## Features

```c
void *malloc(t_uint64 size);

void free(void *ptr);

void *realloc(void *ptr, t_uint64 size);

// show allocated block status
void show_alloc_mem(void);
```
