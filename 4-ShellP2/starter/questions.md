1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

    > **Answer**:  execvp replaces the memory of the current process with the memory of a new process. Calling execvp without calling fork would replace the shell with the memory of another process. Calling fork creates a child process so that execvp will replace the memroy of the child process, not the memeory of the shell.

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

    > **Answer**:  If fork() fails then then execvp will not be called, and the shell will exit. Forking is required to run another process.

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

    > **Answer**:  execvp() searches through the directories defined bythe PATH environment variable. 

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

    > **Answer**:  Calling wait() stop the execution of the parent process until the forked process finishes running. If wait was not called, then then shell may print another prompt before the output of the child process is printed. This would make it difficult to input commands and may lead to a user running two processes at the same time.

5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

    > **Answer**: WEXITSTATUS() allows the parent process to see the exit status of the child process. This is important because the parent process may need to know the exit status to deal with errors. 

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

    > **Answer**:  Spaces inside of quoted arguments are ignored. This is necesary because by default, the shell sperates arguments using spaces. So if a user wants an argument to have spaces they can surround the argument in quotes. 

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

    > **Answer**:  The parsing logic now deals with quoted spaces. It also only uses one comand buff insteading of using seperate variables. This did not lead to any unexpected challenges.

8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

    > **Answer**:  _start here_

- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?

    > **Answer**:  _start here_

- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

    > **Answer**:  _start here_