/**
 *
 * Serveur PolyPeer
 *
 * Fichier Main
 *
 * Olivier Blin
 */

// STL
#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <cstring>

// C library
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Project header
#include <PolypeerServer.hpp>
#include <Logger.hpp>

using namespace std;

void kill_handler(int sig);
void defineHandleStop();
void writePid(string fileName);
bool daemon_conf();

/**
 * Fonction de lecture des arguments du serveur
 */
int args(int argc, char* argv[], ServerOptions* opt);

/** 
 * Affichage l'utilisation de l'executable
 */
void display_usage(int argc, char* argv[]);

/**
 * Affiche l'aide de l'executable
 */
void display_help(int argc, char* argv[]);

int main(int argc, char* argv[])
{
	// variables
	ServerOptions opt = {6666, 5555, 7777};
	int pid;
	int result = args(argc, argv, &opt);
	
	if (result == 2)
	{
		// création d'un processus
		pid = fork();
		if (pid == -1)
		{
			cout << "Erreur, impossible de fork()" << endl;
			exit(1);
			
		}
		else if (pid == 0)
		{
			PolypeerServer* server = PolypeerServer::getInstance();
	
			if (!daemon_conf()) // Configuration du démon
			{
				cout << "Erreur, configuration du démon échoué" <<endl;
				exit(1);
			}
		
			(server->getLogger())<< "Lancement du serveur Polypeer en mode démon" << endlog;
			server->setConfig(&opt);
			server->start();
		
			// vidage mémoire
			delete server;
		}
		else
		{
			exit(0); // terminaison du père pour que le fils soit récupéré par init
		}
	}
	else if (result == 1)
	{
		display_help(argc, argv);
	}
	else if (result == 0)
	{
		// variable principale
		PolypeerServer* server = PolypeerServer::getInstance();
	
		// mise en place de l'interception d'un signal
		defineHandleStop();
		// pid du processus
		writePid("ppserver.pid");
	
		// programme principal
		cout << "+---------------------------------+" << endl;
		cout << "|         Server PolyPeer         |" << endl;
		cout << "+---------------------------------+" << endl;
		cout << "Serveur de distribution de fichiers volumineux en réseau" << endl;
		cout << "Pour les modalités d'utilisation des sources, veuillez lire" << endl;
		cout << "le fichier LICENCE, inclu dans le projet." << endl << endl;
		
		cout << "Fichier de log   : log/ppserver.log" << endl;
		cout << "Fichier de pid   : ppserver.pid" << endl;
		cout << "Port serveur     : " << opt.serverPort << endl;
		cout << "Port clients     : " << opt.clientPort << endl;
		cout << "Port webserver   : " << opt.webserverPort << endl;
		cout << "PID              : " << getpid() << endl;
		
		
		(server->getLogger()) << "Lancement du serveur Polypeer en mode normal" << endlog;
		server->setConfig(&opt);
		server->start();
	
		cout << "Serveur Polypeer terminé" << endl;
	
		delete server;
	}
	else
	{
		display_usage(argc, argv);
	}
	
	return 0;
}



int args(int argc, char* argv[], ServerOptions* opt)
{
	int result = 0;
	
	if (argc == 2)
	{
		if (strcmp("-d", *(argv + 1)) == 0)
		{
			result = 2;
		}
		else if (strcmp("-h", *(argv + 1)) == 0)
		{
			result = 1;
		}
	}
	
	for (int i = 0; i < argc - 1; i++) 
	{
		std::istringstream iss(*(argv + i + 1));
		
		if (strcmp("-c", *(argv + i)) == 0) 
		{
			iss >> opt->clientPort;
			i++;
		}
		else if (strcmp("-s", *(argv + i)) == 0) 
		{
			iss >> opt->serverPort;
			i++;
		}
		else if (strcmp("-w", *(argv + i)) == 0) 
		{
			iss >> opt->webserverPort;
			i++;
		}
		else if (strcmp("-d", *(argv + i)) == 0) 
		{
			result = 2;
		}
	}
	
	return result;
}

void display_usage(int argc, char* argv[])
{
	cout << "Usage: " << *argv << " [-hd] [-s serverPort] [-c clientPort] [-w webserverPort]" << endl; 
}

void display_help(int argc, char* argv[])
{
	cout << "Aide: " << *argv << ", client PolyPeer" << endl;
	cout << "[-s serverPort] Port du serveur" << endl;
	cout << "[-c clientPort] Port du client" << endl;
	cout << "[-w webserverPort] Port du serveur web" << endl;
	cout << "[-d] Lancer en démon" << endl;
	cout << "[-h] Affichage de l'aide" << endl;
}

void defineHandleStop()
{
	// Definition du catch d'arret
	struct sigaction act, oact;
	sigaction(SIGINT,NULL,&oact);
	act.sa_handler=kill_handler;
	act.sa_mask=oact.sa_mask;
	act.sa_flags=SA_RESTART;
	sigaction(SIGINT,&act, NULL);
}

void kill_handler(int sig)
{
	cout << endl << "Interception signal d'arret(" << sig << ")..." << endl;
	PolypeerServer* server = PolypeerServer::getInstance();
	server->stop();
}

void writePid(string fileName)
{
	ofstream file;
	file.open(fileName.c_str(), fstream::trunc);
	
	if (file.is_open()) 
	{
		file << getpid();
		file.close();
	}
}


bool daemon_conf ()
{
	// change the file mode mask
	//umask (0); 
	
	// Create a new SID for the child process
	int sid = setsid ();
	if (sid < 0)
		return false;
	
	// pid du processus
	writePid("ppserver.pid");
	
	// mise en place de l'interception d'un signal
	defineHandleStop();
	
	
	// change the current working directory
	//if ((chdir("/tmp")) < 0)
	//	return false;
	
	// close out the standart file descriptor
	close (STDIN_FILENO);
	close (STDOUT_FILENO);
	close (STDERR_FILENO);
	
	return true;
}



