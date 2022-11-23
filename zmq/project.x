typedef string username<20>;
typedef int status;
typedef string domain_address<20>;
typedef int port;
typedef string is_broker<10>;

struct presence_info {
	username username0;
	status status0;
	domain_address domain_address0;
	port port0;
	is_broker is_broker0;
};

program user {
	version VER1 {
		int  register_user(username, is_broker) = 1;
		int update_presence(username, status, domain_address, port, is_broker) = 2;
		presence_info lookup(username) = 3;
		int unregister(username)=4;
		presence_info lookup_broker()=5;
	} = 1;
} = 99;

