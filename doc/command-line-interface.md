# Command-Line Interface

```bash
$ plasp [options] file...
```

`plasp` automatically detects the language of the input files.

Multiple files may be provided in an arbitrary order.
The `file...` arguments may also be omitted, in which case the input is read from `std::cin`.

`plasp` supports the following options:

| **option** | **explanation** |
|------------|-----------------|
| `-l` [ `--language` ] arg (=`auto`) | Input language (`pddl`, `sas`, `auto`) |
| `--warning-level` arg (=`show`) | Show warnings (`show`), treat them as errors (`error`), or ignore them (`ignore`) |
| `--color` arg (=`auto`) | Colorize output (`always`, `never`, `auto`) |
