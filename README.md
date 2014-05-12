------------------------------------
BackEnd Game Server development
-------------------------------------

connect --> logicSVR--->Storage

1. connect
	use libevent
	refrence fastapi
2. logicSVR
	Key point: load balance;High performance;overload protection;flexibly available;
	reference attach server
3. Storage
	use Mysql native API Wrapper
	also can reference  mysqlcppconn lib
4.data cache 
	use unix shm,reference attach server
	use memcache opensource
5. global area global server
