typedef string username<20>;
typedef int status;
typedef string domain_address<20>;
typedef int port;

struct presence_info {
	status status0;
	domain_address domain_address0;
	port port0;
};

program user {
	version VER1 {
		int  register_user(username) = 1;
		int update_presence(username, status, domain_address, port) = 2;
		presence_info lookup(username) = 3;
		int unregister(username)=4;
	} = 1;
} = 99;

