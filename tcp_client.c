#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <netdb.h>

#define PORT 8010

void
write_to_server (int filedes, char *msg)
{
  int nbytes;

  nbytes = write (filedes, msg, strlen (msg));
  if (nbytes < 0)
  {
    perror ("write");
    exit (EXIT_FAILURE);
  }
}

int
main (int argc, char *argv[])
{
  if (argc < 2)
  {
    printf ("Usage: %s py_script\n\npy_script -- path to Python script that openstartracker will execute.\n\n", argv[0]);
    exit (EXIT_FAILURE);
  }
  
  /* create a socket */
  int network_socket, connection_status, nbytes;
  network_socket = socket (AF_INET, SOCK_STREAM, 0);

  /* specify an address for the socket */
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons (PORT);
  server_address.sin_addr.s_addr = INADDR_ANY;

  connection_status = connect (network_socket,
				  (struct sockaddr *) &server_address,
				  sizeof (server_address));
  /* check for error with the connection */
  if (connection_status == -1)
  {
    perror ("connect (client)");
    exit (EXIT_FAILURE);
  }

  /* send data to the server */
  char server_msg[256];
  snprintf (server_msg, sizeof (server_msg),
	    "exec(compile(open(\"%s\", \"rb\").read(), \"%s\", 'exec'))",
	    argv[1], argv[1]);
  write_to_server (network_socket, server_msg);

  printf ("The server sent the data:\n\n");
  while (1)
  {
    /* receive data from the server */
    char server_response[256];
    nbytes = recv (network_socket, &server_response, sizeof (server_response), 0);
    if (nbytes < 0)
    {
      /* read error */
      perror ("read");
      exit (EXIT_FAILURE);
    }

    /* print out the server's response */
    printf ("%s\n", server_response);
  }
      
  close (network_socket);
  exit (EXIT_SUCCESS);
}
