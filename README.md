# Multiuser-Chat
Working: The multi-user chat system is implemented using the client-server model using socket programming over UNIX system. First the server is set up. Then connections are made with different clients, each corresponding to a particular user. All messages are sent through server. Server maintains a list of client struct where username and socket Ids of all the clients is stored. This helps in sending message to a particular user.

Compiling: Just run the Makefile.

Testing: First run the server. Then run the clients passing username as arguments.

Input: Just typing the message would result in send to all operation. However adding @<enter username here> would send the message to that particular user.

Output: Message recieved from a particular user, would be of the form <Sender>: <Message>.

Errors Handled: If the user tries to send message to a non existent username, no such user occurs.
Handles same username for more than one users.

References: https://gist.github.com/Abhey/47e09377a527acfc2480dbc5515df872
https://codingile.com/multiuser-chat-server-in-c/
https://www.ibm.com/support/knowledgecenter/en/ssw_ibm_i_71/rzab6/xafunixsrv.htm
