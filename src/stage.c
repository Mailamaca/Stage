#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <signal.h>

#if HAVE_CONFIG_H
  #include <config.h>
#endif

//#define DEBUG
#include "stage.h"


const char* stg_model_string( stg_model_type_t mod )
{
  switch( mod )
    {
    case STG_MODEL_GENERIC: return "generic"; break;
    case STG_MODEL_WALL: return "wall"; break;
    case STG_MODEL_POSITION: return "position"; break;
    case STG_MODEL_LASER: return "laser"; break;
    case STG_MODEL_SONAR: return "sonar"; break;
    default: break;
    }
  return "unknown"; 
} 

const char* stg_property_string( stg_prop_id_t id )
{
  switch( id )
    {
    case STG_PROP_CREATE_WORLD: return "STG_PROP_CREATE_WORLD"; break;
    case STG_PROP_DESTROY_WORLD: return "STG_PROP_DESTROY_WORLD"; break;
    case STG_PROP_CREATE_MODEL: return "STG_PROP_CREATE_MODEL"; break;
    case STG_PROP_DESTROY_MODEL: return "STG_PROP_DESTROY_MODEL"; break;
    case STG_PROP_CIRCLES: return "STG_PROP_CIRCLES"; break;
    case STG_PROP_COLOR: return "STG_PROP_COLOR"; break;
    case STG_PROP_COMMAND: return "STG_PROP_COMMAND"; break;
    case STG_PROP_DATA: return "STG_PROP_DATA"; break;
    case STG_PROP_GEOM: return "STG_PROP_GEOM"; break;
    case STG_PROP_IDARRETURN: return "STG_PROP_IDARRETURN"; break;
    case STG_PROP_LASERRETURN: return "STG_PROP_LASERRETURN"; break;
    case STG_PROP_NAME: return "STG_PROP_NAME"; break;
    case STG_PROP_OBSTACLERETURN: return "STG_PROP_OBSTACLERETURN";break;
    case STG_PROP_ORIGIN: return "STG_PROP_ORIGIN"; break;
    case STG_PROP_PARENT: return "STG_PROP_PARENT"; break;
    case STG_PROP_PLAYERID: return "STG_PROP_PLAYERID"; break;
    case STG_PROP_POSE: return "STG_PROP_POSE"; break;
    case STG_PROP_POWER: return "STG_PROP_SONAR_POWER"; break;
    case STG_PROP_PPM: return "STG_PROP_PPM"; break;
    case STG_PROP_PUCKRETURN: return "STG_PROP_PUCKETURN"; break;
    case STG_PROP_RANGEBOUNDS: return "STG_PROP_RANGEBOUNDS";break;
    case STG_PROP_RECTS: return "STG_PROP_RECTS"; break;
    case STG_PROP_SIZE: return "STG_PROP_SIZE"; break;
    case STG_PROP_SONARRETURN: return "STG_PROP_SONARRETURN"; break;
    case STG_PROP_NEIGHBORRETURN: return "STG_PROP_NEIGHBORRETURN"; break;
    case STG_PROP_NEIGHBORBOUNDS: return "STG_PROP_NEIGHBORBOUNDS"; break;
    case STG_PROP_NEIGHBORS: return "STG_PROP_NEIGHBORS"; break;
    case STG_PROP_VELOCITY: return "STG_PROP_VELOCITY"; break;
    case STG_PROP_VISIONRETURN: return "STG_PROP_VISIONRETURN"; break;
    case STG_PROP_VOLTAGE: return "STG_PROP_VOLTAGE"; break;
    case STG_PROP_RANGERS: return "STG_PROP_RANGERS";break;
    case STG_PROP_LASER_DATA: return "STG_PROP_LASER_DATA";break;
    case STG_PROP_BLINKENLIGHT: return "STG_PROP_BLINKENLIGHT";break;
    case STG_PROP_NOSE: return "STG_PROP_NOSE";break;
    case STG_PROP_BORDER: return "STG_PROP_BORDER";break;
    case STG_PROP_LOS_MSG: return "STG_PROP_LOS_MSG";break;
    case STG_PROP_MOUSE_MODE: return "STG_PROP_MOUSE_MODE";break;
    case STG_PROP_MATRIX_RENDER: return "STG_PROP_MATRIX_RENDER";break;
	
      // todo
      //case STG_PROP_POSITION_ODOM: return "STG_PROP_POSITION_ODOM"; break;
      //case STG_PROP_POSITION_MODE: return "STG_PROP_POSITION_MODE"; break;
      //case STG_PROP_POSITION_STEER: return "STG_PROP_POSITION_STEER"; break;

    default:
      break;
    }
  return "unknown";
}

