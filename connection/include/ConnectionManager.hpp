#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include <map>
#include <string>
#include <vector>
#include <pthread.h>

#include <Connection.hpp>
#include <WaitingPackets.hpp>
#include <ServerSocket.hpp>

/**
 * Classe gérant les sockets d'écoute. Quand un socket est ajouté au connectionManager, il lance l'écoute dans un thread.
 * la classe permet de récupérer les socket qu'il contient par le nom du PC corespondant.
 */


class ConnectionManager
{
	public:
		/**
		 * Constructeur
		 * @param WaitingPacket*
		 * Pointeur vers l'instance de la liste des messages reçus. Ces sont les Connections contenus qui vont la mettre à jour.
		 */
		ConnectionManager(WaitingPackets* waitingPackets);
		virtual ~ConnectionManager();

		/**
		 * Permet d'ajouter une connection au manager. Le Manager va lancer l'écoute dans un thread/
		 * @param string
		 * le nom de l'ordinateur connecté sur ce socket
		 * @param Socket*
		 * pointeur vers la socket correspondant au nom et qui va être écouté.
		 */
		void addConnection(std::string name, Socket* socket);

		/**
		 * Permet de récupérer l'instance de connection qui correspond au nom passé en paramètre.
		 * @param string
		 * nom de l'ordinateur dont on veut récupérer la connection
		 * @return Connection*
		 * Pointeur vers la connection correspondante au nom passé en paramètre.
		 */
		Connection* getConnection(std::string name);

		/**
		 * Lance l'écoute des connexions entrantes et l'jout de ces connexions dans le tableau de Conection dans un thread
		 */
		void start();

		/**
		 * Arrête le thread d'écoute des connexions entrantes et coupe toutes les connexions en cours.
		 * A n'utiliser qu'à la fermeture du programme!
		 */
		void stop();


	private:
		std::map<std::string, Connection*> listConnections;
		WaitingPackets* waitingPackets;

		pthread_t thread;
		bool run;
		ServerSocket* serverSocket;

		friend void* runFct(void* connectionManager);
};

#endif // CONNECTIONMANAGER_H
