#include "DBClient.h"
#include <iostream>

int main()
{
	CMysql mysqlInst("localhost","root","123","michelledata");
	try
	{
		mysqlInst.Connect();
	}
	catch(CMysqlException & cException)
	{
		std::cout<<cException.GetErrMsg()<<endl;
	}
	if(mysqlInst.IfConnected("localhost"))
	{
		std::cout<<"Connect success;"<<endl;
		try
		{
			//select 
			mysqlInst.Query("SELECT * from HeroInfo");
			while(mysqlInst.FetchRow())
			{
				char *Id = mysqlInst.GetField("HeroId");
                char *name = mysqlInst.GetField("HeroName");
				std::cout<<"HeroID: "<<Id<<" HeroName: "<<name<<endl;
			}
			//update
			mysqlInst.Query("UPDATE HeroInfo SET HeroName='DNF' where HeroId=3;");
			std::cout<<"Update rows: "<<mysqlInst.GetAffectedRows()<<endl;
			//delete	
			mysqlInst.Query("DELETE from HeroInfo where HeroId=10;");
			std::cout<<"DELETE rows: "<<mysqlInst.GetAffectedRows()<<endl;
			//insert	
			mysqlInst.Query("INSERT into HeroInfo (HeroId,HeroName) values (10,'alibaba');");
			std::cout<<"Insert rows: "<<mysqlInst.GetAffectedRows()<<endl;
			//replace	
			mysqlInst.Query("REPLACE into HeroInfo (HeroId,HeroName) values (4,'alibaba'),(5,'Tencent'),(6,'Baidu');");
			std::cout<<"REPLACE rows: "<<mysqlInst.GetAffectedRows()<<endl;
			
		}
		
		catch(CMysqlException & cException)
		{
			std::cout<<cException.GetErrMsg()<<endl;
		}
	}
	else
	{
		std::cout<<"Connect failed;"<<endl;
	}
	std::cout<<"MainGame Terminal"<<endl;		
}
