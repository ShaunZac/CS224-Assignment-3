Objective: Build ARQ on top of UDP Sockets

We have created two files, receiver.c and sender.c, which perform the task of implementing ARQ on UDP sockets. 

sender.c
The code is sender.c is responsible for sending a fixed number of packets (taken from the command line) to the receiver. Once it has sent a message, it goes into polling mode, where it looks for an ack sent by the receiver for rtt duration (also taken from the command line). If it does not receive any response for rtt time, it resends the same packet and waits for an ack. If it receives an ack with the correct sequence number, it sends the next packet.

receiver.c
The code in receiver.c is responsible for receiving any number of packets from the sender socket. Once it has received a packet, if it has the correct sequence number, it drops it with a fixed probability p (taken from the command line) and sends an ack with probability 1-p. It infinitely looks for packets that it can receive.

Running the code:
The code can be compiled using standard commands:
gcc -o receiver receiver.c
gcc -o sender sender.c

Once this is done, we need two terminals open in the working directory to get the appropriate output. In one of the terminals, we need to first run the receiver as:
./receiver <ReceiverPort> <SenderPort> <PacketDropProbability>

In the other terminal, we run the sender as:
./sender <SenderPort> <ReceiverPort> <RetransmissionTimer> <NoOfPacketsToBeSent>

Received output for the following commands:
./receiver 8080 8081 0.5
./sender 8081 8080 0.2 10

Output on Receiver Terminal:
Sender : Packet:1

Acknowledgement:2

Sender : Packet:2

Acknowledgement:3

Sender : Packet:3

Acknowledgement:4

Sender : Packet:4

Acknowledgement:5

Sender : Packet:5

Acknowledgement:6

Sender : Packet:6

Acknowledgement:7

Sender : Packet:7

Acknowledgement:8

Sender : Packet:8

Acknowledgement:9

Sender : Packet:9

Acknowledgement:10

Sender : Packet:10

Acknowledgement:11

Output on Sender Terminal:
Packet:1

Message sent.
Successful tx
Receiver : Acknowledgement:2

Packet:2

Message sent.
Successful tx
Receiver : Acknowledgement:3

Packet:3

Message sent.
Successful tx
Receiver : Acknowledgement:4

Packet:4

Message sent.
Packet lost
Packet:4

Message sent.
Packet lost
Packet:4

Message sent.
Successful tx
Receiver : Acknowledgement:5

Packet:5

Message sent.
Successful tx
Receiver : Acknowledgement:6

Packet:6

Message sent.
Packet lost
Packet:6

Message sent.
Successful tx
Receiver : Acknowledgement:7

Packet:7

Message sent.
Successful tx
Receiver : Acknowledgement:8

Packet:8

Message sent.
Successful tx
Receiver : Acknowledgement:9

Packet:9

Message sent.
Successful tx
Receiver : Acknowledgement:10

Packet:10

Message sent.
Successful tx
Receiver : Acknowledgement:11