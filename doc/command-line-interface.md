# Command-Line Interface

```bash
$ plasp [files] [options]
```

`[files]` may be omitted, in which case the input is read from `std::cin`.
The `[options]` are listed below:

| **option**                        | **explanation**                                                                                                                   |
|-----------------------------------|-----------------------------------------------------------------------------------------------------------------------------------|
| `-l` [ `--language` ]             | Specify the input language (`sas` or `pddl`). Omit for automatic detection.                                                       |
| `--warning-level` arg (=`normal`) | Specify whether to output warnings normally (`normal`), to treat them as critical errors (`error`), or to ignore them (`ignore`). |
| `--color` arg (=`auto`)           | Specify whether to colorize the output (`always`, `never`, or `auto`).                                                            |
