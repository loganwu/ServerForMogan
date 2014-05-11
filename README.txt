------------------------------------
--BackEnd Game Server development
-------------------------------------
connect --> logicSVR--->Storage
1. connect
	use libevent
	refrence fastapi
2. logicSVR
	reference attach server
3. Storage
	use Mysql native API Wrapper
	also can reference  mysqlcppconn lib
