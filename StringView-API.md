## API Overview
The following is an overview of the public interface declared in the header.

### Construction

| Function       | Purpose                                             |
| -------------- | --------------------------------------------------- |
| `sv()`         | Create a view from a null-terminated C string       |
| `sv_to_cstr()` | Convert a view into a separately allocated C string |

### Character access

| Function     | Purpose                             |
| ------------ | ----------------------------------- |
| `sv_at()`    | Access a character at a given index |
| `sv_front()` | Access the first character          |
| `sv_back()`  | Access the last character           |

### Chop and substring operations

| Function             | Purpose                                         |
| -------------------- | ----------------------------------------------- |
| `sv_chop_right()`    | Remove characters from the right side of a view |
| `sv_chop_left()`     | Remove characters from the left side of a view  |
| `sv_chop_by_delim()` | Split a view using a delimiter character        |
| `sv_chop_by_type()`  | Split a view according to a character predicate |
| `sv_substr()`        | Obtain a substring view                         |
| `sv_prefix()`        | Obtain a prefix of a given length               |
| `sv_suffix()`        | Obtain a suffix of a given length               |

### Trimming

| Function             | Purpose                                                   |
| -------------------- | --------------------------------------------------------- |
| `sv_trim_left()`     | Remove leading whitespace from a view                     |
| `sv_trim_right()`    | Remove trailing whitespace from a view                    |
| `sv_trim()`          | Remove whitespace from both ends                          |
| `sv_trimmed()`       | Return a trimmed view without modifying the original view |
| `sv_trimmed_left()`  | Return a view without leading whitespace                  |
| `sv_trimmed_right()` | Return a view without trailing whitespace                 |

### Searching

| Function            | Purpose                                          |
| ------------------- | ------------------------------------------------ |
| `sv_find()`         | Find the first occurrence of a character         |
| `sv_find_from()`    | Find a character starting at a given index       |
| `sv_find_sv()`      | Find the first occurrence of another string view |
| `sv_find_sv_from()` | Find a substring starting at a given index       |
| `sv_rfind()`        | Find the last occurrence of a character          |
| `sv_rfind_sv()`     | Find the last occurrence of a substring          |

Search functions use `SV_NOT_FOUND` to represent a failed search.

### Comparison

| Function              | Purpose                                                             |
| --------------------- | ------------------------------------------------------------------- |
| `sv_equal()`          | Test whether two views contain the same characters                  |
| `sv_equal_nocase()`   | Test equality without considering letter case                       |
| `sv_compare()`        | Compare two views lexicographically                                 |
| `sv_compare_nocase()` | Compare two views lexicographically without considering letter case |

### Classification

The classification functions accept a predicate that determines whether an individual character satisfies a condition.

| Function    | Purpose                                                     |
| ----------- | ----------------------------------------------------------- |
| `sv_all()`  | Test whether every character satisfies the predicate        |
| `sv_any()`  | Test whether at least one character satisfies the predicate |
| `sv_none()` | Test whether no character satisfies the predicate           |

Standard C character predicates, such as `isspace()` and `isdigit()`, can be useful for these operations.