stg_model_type_t
stg_model_type_from_string( char* str )
{
  stg_model_type_t mod;
 
  if( strcmp( stg_model_string(STG_MODEL_POSITION), str )==0)
    mod = STG_MODEL_POSITION;
  
  else if( strcmp( stg_model_string(STG_MODEL_WALL), str )==0)
    mod = STG_MODEL_WALL;
  
  else if( strcmp( stg_model_string(STG_MODEL_LASER), str )==0)
    mod = STG_MODEL_LASER;
  
  //else if( strcmp( stg_model_string(STG_MODEL_SONAR), str )==0)
  //mod = STG_MODEL_SONAR;
  
  else
    { 
      PRINT_WARN1( "non-specific model created from token \"%s\"", str );
      mod = STG_MODEL_GENERIC;
    }

  return mod;
}


stg_property_t* stg_property_create( void )
{
  stg_property_t* prop;
  
  prop = (stg_property_t*)calloc(sizeof(stg_property_t),1);

  assert( prop );
  
  //PRINT_WARN1( "created property at %p", prop );

  // todo - insert the actual time in here
  prop->timestamp = 100.0;

  return prop;
}

void stg_property_free( stg_property_t* prop )
{
  //PRINT_WARN1( "freeing property at %p", prop );
  free( prop );
}

// lengthen prop by len bytes and copy in the data.
stg_property_t* stg_property_attach_data( stg_property_t* prop,
					  void* data, size_t len )
{
  assert( prop );
  
  PRINT_DEBUG1( "start: prop at %p", prop );
  //stg_property_print( prop );

  if( data && len > 0 )
    {  
      size_t totalsize = sizeof(stg_property_t)+len;
      
      PRINT_DEBUG1( "attaching %d bytes of data", len );
      
      assert( prop = (stg_property_t*)realloc(prop,totalsize) );
      
      /* record the amount of extra data allocated */
      prop->len = len;
      
      /* copy in the data */
      memcpy( prop->data, data, len );
    }
  else
    PRINT_DEBUG( "no data or zero length" );
  
  PRINT_DEBUG1( "result: prop at %p", prop );
  //stg_property_print( prop );
  
  return prop;
}


char* stg_property_sprint( char* str, stg_property_t* prop )
{
  sprintf( str, 
	   "Property @ %p - id: %d time: %.6f prop: %d data bytes: %d\n",
	   prop, prop->id, prop->timestamp, prop->property, prop->len );

  return str;
}

void stg_property_fprint( FILE* fp, stg_property_t* prop )
{
  char str[1000];
  stg_property_sprint( str, prop );
  fprintf( fp, "%s", str );
}

void stg_property_print( stg_property_t* prop )
{
  stg_property_fprint( stdout, prop );
}

size_t stg_packet_read_fd( int fd, void* buf, size_t len )
{
  PRINT_DEBUG1( "attempting to read a packet of max %d bytes", len );
  
  assert( fd > 0 );
  assert( buf ); // data destination must be good
  
  size_t recv = 0;
  // read a header so we know what's coming
  while( recv < len )
    {
      PRINT_DEBUG3( "reading %d bytes from fd %d into %p", 
		    len - recv, fd,  ((char*)buf)+recv );

      /* read will block until it has some bytes to return */
      size_t r = 0;
      if( (r = read( fd, ((char*)buf)+recv,  len - recv )) < 0 )
	{
	  if( errno != EINTR )
	    {
	      PRINT_ERR1( "ReadPacket: read returned %d", r );
	      perror( "system reports" );
	      break;
	    }
	}
      else if( r == 0 ) // EOF
	break; 
      else
	recv += r;
    }      
  
  PRINT_DEBUG2( "read %d/%d bytes", recv, len );
  
  return recv; // the number of bytes read
}

