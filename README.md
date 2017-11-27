# CustomTCP
Academic - Custom TCP Implementation for Linux Systems

My project applies this custom TCP implementation by sending messages between separate ‘client’ and ‘server’ applications. 
Both applications prompt the user to see which protocol mode they should run with, Custom or TCP (for comparison). 
The client application also prompts the user for connection parameters and a path to a text file containing sample data to transfer. 
The server should be started first as it will wait for the client to begin the TCP handshake procedure. 
If both applications are running using the Custom protocol the following events will occur:

1.	Socket Creation: Both the server and client will allocate and configure raw sockets using the SOCK_RAW and IPROTO_TCP options. 
These sockets are bound to the source address/port given as configuration parameters for the server and client.

2.	Handshake: The client will initiate the 3-step handshake protocol with a target address/port by sending a SYN Packet. 
The server listening on that address/port will record the received address/port as well as the TCP window size before acknowledging 
with a SYN-ACK packet. The client will complete the connection by sending its own ACK packet.

3.	Data Transfer: Once the client has confirmed the connection it will begin sending the first packet containing data from the sample file.
Both the client and server use the system socket implementations of revfrom and sendto to read and write to the sockets. 
The client will break up the file into individual packets depending on a configured packet_size variable. 
The client details the packet size in the TCP window field so that the server knows how much to read in the buffer. 
The server will respond with an ACK, an ACK_SEQ containing the received data size, and the remaining window size. 
If the client receives this ACK and there is data remaining in the window, the client it will continue sending data with updated 
SEQ numbers matching the start of each segment. 
If at any point the ACK or ACK_SEQ don’t match what the client is expecting it will retransmit the same packet.

4.	Acknowledge Last Packet: Normally a 4 way disconnect procedure would take place in TCP. 
However, in this simplified implementation we stop the connection on the client side when 
we receive an acknowledgement from the server that contains a Zerowindow 

To run this application first run the Server application and provide it with the TCP method you would lie to use (Custom / TCP).
In a separate process run the Client application. Provide it with a path to a sample text file and the TCP method you'd like to use and it will
pass along all the test data. 

You can also run all the unit tests provided with googletest in order to see individual tests of each piece.

