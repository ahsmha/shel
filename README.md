## Smash

Smash is a simple implementation of a shell in C

### What it can do?
- Can do all the basic features any shell has i.e, `read`, `parse`, `fork`, `exec` and `wait`
- Builtin commands are: `cd`, `help`, `exit`

### What it can't do?
- Doesn't work for multiline commands
- Arguments should be seperated by whitespace
- No piping or redirection

### Running
`gcc -o smash src/main.c` to compile. And then `./smash` to run

### Helpful links / Resources
- [POSIX standard](https://pubs.opengroup.org/onlinepubs/9699919799/utilities/contents.html)
- [The Architecture of Open Source Applications](http://www.aosabook.org/en/bash.html)
