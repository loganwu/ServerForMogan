/*
 * =====================================================================================
 *
 *       Filename:  hashTblExp.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/26/2015 03:14:02 PM CST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  loganwu (linux), loganwu@tencent.com
 *        Company:  tencent
 *
 * =====================================================================================
 */

#include "HashTable.h"
#include <iostream>
#include <algorithm>
#include <stdint.h>
using namespace std;
struct Hero
{
	char name[10];
	uint64_t ullHeroId;
};
int main()
{

	Logan::HashTable<Hero, uint64_t,100> cache;
	typedef Logan::HashTable<Hero, uint64_t,100> hashtype;
	Hero h1;
	hashtype::iterator it;
	h1.ullHeroId = 123;
	cout<<"ele count "<<cache.count()<<endl;
	cout<<"buckets count "<<cache.bucket_size()<<endl;
	pair<hashtype::iterator,int> ret = cache.insert(h1.ullHeroId,h1);
	cout<<"ele count "<<cache.count()<<endl;
	pair<hashtype::iterator,int> ret1 = cache.insert(h1.ullHeroId,h1);
	cout<<"ele count "<<cache.count()<<endl;
	Hero* pHero = cache.get(123);
	cout<<"hero heroid "<<pHero->ullHeroId<<endl;
	cache.erase(123);
	 pHero = cache.get(123);
	cout<<"after erase ele count "<<cache.count()<<" heroid 123 exists: "<<(pHero!=NULL)<<endl;
	
	return 0;
}
