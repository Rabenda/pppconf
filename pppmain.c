/*
 * FreeBSD PPPoE configuration tool
 * (C)2016 Bogdan Nikolaev. All rights reserved
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

/*
 * How works this program?
 * This program simply configure your /etc/ppp/ppp.conf file with your name and password
 * And this program can connect you to internet using ppp daemon
 * What you need to use this program?
 * You need at least ethernet card and your ISP using PPPoE method to connect clients to internet
 * Warning: This program not running DHCP! 
 * How to configure your PPPoE connection? Simply execute command "pppcfg"
 * How to connect to internet using your configured PPPoE connection? Simply execute command "pppconf -connect"
 */

#include "ppp.h"

// Entry point
int main(int argc, char **argv)
{
	if(argc > 1)
	{
		// Check if need connect
		if(strcmp(argv[0], stringConnectArgument))
		{
			system("ppp -ddial pppconf");
			return 0;
		}
		else
		{
			printString(stringHelp);
			return 0;
		}
	}
	
	string inp;
	printString(stringWelcome);
	printString(stringCopy);
	// Check is ppp configuration directory exists
	if(isConfigDirectoryExists(pathPPP) != true) {
		printString(stringPPPDirNotFound);
		getInput(inp);
		// If yes then create dir and continue working
		if(strcmp(inp, stringYes) == succeful) {
			int dirstatus = mkdir(pathPPP, pppDirRights);
			// Create ppp.conf
			FILE *f = fopen(pathPPPConfig, "w");
			fclose(f);
			// If error - kill program
			if(dirstatus != succeful) {
				printString(stringFailedCreatePPPDir);
				return 0;
			}
		}
		
		// If no then terminate program
		if(strcmp(inp, stringNo) == succeful) {
			printString(stringDirNotFound);
			return 0;
		}
		
		// If not yes and not no(i.e invalid answer)
		if(strcmp(inp, stringNo) == succeful || strcmp(inp, stringYes) == succeful) {
			printString(stringBadAnswer);
			return 0;
		}
	}
	
// Label
	configuration:
	{
		char usrname[maxCharBuffer];
		char usrpswd[maxCharBuffer];
		char ethadapter[maxCharBuffer];
		printString(stringEnterYourUserName);
		getInput(usrname);
		printString(stringEnterYourPassword);
		getInput(usrpswd);
		printString(stringEnterEthAdapter);
		getInput(ethadapter);
		isConfigCorrect:
		{
			printString(stringIsConfigurationCorrect);
			printf(stringUserName, usrname, "\n");
			printf(stringUserPassword, usrpswd, "\n");
			printf(stringEthernetAdapter, ethadapter, "\n");
			getInput(inp);
			if(strcmp(inp, stringYes) == succeful)
			{
				int result = ConfigurePPP(usrname, usrpswd, ethadapter);
				if(result == succeful)
				{
					printString(stringSuccess);
					return 0;
				}
				if(result == unsucceful)
				{
					printString(stringUnsuccess);
					return 0;
				}	
			}
	
	
		if(strcmp(inp, stringNo) == succeful)
		{
			goto configuration;
		}
		
		// If not yes and no no
		goto isConfigCorrect;
	}
}
  return 0;
}

void printString(string str)
{
    puts(str);
}

void getInput(string target)
{
	// Because gets() is dangerous function, i use fgets
    fgets(target, maxCharBuffer, stdin);
    target[strlen(target)-1] = '\0';
}
	
bool isConfigDirectoryExists(string path)
{
	DIR* dir = opendir(path);
	if(dir)
	{
		closedir(dir);
		return true;
	}
	if(ENOENT == errno) {
		return false;
	}
	return false;
}

int ConfigurePPP(string username, string userpassword, string adapter)
{
	FILE *f = fopen(pathPPPConfig, "w");
	// If error
	if(f == NULL)
	{
	    return unsucceful;	
	}
	fputs(stringComment, f);
	fputs(stringCommentCopy, f);
	fputs("default: \n", f);
	fputs("  set log Phase tun command \n", f);
	fputs("\n", f);
	fputs("pppconf: \n", f);
	fprintf(f, "  set device PPPoE:%s%s", adapter, "\n");
	fputs("  set dial \"\" \n", f);
	fprintf(f, "  set authname %s%s", username, "\n");
	fprintf(f, "  set authkey %s%s", userpassword, "\n");
    fputs("  set ifaddr 0 0 \n", f);
    fputs("  add default HISADDR \n", f);
    fclose(f);
	return succeful;
}
