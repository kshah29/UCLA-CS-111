*Connect:*

sock fd - client side
struct sock_addrin  ... server side

hostent - returns server information - gethostbyname - needed if done over internet
copy these info using bcopy
otherwise localhost or ip address of it

`port - argument accepted`
`address - gethostadress or localhost`

single address - multiple port nos.

bind socket to server address on client side

listen (): sockfd returned by socket ()
  backlog - max connections can listen to - usually 5


run on server side - final command: accept ()
aceept uses same/prev
    going to return a sockfd - new sockfd created for data transfer



listen & accept - establish connection

- socket bind listen accept - server
- client - socket connect


*pipes , fork ,....*

*encryption*
mycrypt
mode different - based on packets, string

mycrypt_generic_init : creates key for encryption, decryption


once input and once output all the steps
