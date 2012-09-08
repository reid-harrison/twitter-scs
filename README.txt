README.TXT
Reid Harrison
CS 3251 (Networking 1) Project 2
Simple C twitter-like service

================================================
TO COMPILE
================================================
Simply type "make"

================================================
TO RUN
================================================
First, run the server:
    ./server <PORT NUM>
    
The server will run on 127.0.0.1:<PORT NUM>

Next, run the client with the necessary options.
  To upload:
    ./client -u 127.0.0.1 13000 "YOUR TWEET HERE" "SUBJECT"
    
  To download with no subscription:
    ./client -d -nosub 127.0.0.1 13000 "SUBJECT"
    
  To download with subscription:
    ./client -d -sub 127.0.0.1 13000 "SUBJECT"
    
  SUBJECT must be either eat, sleep, or networking.

================================================
THE PROTOCOL
================================================
The protocol for this application is incredibly simple. The client sends messages in the following format:
    
    For upload:
        "upload<?>SUBJECT<?>TWEET<?>"
        
    For download:
        "download<?>SUBJECT<?>"
        
    The server parses the command from the client and then runs accordingly. For downloads, the server simply sends the tweets for the given subject back, each one separated by their <?> token.
    
When a client chooses to subscribe, the client will continually poll the server in the background. If the server responds with tweets that the client doesn't already have then the new list will be printed.

================================================
KNOWN BUGS OR LIMITATIONS
================================================
-Client continually polls the server, this is inefficient. It would be better if the client sent a hash of the list they last received. A long-poll would be preferred if the hash matches.
-Server is blocking and new client connections must be made for each request.
-Errors cause exits
-No checks on tweet size.
-Only check on tweet content ensures that it does not contain the two reserved delimiters.
-There's probably a bug or two that I have not run into.
