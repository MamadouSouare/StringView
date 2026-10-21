# StringView

A lightweight `StringView` library written in C.

**StringView** is a small C library designed to make working with strings more convenient by providing a non-owning view of character sequences. It offers an alternative to traditional null-terminated string handling, with an API inspired by the concept of string views found in other programming languages.

> **Status:** Work in progress. The API and implementation may change as the project evolves.

## About the project

I discovered the concept of `StringView` through [Tsoding](https://www.youtube.com/@TsodingDaily) on YouTube. His content introduced me to the idea, but I wanted to explore it further and understand how far I could take the concept in C.

That curiosity led me to start developing this library.

My goal is to build a practical collection of string-view operations, explore different approaches to string manipulation in C, and deepen my understanding of memory management, pointers, and low-level programming.

This project is both a learning experience and an attempt to create a useful, lightweight library.

## What is a StringView?

A `StringView` is a lightweight structure that references a sequence of characters without owning or copying the underlying string.

Unlike a conventional C string, which relies on a null terminator (`'\0'`), a string view stores a pointer to the character data and an explicit length.

Conceptually, a `StringView` contains:

- `data`: a pointer to the beginning of the character sequence.
- `count`: the number of characters in the view.

For example, a view can reference only part of an existing string without allocating a new buffer or modifying the original data.

### Why use StringView?

- **Avoid unnecessary allocations:** Creating a view does not copy the characters.
- **Efficient substring operations:** Extract prefixes, suffixes, and substrings by adjusting the view.
- **Explicit length:** Work with character sequences without relying on a null terminator at the end of every view.
- **Non-destructive manipulation:** Inspect or transform views while leaving the underlying string unchanged.
- **A consistent API:** Group common operations such as searching, trimming, comparison, and classification.

A string view does not own its data. The underlying character sequence must remain valid for as long as the view is used.

## Features

The library is being developed around the following categories:

| Category       | Operations                                                              |
| -------------- | ----------------------------------------------------------------------- |
| Construction   | Create a view and convert it to a C string                              |
| Access         | Access characters at specific positions                                 |
| Chop           | Remove characters from either end and split by delimiters or predicates |
| Substrings     | Extract substrings, prefixes, and suffixes                              |
| Trim           | Remove leading and trailing whitespace                                  |
| Search         | Find characters and substrings                                          |
| Comparison     | Compare views, with optional case-insensitive operations                |
| Classification | Test whether all, any, or none of the characters satisfy a predicate    |

Some functions are still under development. See the header file and implementation for the current state of the API.

## Getting started

### Requirements

- A C compiler supporting the C standard used by the implementation.
- The standard C library.
- The library header and its corresponding implementation file.

### Installation

The library is currently intended to be used directly from its source files.

1. Clone the repository:

   ```bash
   git clone https://github.com/MamadouSouare/StringView.git
   ```

2. Include the header in your C source file:

   ```c
   #include "StringView/StringView.h"
   ```

3. Compile your program together with the library implementation file.

   ```bash
   gcc main.c ./StringView/StringView.c -o main
   ```

Replace the filenames with the actual names used in the repository.

## Usage

### Creating and printing a StringView

```c
#include <stdio.h>
#include "StringView/StringView.h"

int main(void)
{
    StringView text = sv("Hello, World!");

    printf("Text: " sv_fmt "\n", sv_arg(text));

    return 0;
}
```

Output:

```text
Text: Hello, World!
```

The `sv()` constructor creates a view of the supplied null-terminated C string.

The `sv_fmt` and `sv_arg()` macros simplify printing a `StringView` with `printf()`.

### Understanding the representation

Given the string:

```text
Hello, World!
```

A `StringView` can represent the entire string or a smaller portion of it.

For example, a view of `World` can reference the original character array directly, without allocating a separate string for those five characters.

This is one of the main ideas behind the library: **manipulate views of strings rather than constantly copying strings.**

## API overview
Find the **API overview** [here](./StringView-API.md)
## Important considerations

### Memory ownership

A `StringView` does not own the memory it references.

Creating a view does not extend the lifetime of the underlying data. Avoid using a view after the referenced memory has been freed, has gone out of scope, or has otherwise become invalid.

### Null termination

A view is defined by its pointer and length, not by a terminating null character.

Do not assume that `sv.data` points to a null-terminated substring. Use the view's length when accessing or printing its contents.

### Allocations

Most view-based operations can work without allocating new memory. However, `sv_to_cstr()` is intended to produce a standalone C string and therefore requires separate memory management.

The caller should release the returned allocation with `free()` when it is no longer needed, assuming the implementation follows this ownership contract.