size_t stg_packet_read( stg_client_t* cli, void* buf, size_t len )
{
  return stg_packet_read_fd( cli->pollfd.fd, buf, len );
}
  
void stg_catch_pipe( int signo )
{
  puts( "caught SIGPIPE" );
}

ssize_t stg_packet_write_fd( int fd, void* data, size_t len )
{
  size_t writecnt = 0;
  ssize_t thiswritecnt;
  
  PRINT_DEBUG3( "writing packet on fd %d - %p %d bytes\n", 
		fd, data, len );
  
  while(writecnt < len )
  {
    thiswritecnt = write( fd, ((char*)data)+writecnt, len-writecnt);
      
    // check for error on write
    if( thiswritecnt == -1 )
      return -1; // fail
      
    writecnt += thiswritecnt;
  }
  
  PRINT_DEBUG2( "wrote %d/%d packet bytes\n", writecnt, len );
  
  return len; //success
}

size_t stg_packet_write( stg_client_t* cli, void* data, size_t len )
{
  return stg_packet_write_fd( cli->pollfd.fd, data, len );
}

// return a stg_property_t read from the channel. caller must free the 
// property. On failure the channel is closed and NULL returned.
stg_property_t* stg_property_read_fd( int fd )
{
  stg_property_t* prop = stg_property_create();

#ifdef DEBUG
  size_t propsize = sizeof(stg_property_t);
  PRINT_DEBUG1( "reading a property header of %d bytes", propsize );
#endif

  // read a header
  size_t result = stg_packet_read_fd( fd, prop, sizeof(*prop) );
  
  PRINT_DEBUG2( "read a property header of %d/%d bytes", 
		result, propsize );

  if( result == sizeof(*prop) )
    {	
      // see if the property has any data to follow
      if( prop->len > 0 )
	{
	  void* data = NULL;
	  assert( data = malloc(prop->len) );
	  
	  // read all the data
	  result = stg_packet_read_fd( fd, data, prop->len );
	  
	  PRINT_DEBUG2( "read property data of %d/%d bytes", 
			result, prop->len );

	  // copy the data into the property
	  prop = stg_property_attach_data( prop, data, prop->len );

	  free( data );
	}
    } 
  else
    {
      PRINT_DEBUG2( "short read of property header (%d/%d bytes)", 
		    result, propsize );
      
      stg_property_free(prop);
      prop = NULL; // indicates failure
    }
  return prop; 
}

// return a stg_property_t read from the client. caller must free the 
// property. On failure the channel is closed and NULL returned.
stg_property_t* stg_property_read( stg_client_t* cli )
{
  return stg_property_read_fd( cli->pollfd.fd );
}

// returns 0 on success, else -1
ssize_t stg_property_write_fd( int fd, stg_property_t* prop )
{
  assert( fd > 0 );
  size_t packetlen =  sizeof(stg_property_t) + prop->len;
  ssize_t result = stg_packet_write_fd( fd, prop, packetlen );
  
  if( result == 1 )
    perror( "error attempting to write a property" );
  else if( result == 0 )
    PRINT_WARN( "attempting to write a property wrote zero bytes" );
  
  return result;
}

ssize_t stg_property_write( stg_client_t* cli, stg_property_t* prop )
{
  return( stg_property_write_fd( cli->pollfd.fd, prop ) ); 
}

void stg_client_free( stg_client_t* cli )
{
  close( cli->pollfd.fd );
}

