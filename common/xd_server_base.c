
#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/http.h>



/* 
 * 1. socket
 * 2. bind 
 * 3. fork
 * 4. try to lock 
 * 5. accept
 * 6. event loop
 * 7. exit
 */
