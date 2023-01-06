# **CSHELL**

<p>cshell is a simple command line shell that is written in C++. It has several features including command history and command alias.</p>

### **FEATURES**

---

Command history

- The user can view their command history by typing `history`
- The user can also execute a previous command by typing history followed by the number of the command they want to execute. For example, `history 3` will execute the third command in the history.

Command alias

- The user can set their own aliases for frequently used commands by using the `alias` command. For example, `alias l=ls -la` will set the alias `l` to run the `ls -la` command.
- The shell allows the user to define their own aliases for commonly used commands. Currently, the shell supports a total of 173 aliases.

Custom color

- the ability to change the color of the terminal prompt by using the `setcolor` command, with options for red, blue, or green.
- To change the color of the terminal prompt, use the `setcolor` command followed by the desired color (e.g. `setcolor red`).

### **Compiling and Running**

<p>cshell can be compiled and run on a Unix-like system by following these steps:</p>

---

1. Clone the repository

```bash
git clone https://github.com/adhithyanmv/cshell.git
```

2. Navigate to the cloned repository

```
cd cshell
```

bash 3. Compile the source code

```bash
g++ shell.cpp -o shell
```

4. Run cshell

```bash
./cshell
```
