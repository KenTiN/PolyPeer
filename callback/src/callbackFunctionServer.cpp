// Class header
#include <callbackFunctionServer.hpp>

// STL
#include <iostream>

// Project header
#include <PolypeerServer.hpp>
#include <ServerData.hpp>
#include <Packet.hpp>
#include <includePacket.hpp>


using namespace std;

int callbackReady(Packet& p)
{
	PacketReady pp (p);

	ServerData& sd = PolypeerServer::getInstance()->getServerData();
	
	string src = p.getAddress();
	//cout << "callbackReady : " << src << " Num : " << pp.getChunkNumber() << endl;
	sd.updateHost (src, pp.getIdFile(), pp.getChunkNumber());
	
	// Réveil de l'algo
	sd.getSemaphore()->free();

	return 1;
}

int callbackChunkReceived(Packet& p)
{
	PacketChunkReceived pp (p);

	ServerData& sd = PolypeerServer::getInstance()->getServerData();
	
	string src = p.getAddress();
	//cout << "callbackChunkReceived : " << src << endl;
	sd.updateHost(src, pp.getIdFile(), pp.getChunkNumber());

	// Réveil de l'algo
	sd.getSemaphore()->free();
	
	return 1;
}

int callbackMd5Error(Packet& p)
{
	PacketMd5Error pp (p);

	ServerData& sd = PolypeerServer::getInstance()->getServerData();

	string src = p.getAddress();
	//cout << "callbackMd5Error : " << src << endl;
	sd.updateHost(src, WAIT);

	// Réveil de l'algo
	sd.getSemaphore()->free();
	
	return 1;
}

int callbackPacketDiskFull(Packet& p)
{
	PacketDiskFull pp (p);

	ServerData& sd = PolypeerServer::getInstance()->getServerData();
	
	string src = p.getAddress();
	//cout << "callbackPacketDiskFullError : " << src << endl;
	sd.updateHost(src, pp.getIdFile(), HDS_DISKFULL);

	// Réveil de l'algo
	sd.getSemaphore()->free();
	
	return 1;
}

int callbackPacketSendOperationFinished(Packet& p)
{
	PacketSendOperationFinished pp (p);

	ServerData& sd = PolypeerServer::getInstance()->getServerData();

	string src = p.getAddress();
	//cout << "callbackPacketSendOperationFinished : " << src << endl;
	sd.updateHost(src, WAIT);

	// Réveil de l'algo
	sd.getSemaphore()->free();
	
	return 1;
}

int callBackAddHost(Packet& p)
{
	PacketAddHost pp (p);
	
	ServerData& sd = PolypeerServer::getInstance()->getServerData();
	
	//PolypeerServer::getInstance()->multiSleep(2500);
	
	//cout << "PacketAddHost" << pp.getIpAddress() << endl;
	sd.updateHost(pp.getIpAddress(), WAIT);
	
	// Réveil de l'algo
	sd.getSemaphore()->free();
	
	return 1;
}

int callbackRemoveHost(Packet& p)
{
	PacketRemoveHost pp(p);
	
	ServerData& sd = PolypeerServer::getInstance()->getServerData();
	ConnectionManager* cm = sd.getConnectionManager();
	if(cm!=NULL)
	{
		cm->removeConnection(pp.getIpAddress());
		sd.updateHostInit(pp.getIpAddress());
	}

	
	
	//cout << "callbackRemoveHost" << pp.getIpAddress() << endl;
	
	
	// Réveil de l'algo
	sd.getSemaphore()->free();
	
	
	return 1;
}

