/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _PROJECT_H_RPCGEN
#define _PROJECT_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif


typedef char *username;

typedef int status;

typedef char *domain_address;

typedef int port;

struct presence_info {
	status status0;
	domain_address domain_address0;
	port port0;
};
typedef struct presence_info presence_info;

struct update_presence_1_argument {
	username arg1;
	status arg2;
	domain_address arg3;
	port arg4;
};
typedef struct update_presence_1_argument update_presence_1_argument;

#define user 99
#define VER1 1

#if defined(__STDC__) || defined(__cplusplus)
#define register_user 1
extern  int * register_user_1(username , CLIENT *);
extern  int * register_user_1_svc(username , struct svc_req *);
#define update_presence 2
extern  int * update_presence_1(username , status , domain_address , port , CLIENT *);
extern  int * update_presence_1_svc(username , status , domain_address , port , struct svc_req *);
#define lookup 3
extern  presence_info * lookup_1(username , CLIENT *);
extern  presence_info * lookup_1_svc(username , struct svc_req *);
#define unregister 4
extern  int * unregister_1(username , CLIENT *);
extern  int * unregister_1_svc(username , struct svc_req *);
extern int user_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define register_user 1
extern  int * register_user_1();
extern  int * register_user_1_svc();
#define update_presence 2
extern  int * update_presence_1();
extern  int * update_presence_1_svc();
#define lookup 3
extern  presence_info * lookup_1();
extern  presence_info * lookup_1_svc();
#define unregister 4
extern  int * unregister_1();
extern  int * unregister_1_svc();
extern int user_1_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_username (XDR *, username*);
extern  bool_t xdr_status (XDR *, status*);
extern  bool_t xdr_domain_address (XDR *, domain_address*);
extern  bool_t xdr_port (XDR *, port*);
extern  bool_t xdr_presence_info (XDR *, presence_info*);
extern  bool_t xdr_update_presence_1_argument (XDR *, update_presence_1_argument*);

#else /* K&R C */
extern bool_t xdr_username ();
extern bool_t xdr_status ();
extern bool_t xdr_domain_address ();
extern bool_t xdr_port ();
extern bool_t xdr_presence_info ();
extern bool_t xdr_update_presence_1_argument ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_PROJECT_H_RPCGEN */
