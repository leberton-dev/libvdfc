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
#include <stdio.h>
#include <vdfc/vdf.h>

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

char *out;
err = vdf_dump_node(node, &out)
if (err != VDF_OK)
{
    // handle
    exit(1);
}
printf("%s\n", out);

// Should output something like:
// (null): object
//   AppState: object
//     name: string
//     appid: string

VDFNode *appstate = vdf_get(node, "AppState");
const char *name = vdf_get_string(appstate, "name", "unknown");
printf("%s\n", name);

free(out);
vdf_free_node(node);
```


## Installation
- A simple `make install` to install it on your system and `make uninstall` to uninstall it.
> Attention: currently it only installs and uninstalls it on `/usr/local` as prefix meaning `$prefix/lib` and `$prefix/include`.
- `make test` - run the test suite ( criterion is needed to run the test suites. )

```bash
git clone https://github.com/leberton-dev/libvdfc.git
cd libvdfc
make install

# in your project
cc your_file.{.c,.o} -lvdfc -o prog_name
```

The installed packages are found under `$prefix/lib/libvdf.ca` and `$prefix/include/vdfc/`.
`$prefix` beeing `/usr/lib` at the moment.


## Feedback and Contributing
- For bugs and suggestions see the [issues](https://github.com/leberton-dev/libvdfc/issues)
- PRs are welcome, please use `make test` before submitting
- Code style is enforced via `.clang-format`; run `clang-format -i` before submitting

