#ifndef PACKETSENDOPERATION_H
#define PACKETSENDOPERATION_H


/*
 * Classe PacketSendOperation
 *
 * Hérite de la classe Packet
 * Gérer un type de packet spécifique
 *
 *
 *
 *
 * par Olivier
 */

// Project header
#include <Packet.hpp>

// forward declaration
class Chunk;

class PacketSendOperation : public Packet
{
private:
	

public:
	/**
	* Création d'un PacketSendOperation pret à être envoyé
	* 
	*/
	PacketSendOperation(std::string target, int idfile, int numChunk);
	
	/**
	* Constructeur sur un Packet (une maniere de caster un Packet)
	*/
	PacketSendOperation(const Packet& p);
	
	/**
	* Destructeur
	*/
	virtual ~PacketSendOperation();
	
	
	std::string getTarget ();
	
	int getIdFile ();
	
	int getNumChunk ();
	

private:
	PacketSendOperation() {}

};

#endif // PACKETSENDOPERATION_H
