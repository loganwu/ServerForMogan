------------------------------------
BackEnd Game Server development
-------------------------------------

connect --> logicSVR--->Storage

1. connect
	use libevent
2. logicSVR
	Key point: load balance;High performance;overload protection;flexibly available;
	reference attach server
3. Storage
	use Mysql native API Wrapper
	also can reference  mysqlcppconn lib
4.data cache 
	use unix shm,reference attach server
	use memcache opensource
5. data representation: google protobuf
6. Game architecture: global area and global server, logically divide into single area
7. Add some basical data structure: heap hash sort vector etc.
