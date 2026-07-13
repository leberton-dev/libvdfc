# LIBVDFC

## Highlights
- A simple API in pure C without external dependencies
- Correct handling of escaped characters (`\"...\"`, etc.) inside quoted strings.


## Overview
The .vdf file extension stands for Valve Data Format. It uses a key-value structure.
I wanted to create a lib to parse/extract Steam data, and realized I needed a solid VDF parser.
It is for anyone who wants to use it :).


### Authors
- [TheLeBerton](https://github.com/TheLeBerton)


## Usage
> Typical flow: read a file, parse it, walk the tree, free it.
```c
#include "vdfc/vdf.h"
#include <stdio.h>

char    *out;
size_t  out_size;
VDFcode err;

err = vdf_read_file("path/to/file.vdf", &out, &out_size);
if (err != VDF_OK) {
    // handle
    exit(1);
}

VDFNode *node;
err = vdf_parse(out, &node);
if (err != VDF_OK) {
    // handle
    exit(1);
}

// Assuming file.vdf contains:
// "AppState"
// {
//      "name" "Counter-Strike"
//      "appid" "10"
// }

VDFNode *appstate = vdf_get(node, "AppState");
const char *name = vdf_get_string(appstate, "name", "unknown");
printf("%s\n", name);

free(out);
vdf_free_node(node);
```


## Installation
- Builds with default `cc`, no explicit `-std=` set
- `make test` - run the test suite

```bash
git clone https://github.com/leberton-dev/libvdfc.git
cd libvdfc
make

# in your project
cc -I/path/to/libvdfc/include your_file.c /path/to/libvdfc/libvdfc.a -o your_program
```


## Feedback and Contributing
- For bugs and suggestions see the [issues](https://github.com/leberton-dev/libvdfc/issues)
- PRs are welcome, please use `make test` before submitting
- Code style is enforced via `.clang-format`; run `clang-format -i` before submitting

