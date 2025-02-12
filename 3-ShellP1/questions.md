1. In this assignment I suggested you use `fgets()` to get user input in the main while loop. Why is `fgets()` a good choice for this application?

    > **Answer**:  fgets() terminates on a newline character, end of string, or end of file. This is a good choice because there will always be a newline character at the end of the input. It also reads up to a maximum amount of characters. This enables us to more easily limit the length of commands. This also enables us to write less code.

2. You needed to use `malloc()` to allocte memory for `cmd_buff` in `dsh_cli.c`. Can you explain why you needed to do that, instead of allocating a fixed-size array?

    > **Answer**: It is not necesary. This is because we know the maximum size of a command, so it is not necesarry to allocate memory dynamicaly. Using malloc wont save any significant space. 


3. In `dshlib.c`, the function `build_cmd_list(`)` must trim leading and trailing spaces from each command before storing it. Why is this necessary? If we didn't trim spaces, what kind of issues might arise when executing commands in our shell?

    > **Answer**: This is necesary because users may accidentally put spaces before or after. This will make it difficult to comapre inputted commands to the known commands. It would also make it very difficult to seperate commands from arguments since they are eperated by spaces. 

4. For this question you need to do some research on STDIN, STDOUT, and STDERR in Linux. We've learned this week that shells are "robust brokers of input and output". Google _"linux shell stdin stdout stderr explained"_ to get started.

- One topic you should have found information on is "redirection". Please provide at least 3 redirection examples that we should implement in our custom shell, and explain what challenges we might have implementing them.

    > **Answer**:  We want to be able to redirect a process's stdout to files. To do this, it could be challenging to get information from a specifc process's standard out. We should also be able to redirect a file into stdin. We would need to read the file and redirect it into a program's stdin. I could imagine that it is challenging to write to a processes stdin while it is running. We should also be able to redirect stderr to a file.

- You should have also learned about "pipes". Redirection and piping both involve controlling input and output in the shell, but they serve different purposes. Explain the key differences between redirection and piping.

    > **Answer**:  Pipes connect the stdout of a process to the stdin of another process. Redirection writes the output of a process to a file, or connects the information in a file to the stdin of a process. 

- STDERR is often used for error messages, while STDOUT is for regular output. Why is it important to keep these separate in a shell?

    > **Answer**:  These should be kept seperate because it may be difficult to find an error message if the program has a lot of output. An error message in the output also may interfere with whatever is recieving the output. It makes error handling much simpler. 

- How should our custom shell handle errors from commands that fail? Consider cases where a command outputs both STDOUT and STDERR. Should we provide a way to merge them, and if so, how?

    > **Answer**: We can make both stderr and stdout default to printing to the screen. However, there should also be a way to redirect these to another file or process. 