// returns a pointer to an allocated poll structure attached to the
// Stage server - use stg_destroy_client() to free the connection
stg_client_t* stg_client_create( char* host, int port )
{
  if( host == NULL )
    host = "localhost";

  if( port < 1 )
    port = STG_DEFAULT_SERVER_PORT;

  stg_client_t *cli = (stg_client_t*)malloc( sizeof( stg_client_t ) );
  assert( cli );
  
  memset( cli, 0, sizeof(*cli) );

  // get the IP of our host
  struct hostent* info = gethostbyname(  host );
  
  if( info )
    { // make sure this looks like a regular internet address
      assert( info->h_length == 4 );
      assert( info->h_addrtype == AF_INET );
    }
  else
    {
      PRINT_ERR1( "failed to resolve IP for remote host\"%s\"\n", 
		  host );
      return NULL;
    }
  struct sockaddr_in servaddr;
  
  /* open socket for network I/O */
  cli->pollfd.fd = socket(AF_INET, SOCK_STREAM, 0);
  cli->pollfd.events = POLLIN; // notify me when data is available
  
  // switch on the re-use-address option
  const int on = 1;
  setsockopt( cli->pollfd.fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on) );
  
  if( cli->pollfd.fd < 0 )
    {
      puts( "Error opening network socket for connection"  );
      fflush( stdout );
      return NULL;
    }
  
  /* setup our server address (type, IP address and port) */
  bzero(&servaddr, sizeof(servaddr)); /* initialize */
  servaddr.sin_family = AF_INET;   /* internet address space */
  servaddr.sin_port = htons( port ); /*our command port */ 
  memcpy(&(servaddr.sin_addr), info->h_addr_list[0], info->h_length);
 
  if( signal( SIGPIPE, stg_catch_pipe ) == SIG_ERR )
    {
      PRINT_ERR( "error setting pipe signal catcher" );
      exit( -1 );
    }
 
  if( connect( cli->pollfd.fd, 
               (struct sockaddr*)&servaddr, sizeof( servaddr) ) == -1 )
    {
      PRINT_ERR2( "Connection failed on %s:%d ", 
		  info->h_addr_list[0], port ); 
      perror( "" );
      fflush( stdout );
    return NULL;
    }
  
  // send the greeting
  stg_greeting_t greeting;
  greeting.code = STG_SERVER_GREETING;
  greeting.pid = getpid();
  
  int r;
  if( (r = write( cli->pollfd.fd, &greeting, sizeof(greeting) )) < 1 )
    {
      PRINT_ERR( "failed to write STG_SERVER_GREETING to server.\n" );
      if( r < 0 ) perror( "error on write" );
      return NULL;
    }
  
  // wait for the reply
  if( (r = read( cli->pollfd.fd, &greeting, sizeof(greeting) )) < 1 )
    {
      PRINT_ERR( "failed to READ STG_CLIENT_GREETING from server.\n" );
      if( r < 0 ) perror( "error on read" );
      return NULL;
    }
  
  if( greeting.code != STG_CLIENT_GREETING ) 
    PRINT_ERR1( "received bad reply from server (%d)", greeting.code );
  else
    PRINT_MSG1( "received good reply from server (%d)", greeting.code );

  return cli;
}  

stg_property_t* stg_send_property( stg_client_t* cli,
				   int id, 
				   stg_prop_id_t type,
				   stg_prop_action_t action,
				   void* data, 
				   size_t len )
{
  stg_property_t* prop =  stg_property_create();
  prop->id = id;
  prop->timestamp = 1.0;
  prop->property = type;
  prop->action = action;
  
  prop = stg_property_attach_data( prop, data, len);  
  stg_property_write( cli, prop );
  stg_property_free( prop );

  return( stg_property_read( cli ) );  
}  

// set a property of the model with the given id. 
// returns 0 on success, else -1.
int stg_set_property( stg_client_t* cli,
		      int id, 
		      stg_prop_id_t type,
		      void* data, 
		      size_t len )
{
  stg_property_t* reply = 
    stg_send_property( cli, id, type, STG_SET, data, len );
  
  if( reply == NULL )
    return -1;
 
  stg_property_free( reply );
  return 0; //ok
}

// gets the requested data from the server, allocating memory for the packet.
// caller must free() the data
// returns 0 on success, else -1.
int stg_get_property( stg_client_t* cli,
		      int id, 
		      stg_prop_id_t type,
		      void **data, 
		      size_t *len )
{
  stg_property_t* reply = 
    stg_send_property( cli, id, type, STG_GET, NULL, 0 );
  
  if( reply == NULL ) // fail
    {
      *data = NULL;
      *len = 0;
      return -1;
    }

  // success
  *data = calloc( reply->len, 1 );
  memcpy( *data, reply->data, reply->len );
  *len = reply->len;

  stg_property_free(reply);
  return 0;
}



