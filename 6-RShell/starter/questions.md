1. How does the remote client determine when a command's output is fully received from the server, and what techniques can be used to handle partial reads or ensure complete message transmission?

It determines when a commands output is fully recieved from the server by checking if the last character of the recieved message is an EOF. The server sends an EOF after it has sent all that needs to be sent. The client loops and recieves messages until it recieves an EOF. When the EOF is recieved it knows the message is complete.

2. This week's lecture on TCP explains that it is a reliable stream protocol rather than a message-oriented one. Since TCP does not preserve message boundaries, how should a networked shell protocol define and detect the beginning and end of a command sent over a TCP connection? What challenges arise if this is not handled correctly?

A networked shell protocol can specifiy and specific character or sequence of characaters to signal the begining and ends of a message. If this is not handleded correctly, this can lead to a client or server waiting for a message to be finished indefinitlely.

3. Describe the general differences between stateful and stateless protocols.

In a stateful protocol, the clients past messages are remembered. However, this is not the case for stateless protocols. Statful protocols are generally more reliable because they allow for more verification regarding the ordering and arrival of information, where as it is much more difficult for stateless protocols to implement this.

4. Our lecture this week stated that UDP is "unreliable". If that is the case, why would we ever use it?

UDP can be used when it is acceptable for a small percetnage of information to be missing, and when extra speed is beneficial. For example, when streaming video, it is acceptable for small pieces of information to be missing because it likely wont be noticable.

5. What interface/abstraction is provided by the operating system to enable applications to use network communications?

Sockets and their related system calls are an interface that allows applications to use network connections. In linux they are abstracted as file descriptors.