stg_id_t stg_model_create( stg_client_t* cli, stg_entity_create_t* ent )
{
  stg_property_t* reply = stg_send_property( cli, -1, 
					     STG_PROP_CREATE_MODEL, 
					     STG_SETGET,
					     ent, sizeof(stg_entity_create_t) );
  if( reply == NULL )
    return -1;

  stg_id_t returned_id = reply->id;
  stg_property_free( reply );
  
  return returned_id;
}

int stg_model_destroy( stg_client_t* cli, stg_id_t id )
{
  stg_property_t* reply = stg_send_property( cli, id,
					     STG_PROP_DESTROY_MODEL, 
					     STG_SETGET,
					     NULL, 0 );
  if( reply == NULL )
    return -1;

  stg_id_t returned_id = reply->id;
  stg_property_free( reply );
  
  return( returned_id == -1 ?  0 : -1 );
}


void stg_los_msg_print( stg_los_msg_t* msg )
{
  printf( "Mesg - id: %d power: %d consume: %d len: %d bytes: %s\n",
	  msg->id, msg->power, msg->consume, msg->len, msg->bytes );
}


int stg_model_send_los_msg(  stg_client_t* cli, stg_id_t id, 
			     stg_los_msg_t *msg )
{
  stg_property_t* reply = stg_send_property( cli, id, 
					     STG_PROP_LOS_MSG,
					     STG_SETGET,
					     msg,sizeof(stg_los_msg_t));
  
  if( reply == NULL )
    return -1;

  memcpy( msg, reply->data, sizeof(stg_los_msg_t) );
  stg_property_free( reply );
  return 0;
}

int stg_model_exchange_los_msg(  stg_client_t* cli, stg_id_t id, 
				 stg_los_msg_t *msg )
{
  stg_property_t* reply = stg_send_property( cli, id, 
					     STG_PROP_LOS_MSG,
					     STG_SETGET,
					     msg,sizeof(stg_los_msg_t));
  
  if( reply == NULL )
    return -1;

  memcpy( msg, reply->data, sizeof(stg_los_msg_t) );
  stg_property_free( reply );
  return 0;
}

stg_id_t stg_world_create( stg_client_t* cli, stg_world_create_t* world )
{
  stg_property_t* reply = stg_send_property( cli, -1,
					     STG_PROP_CREATE_WORLD, 
					     STG_SETGET,
					     world, 
					     sizeof(stg_world_create_t));  
  if( reply == NULL )
    return -1;
  
  stg_id_t returned_id = reply->id;
  stg_property_free( reply );
  
  return returned_id;
}

///////////////////////////////////////////////////////////////////////////
// Look up the color in a data based (transform color name -> color value).
stg_color_t stg_lookup_color(const char *name)
{
  FILE *file;
  const char *filename;
  
  if( name == NULL ) // no string?
    return 0; // black
  
  if( strcmp( name, "" ) == 0 ) // empty string?
    return 0; // black

  filename = COLOR_DATABASE;
  file = fopen(filename, "r");
  if (!file)
  {
    PRINT_ERR2("unable to open color database %s : %s",
               filename, strerror(errno));
    fclose(file);
    return 0xFFFFFF;
  }
  
  while (TRUE)
  {
    char line[1024];
    if (!fgets(line, sizeof(line), file))
      break;

    // it's a macro or comment line - ignore the line
    if (line[0] == '!' || line[0] == '#' || line[0] == '%') 
      continue;

    // Trim the trailing space
    while (strchr(" \t\n", line[strlen(line)-1]))
      line[strlen(line)-1] = 0;

    // Read the color
    int r, g, b;
    int chars_matched = 0;
    sscanf( line, "%d %d %d %n", &r, &g, &b, &chars_matched );
      
    // Read the name
    char* nname = line + chars_matched;

    // If the name matches
    if (strcmp(nname, name) == 0)
    {
      fclose(file);
      return ((r << 16) | (g << 8) | b);
    }
  }
  PRINT_WARN1("unable to find color [%s]; using default (red)", name);
  fclose(file);
  return 0xFF0000;
}
